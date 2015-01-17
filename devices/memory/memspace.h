/*                                                                             
  This file is part of MSP430 Emulator                                         
                                                                               
  MSP430 Emulator is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by         
  the Free Software Foundation, either version 3 of the License, or            
  (at your option) any later version.                                          
                                                                               
  MSP430 Emulator is distributed in the hope that it will be useful,   
  but WITHOUT ANY WARRANTY; without even the implied warranty of               
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                
  GNU General Public License for more details.                                 
                                                                               
  You should have received a copy of the GNU General Public License            
  along with MSP430 Emulator.  If not, see <http://www.gnu.org/licenses/>.
*/

void initialize_msp_memspace();
void uninitialize_msp_memspace();

uint8_t *MEMSPACE;   /* Memory Space */
uint8_t *IVT;        /* Interrupt Vector Table {Within ROM} */
uint8_t *ROM;        /* Flash/Read-Only memory */
uint8_t *RAM;        /* Random Access Memory */
uint8_t *PER16;      /* 16-bit peripherals */
uint8_t *PER8;       /* 8-bit peripherals */
uint8_t *SFRS;       /* Special Function Registers */
uint8_t *LOAD_POS;

#include "memspace.c"
