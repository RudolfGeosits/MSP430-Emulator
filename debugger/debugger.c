/*
  MSP430 Emulator                     
  Copyright (C) 2014, 2015 Rudolf Geosits (rgeosits@live.esu.edu)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or 
  (at your option) any later version.   
                           
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.               
                                                         
  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses
*/

#include "debugger.h"

Emulator *local_emu = NULL;

bool exec_cmd (Emulator *emu, char *line, int len) 
{
  Cpu *cpu = emu->cpu;
  Debugger *deb = emu->debugger;

  char cmd[100] = {0};
  unsigned int op1 = 0, op2 = 0, op3 = 0;
  int ops;

  char bogus1[100] = {0};
  uint32_t bogus2 = 0, bogus3 = 0;

  ops = sscanf(line, "%s %u %u", cmd, &op1, &op2);
  //printf("Got %s, %u, %u - ops %d\n", cmd, op1, op2, ops);

  /* RESET / RESTART
     
     Resets the entire virtual machine to it's starting state.
     Puts the starting address back into Program Counter
   */
  if ( !strncasecmp("reset", cmd, sizeof "reset") ||
       !strncasecmp("restart", cmd, sizeof "restart")) 
    {      
      cpu->pc = 0xC000;

      display_registers(emu);
      disassemble(emu, cpu->pc, 1);
    }
  
  // s [NUM], step NUM instructions forward, defaults to 1 // 
  else if ( !strncasecmp("s", cmd, sizeof "s") ||
	    !strncasecmp("step", cmd, sizeof "step")) 
    {
      int steps = 1; // 1 step by default
      uint32_t i;

      if (ops == 2) {
	steps = (int) op1;
      }

      for (i = 0;i < steps;i++) {
	decode(emu, fetch(emu), EXECUTE);

        // Handle Peripherals     
	handle_bcm(emu);
	handle_timer_a(emu);
	handle_port_1(emu);
	handle_usci(emu); 
      }

      display_registers(emu);
      disassemble(emu, cpu->pc, 1);
  }                                
  
  // Quit program //
  else if ( !strncasecmp("quit", cmd, sizeof "quit") ||
	    !strncasecmp("q", cmd, sizeof "q")) 
    {    
      // This flag stops the main loop in main.c
      deb->quit = true;
    }
  
  // run the program until a breakpoint is hit //
  else if ( !strncasecmp("run", cmd, sizeof "run") ||
	    !strncasecmp("r", cmd, sizeof "r")) 
    {
      cpu->running = true;
      deb->debug_mode = false;

      update_register_display(emu);
    }

  // Display disassembly of N at HEX_ADDR: dis [N] [HEX_ADDR] //
  else if ( !strncasecmp("disas", cmd, sizeof "disas") ||
	    !strncasecmp("dis", cmd, sizeof "dis") ||
	    !strncasecmp("disassemble", cmd, sizeof "disassemble")) 
    {
      uint16_t start_addr = cpu->pc;
      uint32_t num = 10; 
      
      ops = sscanf(line, "%s %u %X", bogus1, &bogus2, &bogus3);
      
      if (ops == 2) {
	sscanf(line, "%s %u", bogus1, &num);
      }
      else if (ops == 3) {
	sscanf(line, "%s %u %X", bogus1, &num, 
		     (unsigned int *)&start_addr);
      }
      
      disassemble(emu, start_addr, num);
    }

  else if ( !strncasecmp("dump", cmd, sizeof "dump" )) 
    {
      char str[100] = {0};
      uint16_t start_addr = cpu->pc;
      uint32_t stride;
      
      sscanf(line, "%s %s", bogus1, str);
      
      // Is it a direct address or an adress in a register being spec'd 
      if (str[0] >= '0' && str[0] <= '9') {
	sscanf(str, "%X", (unsigned int *) &start_addr);
      }
      else if (str[0] == '%' || str[0] == 'r' || str[0] == 'R'){
	uint16_t *p = get_reg_ptr(emu, reg_name_to_num(str));
	start_addr = *p;
      }
      
      stride = BYTE_STRIDE;
      dump_memory(emu, MEMSPACE, 0x0, start_addr, stride);	
    }

  // Set REG/LOC VALUE 
  else if ( !strncasecmp("set", cmd, sizeof "set") ) 
    {
      uint16_t value = 0;
      char reg_name_or_addr[50] = {0};
      char *reg_name = NULL;
      char *addr_str = NULL;

      //print_console(emu, "Not yet implemented.\n");
      
      sscanf(line, "%s %s %X", bogus1, reg_name_or_addr, 
	     (unsigned int*)&value);
      
      //printf("Got %s and %X\n", reg_name_or_addr, value);

      // Figure out if the value given is a reg name or addr
      int res = reg_name_to_num(reg_name_or_addr);

      if (res != -1) {  // If its a reg name
	reg_name = reg_name_or_addr;
	printf("In reg part...\n");
	
	if (res == 2) { // SR (R2)
	  set_sr_value(emu, value);
	}
	else { // All others
	  uint16_t *p = get_reg_ptr(emu, res);
	  *p = value;
	}

	display_registers(emu);
	disassemble(emu, cpu->pc, 1);
      }
      else {
	addr_str = reg_name_or_addr;
	printf("In addr part...\n");

	uint16_t virtual_addr = (uint16_t) strtol(addr_str, NULL, 0);

	uint16_t *p = get_addr_ptr(virtual_addr);
	*p = value;
      }
    }

  // break BREAKPOINT_ADDRESS - set breakpoint //
  else if ( !strncasecmp("break", cmd, sizeof "break") ) 
    {
      if (deb->num_bps >= MAX_BREAKPOINTS) {
	//printf("Breakpoints are full.\n");
	print_console(emu, "Breakpoints are full.\n");

	return true;
      }
    
      ops = sscanf(line, "%s %X", bogus1, &bogus2);
      char entry[100] = {0};

      if (ops == 2) {
	sscanf(line, "%s %X", bogus1, (unsigned int *) 
	       &deb->bp_addresses[deb->num_bps]);
      
	sprintf(entry, "\n\t[Breakpoint [%d] Set]\n", deb->num_bps + 1);
	//printf("%s", entry);
	print_console(emu, entry);

	++deb->num_bps;
      }
      else {
	//printf("error\n");
	print_console(emu, "error\n");
      }
    }

  // Display all breakpoints //
  else if ( !strncasecmp("bps", cmd, sizeof "bps" )) 
    {
      char entry[100] = {0};

      if (deb->num_bps > 0) {
	deb->current_bp = 0;

	while (deb->current_bp < deb->num_bps) {
	  sprintf(entry, "\t[%d] 0x%04X\n", 
		  deb->current_bp+1, deb->bp_addresses[deb->current_bp]);

	  //printf("%s", entry);
	  print_console(emu, entry);

	  ++deb->current_bp;
	}
      }
      else {
	//printf("You have not set any breakpoints!\n");
	print_console(emu, "You have not set any breakpoints!\n");
      }
    }

  // Display registers //
  else if ( !strncasecmp("regs", cmd, sizeof "regs")) 
    {
      display_registers(emu);
      disassemble(emu, cpu->pc, 1);
    }

  // help, display a list of debugger cmds //
  else if ( !strncasecmp("help", cmd, sizeof "help") ||
	    !strncasecmp("h", cmd, sizeof "h") ) 
    {
      display_help(emu);
    }

  // End the line loop, next instruction
  else 
    {
      print_console(emu, "\t[Invalid command, type \"help\".]\n");
    }

  return true;
}

