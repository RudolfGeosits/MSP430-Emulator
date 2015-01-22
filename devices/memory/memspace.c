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

/* Initialize Address Space Locations
** 
** Allocate and set MSP430 Memory space
** Some of these locations vary by model
*/
void initialize_msp_memspace()
{
  /* 128 KB Address Space */
  MEMSPACE = (uint8_t *) malloc(0x1FFFF);
  
  IVT = MEMSPACE +   0xFFE0;   /* 0xFFE0 - 0x1FFFF */
  ROM = MEMSPACE +   0x0400;   /* 0x400 - 0xFFDF {Varies with device} */
  RAM = MEMSPACE +   0x0200;   /* 0x200 - 0x3FF {Varies with device} */
  PER16 = MEMSPACE + 0x0100;   /* 0x0100 - 0x01FF */
  PER8 = MEMSPACE +  0x0010;   /* 0x0010 - 0x00FF */
  SFRS = MEMSPACE +  0x0;      /* 0x0 - 0x0F */

  LOAD_POS = MEMSPACE + 0xC000;
}


/* 
** Free MSP430 virtual memory
*/
void uninitialize_msp_memspace()
{
  free(MEMSPACE);
}
