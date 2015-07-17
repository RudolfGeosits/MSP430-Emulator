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

//##########+++ MSP430 Register initialization +++##########
void initialize_msp_registers(Cpu *cpu)
{
  /* Initialize PC to boot loader code on cold boot (COLD)*/
  //cpu->pc = 0x0C00;

  /* Initialize Program Counter to *0xFFFE at boot or reset (WARM)*/
  cpu->pc = 0xC000;
  
  /* Stack pointer typically begins at the top of RAM after reset */
  cpu->sp = 0x400;

  /* Initialize the status register */
  memset(&cpu->sr, 0, sizeof(Status_reg));

  cpu->cg2 = 0;

  cpu->r4 = cpu->r5 = cpu->r6 = cpu->r7 = cpu->r8 = 
    cpu->r9 = cpu->r10 = cpu->r11 = cpu->r12 = cpu->r13 = 
    cpu->r14 = cpu->r15 = 0;
}

//##########+++ Return value from SR struct +++##########
uint16_t sr_to_value(Cpu *cpu)
{ 
  uint16_t r2 = 0;
  
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
