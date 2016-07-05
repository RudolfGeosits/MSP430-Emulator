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
  along with this program. If not, see <http://www.gnu.org/licenses/>
*/

#include "registers.h"

//##########+++ MSP430 Register initialization +++##########
void initialize_msp_registers(Emulator *emu)
{
  Cpu *cpu = emu->cpu;
  Debugger *debugger = emu->debugger;

  /* Initialize PC to boot loader code on cold boot (COLD)*/
  //cpu->pc = 0x0C00;

  /* Initialize Program Counter to *0xFFFE at boot or reset (WARM)*/
  cpu->pc = 0xC000;
  
  /* Stack pointer typically begins at the top of RAM after reset */
  cpu->sp = 0x400;

  /* Initialize the status register */
  memset(&cpu->sr, 0, sizeof(Status_reg));

  cpu->running = false;
  cpu->cg2 = 0;

  cpu->r4 = cpu->r5 = cpu->r6 = cpu->r7 = cpu->r8 = 
    cpu->r9 = cpu->r10 = cpu->r11 = cpu->r12 = cpu->r13 = 
    cpu->r14 = cpu->r15 = 0;
}

void update_register_display (Emulator *emu) 
{
  Cpu *cpu = emu->cpu;  
  char thing[50] = "....";  

  if (emu->cpu->running) {
    send_control(emu, UPDATE_ALL_REGS_PACKET, (void *)thing, strlen(thing));
    
    return;
  }

  sprintf(thing, "%04X", cpu->pc);
  send_control(emu, UPDATE_REG_R0_PACKET, (void *)thing, strlen(thing));

  sprintf(thing, "%04X", cpu->sp);
  send_control(emu, UPDATE_REG_R1_PACKET, (void *)thing, strlen(thing));

  sprintf(thing, "%04X", sr_to_value(emu));
  send_control(emu, UPDATE_REG_R2_PACKET, (void *)thing, strlen(thing));  

  sprintf(thing, "%04X",(uint16_t) cpu->cg2);
  send_control(emu, UPDATE_REG_R3_PACKET, (void *)thing, strlen(thing));  

  sprintf(thing, "%04X",(uint16_t) cpu->r4);
  send_control(emu, UPDATE_REG_R4_PACKET, (void *)thing, strlen(thing));  

  sprintf(thing, "%04X",(uint16_t) cpu->r5);
  send_control(emu, UPDATE_REG_R5_PACKET, (void *)thing, strlen(thing));  

  sprintf(thing, "%04X",(uint16_t) cpu->r6);
  send_control(emu, UPDATE_REG_R6_PACKET, (void *)thing, strlen(thing));  

  sprintf(thing, "%04X",(uint16_t) cpu->r7);
  send_control(emu, UPDATE_REG_R7_PACKET, (void *)thing, strlen(thing));  

  sprintf(thing, "%04X",(uint16_t) cpu->r8);
  send_control(emu, UPDATE_REG_R8_PACKET, (void *)thing, strlen(thing));  

  sprintf(thing, "%04X",(uint16_t) cpu->r9);
  send_control(emu, UPDATE_REG_R9_PACKET, (void *)thing, strlen(thing));  

  sprintf(thing, "%04X", (uint16_t)cpu->r10);
  send_control(emu, UPDATE_REG_R10_PACKET, (void *)thing, strlen(thing));  

  sprintf(thing, "%04X", (uint16_t)cpu->r11);
  send_control(emu, UPDATE_REG_R11_PACKET, (void *)thing, strlen(thing));  

  sprintf(thing, "%04X", (uint16_t)cpu->r12);
  send_control(emu, UPDATE_REG_R12_PACKET, (void *)thing, strlen(thing));  

  sprintf(thing, "%04X", (uint16_t)cpu->r13);
  send_control(emu, UPDATE_REG_R13_PACKET, (void *)thing, strlen(thing));  

  sprintf(thing, "%04X", (uint16_t)cpu->r14);
  send_control(emu, UPDATE_REG_R14_PACKET, (void *)thing, strlen(thing));  

  sprintf(thing, "%04X", (uint16_t)cpu->r15);
  send_control(emu, UPDATE_REG_R15_PACKET, (void *)thing, strlen(thing));
}

//##########+++ Set SR struct Value +++##########
void set_sr_value (Emulator *emu, uint16_t value) 
{
  Cpu *cpu = emu->cpu;  

  // reset SR to set it properly...
  memset(&cpu->sr, 0, sizeof(Status_reg));
  //memcpy(&cpu->sr, &value, 16);

  if (value & 0x8000) cpu->sr.reserved |= 0x8000;
  if (value & 0x4000) cpu->sr.reserved |= 0x4000;
  if (value & 0x2000) cpu->sr.reserved |= 0x2000;
  if (value & 0x1000) cpu->sr.reserved |= 0x1000;
  if (value & 0x0800) cpu->sr.reserved |= 0x0800;
  if (value & 0x0400) cpu->sr.reserved |= 0x0400;
  if (value & 0x0200) cpu->sr.reserved |= 0x0200;

  cpu->sr.overflow = (value & 0x0100) ? 1 : 0;
  cpu->sr.SCG1 =     (value & 0x0080) ? 1 : 0;
  cpu->sr.SCG0 =     (value & 0x0040) ? 1 : 0;
  cpu->sr.OSCOFF =   (value & 0x0020) ? 1 : 0;
  cpu->sr.CPUOFF =   (value & 0x0010) ? 1 : 0;
  cpu->sr.GIE =      (value & 0x0008) ? 1 : 0;
  cpu->sr.negative = (value & 0x0004) ? 1 : 0;
  cpu->sr.zero =     (value & 0x0002) ? 1 : 0;
  cpu->sr.carry =    (value & 0x0001) ? 1 : 0;
}

//##########+++ Return value from SR struct +++##########
uint16_t sr_to_value(Emulator *emu)
{ 
  Cpu *cpu = emu->cpu;
  uint16_t r2 = 0;

  // reserved bits not working quite right yet
  if (cpu->sr.reserved & 0b1000000) {
    r2 |= 0x8000;
  }
  if (cpu->sr.reserved & 0b0100000) {
    r2 |= 0x4000;
  }
  if (cpu->sr.reserved & 0b0010000) {
    r2 |= 0x2000;
  }
  if (cpu->sr.reserved & 0b0001000) {
    r2 |= 0x1000;
  }
  if (cpu->sr.reserved & 0b0000100) {
    r2 |= 0x0800;
  }
  if (cpu->sr.reserved & 0b0000010) {
    r2 |= 0x0400;
  }
  if (cpu->sr.reserved & 0b0000001) {
    r2 |= 0x0200;
  }

  if (cpu->sr.overflow) {
    r2 |= 0x0100; 
  }
  
  if (cpu->sr.SCG1) {
    r2 |= 0x0080;
  }
  
  if (cpu->sr.SCG0) {
    r2 |= 0x0040;
  }

  if (cpu->sr.OSCOFF) {
    r2 |= 0x0020;
  }

  if (cpu->sr.CPUOFF) {
    r2 |= 0x0010;
  }

  if (cpu->sr.GIE) {
    r2 |= 0x0008;
  }

  if (cpu->sr.negative) {
    r2 |= 0x0004;
  }
  
  if (cpu->sr.zero) {
    r2 |= 0x0002;
  }
  
  if (cpu->sr.carry) {
    r2 |= 0x0001;
  }
  
  return r2;
}
