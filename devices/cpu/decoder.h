/*                                                                             
  This file is part of MSP430 Emulator                                         
                                                                               
  Foobar is free software: you can redistribute it and/or modify               
  it under the terms of the GNU General Public License as published by         
  the Free Software Foundation, either version 3 of the License, or            
  (at your option) any later version.                                          
                                                                               
  Foobar is distributed in the hope that it will be useful,                    
  but WITHOUT ANY WARRANTY; without even the implied warranty of               
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                
  GNU General Public License for more details.                                 
                                                                               
  You should have received a copy of the GNU General Public License            
  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.             
*/

void decode_formatI(uint16_t instruction);
void decode_formatII(uint16_t instruction);
void deocde_formatIII(uint16_t instruction);
void decode(uint16_t instruction);
uint16_t fetch();

enum { WORD, BYTE };

#include "formatI.c"
#include "formatII.c"
#include "formatIII.c"
#include "decoder.c"