/* Main command loop */
bool command_loop (Emulator *emu, char *buf, int len)
{
  /*
  Cpu *cpu = emu->cpu;
  Debugger *debugger = emu->debugger;

  static uint16_t breakpoint_addresses[MAX_BREAKPOINTS];
  static uint8_t cur_bp_number = 0;

  char cmd[512];
  char *line;

  // Check for breakpoints //
  int i;
  for (i = 0;i < cur_bp_number;i++) {
    if (cpu->pc == breakpoint_addresses[i]) {
      debugger->run = 0; // Stop fast execution //
      debugger->debug_mode = true;
      printf("\n\t[Breakpoint %d hit]\n\n", i + 1);
      break;
    }
  }

  if (!debugger->disassemble_mode && debugger->debug_mode) {
    display_registers(emu);
    disassemble(emu, cpu->pc, 1);
  }
  
  while (!debugger->run) {
    bzero(cmd, sizeof cmd);    
    line = readline("\n>> ");

    if ( strlen(line) >= 1 ) {
      add_history(line);
      sscanf(line, "%s", cmd);
      line += strlen(cmd) + 1;
    }
    else {
      continue;
    }
    
    // reset the virtual machine
    if ( !strncasecmp("reset", cmd, sizeof "reset") ||
	 !strncasecmp("restart", cmd, sizeof "restart")) {
      
      cpu->pc = 0xC000;
      break;
    }
    // s NUM_STEPS, step X instructions forward, defaults to 1 
    else if ( !strncasecmp("s", cmd, sizeof "s") ||
	      !strncasecmp("step", cmd, sizeof "step")) {
      
      unsigned int num_of_steps = 0;

      if (line[1] == ' ') {
	sscanf(line, "%u", &num_of_steps);
	printf("TODO:Stepping %u\n", num_of_steps);
      }

      break;
    }                                 

    // run, run the program until a breakpoint is hit 
    else if ( !strncasecmp("quit", cmd, sizeof "quit") ||
	      !strncasecmp("q", cmd, sizeof "q")) {
      return false;
    }

    // run, run the program until a breakpoint is hit 
    else if ( !strncasecmp("run", cmd, sizeof "run") ||
	      !strncasecmp("r", cmd, sizeof "r")) {
      debugger->run = true;
      debugger->debug_mode = false;
      
      break;
    }

    // Display disassembly 
    else if ( !strncasecmp("disas", cmd, sizeof "disas") ||
	      !strncasecmp("dis", cmd, sizeof "dis") ||
	      !strncasecmp("disassemble", cmd, sizeof "disassemble")) {

      uint16_t start_addr;
      uint32_t num;
      int res;

      res = sscanf(line, "%X%u", (unsigned int *) &start_addr, &num);

      if (res <= 0) {
	start_addr = cpu->pc;
	num = 10;
      } 
      else if (res == 1) {
	num = 10;
      }

      if (num > 0) 
	disassemble(emu, start_addr, num);

      continue;
    }

    // Display all 16 registers 
    else if ( !strncasecmp("regs", cmd, sizeof "regs")) {
      display_registers(emu);
      continue;
    }
    
    // Display all breakpoints 
    else if ( !strncasecmp("bps", cmd, sizeof "bps" )) {
      if (cur_bp_number > 0) {
	int i;
	for (i = 0;i < cur_bp_number;i++) {
	  printf("\t[%d] 0x%04X\n", i + 1, breakpoint_addresses[i]);
	}
      }
      else {
	puts("You have not set any breakpoints!\n");
      }
      
      continue;
    }

    else if ( !strncasecmp("dump", cmd, sizeof "dump" )) {
      char param1[33] = {0};
      uint16_t start_addr;
      uint32_t stride;
      sscanf(line, "%s", param1);
      
      // Is it a direct address or a an address in a register being spec'd 
      if (param1[0] >= '0' && param1[0] <= '9') {
	sscanf(param1, "%X", (unsigned int *) &start_addr);
      }
      else if (param1[0] == '%' || param1[0] == 'r' || param1[0] == 'R') {   
	uint16_t *p = get_reg_ptr(emu, reg_name_to_num(param1));
	start_addr = *p;
      }
      
      stride = BYTE_STRIDE;
      dump_memory(MEMSPACE, 0x0, start_addr, stride);	
    }

    // Set REG/LOC VALUE 
    else if ( !strncasecmp("set", cmd, sizeof "set") ) {
      int value = 0;
      char reg_name_or_addr[33];
      
      sscanf(line, "%s %X", reg_name_or_addr, &value);

      if ( reg_name_to_num(reg_name_or_addr) != -1 ) {
	uint16_t *p = get_reg_ptr(emu, reg_name_to_num(reg_name_or_addr) );
	*p = value;

	display_registers(emu);
	disassemble(emu, cpu->pc, 1);
      }
      else {
	uint16_t virtual_addr = (uint16_t) strtol(reg_name_or_addr, NULL, 0);

	uint16_t *p = get_addr_ptr(virtual_addr);
	*p = value;
      }

      continue;
    }

    // break BREAKPOINT_ADDRESS 
    else if ( !strncasecmp("break", cmd, sizeof "break") ) {
      if (cur_bp_number >= MAX_BREAKPOINTS) {
	printf("Too many breakpoints.\n");
      }
      else {
	sscanf(line, "%X", (unsigned int *) 
	       &breakpoint_addresses[cur_bp_number]);
	printf("\n\t[Breakpoint [%d] Set]\n", cur_bp_number + 1);
	
	++cur_bp_number;
      }
      
      continue;
    }

    // help, display a list of debugger cmds 
    else if ( !strncasecmp("help", cmd, sizeof "help") ||
	      !strncasecmp("h", cmd, sizeof "h") ) {
      display_help(emu);
    }

    // End the line loop, next instruction 
    else {
      puts("\t[Invalid command, type \"help\".]");
      continue;
    }
  }
  */
  return true;
}

