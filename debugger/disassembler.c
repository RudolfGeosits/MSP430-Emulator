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

#include "disassembler.h"

void disassemble(Emulator *emu, uint16_t start_addr, uint8_t times)
{
  Cpu *cpu = emu->cpu;
  Debugger *debugger = emu->debugger;

  uint16_t saved_pc = cpu->pc, opcode;
  uint32_t i;

  debugger->disassemble_mode = true;
  cpu->pc = start_addr;
  
  for (i = 0;i < times;i++) {
    char addr_str[32] = {0};

    sprintf(addr_str, "0x%04X:\t", cpu->pc);

    printf("%s", addr_str);
    if (debugger->web_interface) print_console(emu, addr_str);

    opcode = fetch(emu);    
    decode(emu, opcode, DISASSEMBLE);
  }

  debugger->disassemble_mode = false;
  cpu->pc = saved_pc; // Restore PC
}
