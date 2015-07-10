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

#include "register_display.c"

/* Dump Bytes, Dump Words, Dump Double Words */
typedef enum {BYTE_STRIDE, WORD_STRIDE, DWORD_STRIDE} Stride;   
enum {MAX_BREAKPOINTS = 10};
bool run = false;

/* Main command loop */
bool command_loop(Cpu *cpu)
{
  static uint16_t breakpoint_addresses[MAX_BREAKPOINTS];
  static uint8_t cur_bp_number = 0;
  char cmd[512];
  char *line;

  /* Check for breakpoints */
  int i;
  for (i = 0;i < cur_bp_number;i++) {
    if (cpu->pc == breakpoint_addresses[i]) {
      run = 0; /* Stop fast execution */
      debug_mode = true;
      printf("\n\t[Breakpoint %d hit]\n\n", i + 1);
      break;
    }
  }

  if (!disassemble_mode && debug_mode) {
    display_registers(cpu);
    disassemble(cpu, 1, false);
  }

  while (!run) {
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

    /* s NUM_STEPS, step X instructions forward, defaults to 1 */
    if ( !strncasecmp("s", cmd, sizeof "s") ||
	 !strncasecmp("step", cmd, sizeof "step")) {

      unsigned int num_of_steps = 0;
      
      if (line[1] == ' ') {
	sscanf(line, "%u", &num_of_steps);
	printf("TODO:Stepping %u\n", num_of_steps);
      }

      break;
    }                                 

    /* run, run the program until a breakpoint is hit */
    else if ( !strncasecmp("quit", cmd, sizeof "quit") ||
	      !strncasecmp("q", cmd, sizeof "q")) {
      return false;
    }

    /* run, run the program until a breakpoint is hit */
    else if ( !strncasecmp("run", cmd, sizeof "run") ||
	      !strncasecmp("r", cmd, sizeof "r")) {
      run = true;
      debug_mode = false;
      
      break;
    }

    /* Display disassembly */
    else if ( !strncasecmp("disas", cmd, sizeof "disas") ||
	      !strncasecmp("dis", cmd, sizeof "dis") ||
	      !strncasecmp("disassemble", cmd, sizeof "disassemble")) {

      disassemble(cpu, 10, false);
      continue;
    }

    /* Display all 16 registers */
    else if ( !strncasecmp("regs", cmd, sizeof "regs")) {
      display_registers(cpu);
      continue;
    }
    
    /* Display all breakpoints */
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

    /* d(b/w/d) ADDRESS, dump bytes, words, or double words at ADDRESS */
    /* d(b/w/d) REGISTER, dump from register value address */
    else if ( cmd[0] == 'd' &&
	      (cmd[1] == 'b' || cmd[1] == 'w' || cmd[1] == 'd') ) {

      char param1[33] = {0};
      uint32_t start_addr, stride;
      sscanf(line, "%s", param1);
      
      /* Is it a direct address or a an address in a register being spec'd */
      if (param1[0] >= '0' && param1[0] <= '9') {
	sscanf(param1, "%X", &start_addr);
      }
      else if (param1[0] == '%' || param1[0] == 'r') {   /* got Register */
	start_addr = (uint16_t) *get_reg_ptr(cpu, reg_name_to_num(param1));
      }
      
      if (cmd[1] == 'b') {
	stride = BYTE_STRIDE;
      }
      else if (cmd[1] == 'w') {
	stride = WORD_STRIDE;
      }
      else {
	stride = DWORD_STRIDE;
      }

      dump_memory(MEMSPACE, 0x0, start_addr, stride);	
    }

    /* Set REG/LOC VALUE */
    else if ( !strncasecmp("set", cmd, sizeof "set") ) {
      int value = 0;
      char reg_name_or_addr[33];
      
      sscanf(line, "%s %X", reg_name_or_addr, &value);

      if ( reg_name_to_num(reg_name_or_addr) != -1 ) {
	*get_reg_ptr( cpu, reg_name_to_num(reg_name_or_addr) ) = value;
	display_registers(cpu);
	disassemble(cpu, 1, false);
      }
      else {
	uint16_t virtual_addr = (uint16_t) strtol(reg_name_or_addr, NULL, 0);
	*get_addr_ptr(virtual_addr) = value;
      }

      continue;
    }

    /* break BREAKPOINT_ADDRESS */
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

    /* help, display a list of debugger cmds */
    else if ( !strncasecmp("help", cmd, sizeof "help") ||
	      !strncasecmp("h", cmd, sizeof "h") ) {
      display_help();
    }

    /* End the line loop, next instruction */
    else {
      puts("\t[Invalid command, type \"help\".]");
      continue;
    }
  }

  return true;
}

//##########+++ Dump Memory Function +++##########
void dump_memory(uint8_t *MEM, uint32_t size, 
		 uint32_t start_addr, uint8_t stride)
{
  uint32_t i, msp_addr = start_addr;
  MEM += start_addr;

  puts("");

  for (i = 0; i < 32; i += 8) {
    printf("0x%04X:\t", msp_addr);

    if ( stride == BYTE_STRIDE ) {
      printf("0x%02X  0x%02X  0x%02X  0x%02X  " \
	     "0x%02X  0x%02X  0x%02X  0x%02X\n",
             *(MEM+0),*(MEM+1),*(MEM+2),*(MEM+3),
	     *(MEM+4),*(MEM+5),*(MEM+6),*(MEM+7));
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
    
    MEM += 8;        /* Increase character by 4 */
    msp_addr += 8;   /* Increase msp_addr by 4  */
  }

  puts("");
}

void handle_sigint(int sig)
{
  run = false;
  debug_mode = true;
}

void register_signal(int sig)
{
  signal(sig, handle_sigint);
}