//##########+++ Dump Memory Function +++##########
void dump_memory ( Emulator *emu, uint8_t *MEM, uint32_t size, 
		   uint32_t start_addr, uint8_t stride)
{
  uint32_t i, msp_addr = start_addr;
  MEM += start_addr;
  char str[100] = {0};

  puts("");
  print_console(emu, "\n");

  for (i = 0; i < 32; i += 8) {
    sprintf(str, "0x%04X:\t", msp_addr);

    printf("%s", str);
    print_console(emu, str);

    if ( stride == BYTE_STRIDE ) {
      sprintf(str, "0x%02X  0x%02X  0x%02X  0x%02X  "\
	      "0x%02X  0x%02X  0x%02X  0x%02X\n",
	      *(MEM+0),*(MEM+1),*(MEM+2),*(MEM+3),
	      *(MEM+4),*(MEM+5),*(MEM+6),*(MEM+7));

      printf("%s", str);
      print_console(emu, str);
    }
    else if ( stride == WORD_STRIDE ) {
      printf("0x%02X%02X  0x%02X%02X  0x%02X%02X  0x%02X%02X\n",
             *(MEM+0),*(MEM+1),*(MEM+2),*(MEM+3),*(MEM+4),
	     *(MEM+5),*(MEM+6),*(MEM+7));
    }
    else if ( stride == DWORD_STRIDE ) {
      printf("0x%02X%02X%02X%02X  0x%02X%02X%02X%02X\n",
             *(MEM+0),*(MEM+1),*(MEM+2),*(MEM+3),*(MEM+4),
	     *(MEM+5),*(MEM+6),*(MEM+7));
    }
    
    MEM += 8;        // Increase character by 4
    msp_addr += 8;   // Increase msp_addr by 4
  }
  
  puts("");
}

