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

#include "register_display.h"

/*   Display all 16 registers                                                  
 *   - Toggle between Common mode and Specific mode                         
 *     (Common: Display all as R0 - R15)                                   
 *     (Specific: Display Register usages like PC, SR, etc.)               
 */

void display_registers(Emulator *emu)
{
  Cpu *cpu = emu->cpu;
  Debugger *debugger = emu->debugger;

  typedef enum {UNDEF, LINUX, WINDOWS} System_t;
  System_t this_system;
  uint16_t r2 = sr_to_value(emu);

  char full[1000] = {0};

#ifdef __linux
  this_system = LINUX;
#else
  this_system = UNDEF;
#endif

  char *v_flag, *n_flag, *z_flag, *c_flag;
  char *red, *green, *cyan, *clear, *blue, *white, *yellow;
  char *reg_col, *spec_reg_col, *decor_col, *value_col;

  char *r0_name = "PC";
  char *r1_name = "SP";
  char *r2_name = "SR";
  char *r3_name = "CG2";
  char *r4_name = "R4";
  char *r5_name = "R5";
  char *r6_name = "R6";
  char *r7_name = "R7";
  char *r8_name = "R8";
  char *r9_name = "R9";
  char *r10_name = "R10";
  char *r11_name = "R11";
  char *r12_name = "R12";
  char *r13_name = "R13";
  char *r14_name = "R14";
  char *r15_name = "R15";    


  if (debugger->console_interface) {
    red = "\x1b[31;1m";
    green = "\x1b[32;1m";
    cyan = "\x1b[36;1m";
    blue = "\x1b[34;1m";
    white = "\x1b[37;1m";
    yellow = "\x1b[33;1m";
    clear = "";

    reg_col = blue;
    value_col = white;
    spec_reg_col = red;
    decor_col = green;

    v_flag = "V\x1b[0m";
    n_flag = "\x1b[36;1mN\x1b[0m";
    z_flag = "\x1b[36;1mZ\x1b[0m";
    c_flag = "\x1b[36;1mC\x1b[0m";
  }
  else {
    red = "";
    green = "";
    cyan = "";
    blue = "";
    white = "";
    yellow = "";
    clear = "";

    reg_col = "";
    value_col = "";
    spec_reg_col = "";
    decor_col = "";

    v_flag = "V";
    n_flag = "N";
    z_flag = "Z";
    c_flag = "C";
  }
  
  sprintf(full,
	  "\n%s%s%s:   %s%04X  "\
	  "%s%s%s:   %s%04X  "	\
	  "%s%s%s:   %s%04X  "\
	  "%s%s%s:  %s%04X  "\
	  "%s%s%s: %s%d\n"\
	  "%s%%%s%s%s:  %s%04X  "\
	  "%s%%%s%s%s:  %s%04X  "\
	  "%s%%%s%s%s:  %s%04X  "\
	  "%s%%%s%s%s:  %s%04X  "\
	  "%s%s%s: %s%d\n"\
	  "%s%%%s%s%s:  %s%04X  "\
	  "%s%%%s%s%s:  %s%04X  "\
	  "%s%%%s%s%s: %s%04X  "\
	  "%s%%%s%s%s: %s%04X  "\
	  "%s%s%s: %s%d\n"\
	  "%s%%%s%s%s: %s%04X  "\
	  "%s%%%s%s%s: %s%04X  "\
	  "%s%%%s%s%s: %s%04X  "\
	  "%s%%%s%s%s: %s%04X  "\
	  "%s%s%s: %s%d\n\n",
	 
	  red, r0_name, decor_col, value_col, (uint16_t)cpu->pc, 
	  red, r1_name, decor_col, value_col, (uint16_t)cpu->sp, 
	  red, r2_name, decor_col, value_col, (uint16_t)r2, 
	  red, r3_name, decor_col, value_col, (uint16_t)cpu->cg2, 

	  cyan, c_flag, decor_col, value_col, cpu->sr.carry,

	  decor_col, reg_col, r4_name, decor_col, value_col, (uint16_t)cpu->r4, 
	  decor_col, reg_col, r5_name, decor_col, value_col, (uint16_t)cpu->r5, 
	  decor_col, reg_col, r6_name, decor_col, value_col, (uint16_t)cpu->r6,
	  decor_col, reg_col, r7_name, decor_col, value_col, (uint16_t)cpu->r7, 

	  cyan, z_flag, decor_col, value_col, cpu->sr.zero,

	  decor_col, reg_col, r8_name, decor_col,value_col, (uint16_t)cpu->r8, 
	  decor_col, reg_col, r9_name, decor_col,value_col, (uint16_t)cpu->r9, 
	  decor_col, reg_col, r10_name, decor_col,value_col,(uint16_t)cpu->r10,
	  decor_col, reg_col, r11_name, decor_col,value_col,(uint16_t)cpu->r11, 

	  cyan, n_flag, decor_col, value_col, cpu->sr.negative, 

	  decor_col, reg_col, r12_name, decor_col,value_col,(uint16_t)cpu->r12, 
	  decor_col, reg_col, r13_name, decor_col,value_col,(uint16_t)cpu->r13, 
	  decor_col, reg_col, r14_name, decor_col,value_col,(uint16_t)cpu->r14,
	  decor_col, reg_col, r15_name, decor_col,value_col,(uint16_t)cpu->r15,

	  cyan, v_flag, decor_col, value_col, cpu->sr.overflow);

  printf("%s", full);
  print_console(emu, full);
}
