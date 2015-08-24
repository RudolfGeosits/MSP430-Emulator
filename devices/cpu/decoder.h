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

#ifndef _DECODER_H_
#define _DECODER_H_

int16_t run_constant_generator(uint8_t source, uint8_t as_flag);

uint8_t is_overflowed (uint16_t source, uint16_t original_destination,
                       uint16_t *result_addr, uint8_t bw_flag);

uint8_t is_negative (int16_t *result_addr, uint8_t bw_flag);

uint8_t is_zero (uint16_t *result_addr, uint8_t bw_flag);

uint8_t is_carried (uint32_t original_dst_value, uint32_t source_value,
                    uint8_t bw_flag);

char mnemonic[50] = {0};
bool disassemble_mode = false;
bool debug_mode = true;

void decode_formatI(Cpu *cpu, uint16_t instruction, bool disassemble);
void decode_formatII(Cpu *cpu, uint16_t instruction, bool disassemble);
void deocde_formatIII(Cpu *cpu, uint16_t instruction, bool disassemble);

void decode(Cpu *cpu, uint16_t instruction, bool disassemble);
uint16_t fetch(Cpu *cpu);

enum { 
  WORD, 
  BYTE 
};

enum {
  EXECUTE = 0,
  DISASSEMBLE,
};

#include "flag_handler.c"
#include "formatI.c"
#include "formatII.c"
#include "formatIII.c"
#include "decoder.c"

#endif