void setup_debugger(Emulator *emu)
{
  local_emu = emu;
  Debugger *deb = emu->debugger;

  deb->debug_mode = true;
  deb->disassemble_mode = false;
  deb->quit = false;

  deb->web_interface = true;
  deb->web_server_ready = false;
  deb->web_firmware_uploaded = false;

  deb->console_interface = false;

  memset(deb->bp_addresses, 0, sizeof(deb->bp_addresses));
  deb->num_bps = 0;
  deb->current_bp = 0;
}

void handle_sigint(int sig)
{
  if (local_emu == NULL) return;

  local_emu->cpu->running = false;
  local_emu->debugger->debug_mode = true;
}

void register_signal(int sig)
{
  signal(sig, handle_sigint);
}

void handle_breakpoints (Emulator *emu)
{
  int i;
  Cpu *cpu = emu->cpu;
  Debugger *deb = emu->debugger;
  char str[100] = {0};

  for (i = 0;i < deb->num_bps;i++) {

    if (cpu->pc == deb->bp_addresses[i]) {
      cpu->running = false;
      deb->debug_mode = true;
      
      sprintf(str, "\n\t[Breakpoint %d hit]\n\n", i + 1);
      printf("%s", str);
      print_console(emu, str);
      
      display_registers(emu);
      disassemble(emu, cpu->pc, 1);
      
      return;
    }

  }
}
