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
void command_loop()
{
  static uint16_t breakpoint_addresses[MAX_BREAKPOINTS];
  static uint8_t cur_bp_number = 0;
  char command[333];

  /* Check for breakpoints */
  int i;
  for (i = 0;i < cur_bp_number;i++) {
    if (PC == breakpoint_addresses[i]) {
      run = 0; /* Stop fast execution */
      debug_mode = true;
      printf("\n\t[Breakpoint %d hit]\n\n", i + 1);
      break;
    }
  }

  if (!disassemble_mode && debug_mode) {
    display_registers();
    disassemble(1, false);
  }

  while (!run) {
    memset(command, 0, sizeof(command));
    scanf("%s", command);
    filter_uppercase(command);

    /* s NUM_STEPS, step X instructions forward, defaults to 1 */
    if ( strncmp("s", command, sizeof "s") == 0 ||
	 strncmp("step", command, sizeof "step") == 0) {

      unsigned int num_of_steps = 0;
      
      if (command[1] == ' ') {
	scanf("%u", &num_of_steps);
	printf("TODO:Stepping %u\n", num_of_steps);
      }

      break;
    }                                 

    /* run, run the program until a breakpoint is hit */
    else if ( strncmp("exit", command, sizeof "exit") == 0 ||
	      strncmp("e", command, sizeof "e") == 0) {
      exit(0);
      break;
    }

    /* run, run the program until a breakpoint is hit */
    else if ( strncmp("run", command, sizeof "run") == 0 ||
	      strncmp("r", command, sizeof "r") == 0) {
      run = true;
      debug_mode = false;
      
      break;
    }

    /* Display disassembly */
    else if ( strncmp("disas", command, sizeof "disas") == 0 ||
	      strncmp("dis", command, sizeof "dis") == 0 ||
	      strncmp("disassemble", command, sizeof "disassemble") == 0) {

      disassemble(10, false);
      continue;
    }

    /* Display all 16 registers */
    else if ( strncmp("regs", command, sizeof "regs") == 0 ) {
      display_registers();
      continue;
    }
    
    /* Display all breakpoints */
    else if ( strncmp("bps", command, sizeof "bps" ) == 0) {
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
    else if ( command[0] == 'd' && 
	      (command[1] == 'b' || command[1] == 'w' || command[1] == 'd') ) {

      char param1[20];
      uint32_t start_addr, stride;
      
      memset(param1, 0, sizeof param1);
      scanf("%s", param1);
      filter_uppercase(param1);

      if (param1[0] >= 0x30 && param1[0] <= 0x39) {   /* Runt hex ADDRESS */
	sscanf(param1, "%X", &start_addr);
      }
      else if (param1[0] == '%' || param1[0] == 'r') {   /* got Register */
	start_addr = (uint16_t) *get_reg_ptr( reg_name_to_num(param1) );
      }
      
      if (command[1] == 'b') {
	stride = BYTE_STRIDE;
      }
      else if (command[1] == 'w') {
	stride = WORD_STRIDE;
      }
      else {
	stride = DWORD_STRIDE;
      }

      dump_memory(MEMSPACE, 0x0, start_addr, stride);	
    }

    /* Set REG/LOC VALUE */
    else if ( strncmp("set", command, sizeof "set") == 0 ) {
      int value = 0;
      char reg_name_or_addr[33];
      
      scanf("%s %X", reg_name_or_addr, &value);

      if ( reg_name_to_num(reg_name_or_addr) != -1 ) {
	*get_reg_ptr( reg_name_to_num(reg_name_or_addr) ) = value;
	display_registers();
	disassemble(1, false);
      }
      else {
	uint16_t virtual_addr = (uint16_t) strtol(reg_name_or_addr, NULL, 0);
	*get_addr_ptr(virtual_addr) = value;
      }

      continue;
    }

    /* break BREAKPOINT_ADDRESS */
    else if ( strncmp("break", command, sizeof "break") == 0 ) {
      if (cur_bp_number >= MAX_BREAKPOINTS) {
	printf("Too many breakpoints.\n");
      }
      else {
	scanf("%X", (unsigned int *) 
	      &breakpoint_addresses[cur_bp_number]);
	printf("\n\t[Breakpoint [%d] Set]\n", cur_bp_number + 1);
      
	++cur_bp_number;
      }
      
      continue;
    }

    /* help, display a list of debugger commands */
    else if ( strncmp("help", command, sizeof "help") == 0 ) {
      printf("\n  run :\t\t\t\tRun Program Until Breakpoint is Hit\n"\
	     "  step :\t\t\tStep Into Instruction\n"\
	     "  disassemble :\t\t\tDisassemble Instructions\n"\
	     "  break ADDR :\t\t\tSet a Breakpoint\n"\
	     "  set HEX_ADDR|Rn :\t\tSet Memory or Register\n"\
	     "  bps :\t\t\t\tDisplay Breakpoints\n"\
	     "  regs :\t\t\tDisplay Registers\n"\
	     "  d(b|w|d) HEX_ADDR|Rn :\tDump Memory or Register\n"\
	     "  CTRL+C :\t\t\tPause Execution\n"\
	     "  quit :\t\t\tExit program\n"\
      );
    }

    /* End the command loop, next instruction */
    else {
      
      break;
    }
  }
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
