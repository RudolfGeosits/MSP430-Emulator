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

//##########+++ CPU Fetch Cycle +++##########
uint16_t fetch()
{
  uint16_t word;
  word = *( (uint16_t*)(MEMSPACE + PC) );
  PC += 2;

  return word;
}

//##########+++ CPU Decode Cycle +++##########
void decode(uint16_t instruction)
{  
  int done = 0;
  uint8_t format_id;

  format_id = (uint8_t)(instruction >> 12);
  printf("Got instruction 0x%04X\n", instruction);

  if (format_id == 0x1) {
    decode_formatI(instruction);    /* format I (single operand) instruction */
  }    
  else if (format_id >= 0x2 && format_id <= 3) {
    decode_formatII(instruction);   /* format II (jump) instruction */
  }
  else if (format_id >= 0x4) {
    decode_formatIII(instruction);  /* format III (two operand) instruction */
  }
}
