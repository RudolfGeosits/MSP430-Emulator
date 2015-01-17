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

//##########+++ Decode Format III Instructions +++#########
//# Format III are jump instructions of the form:
//#   [001C][CCXX][XXXX][XXXX]
//# 
//# Where C = Condition, X = 10-bit signed offset 
//# 
//########################################################
void decode_formatIII( uint16_t instruction )
{
  uint8_t condition = (instruction & 0x1C00) >> 10;
  int16_t signed_offset = (instruction & 0x03FF);

  switch(condition){
  
  //# JNE/JNZ Jump if not equal/zero             
  case 0x0:{
    printf("JNZ 0x%04X\n", PC + (signed_offset*2));
    break;
  }
  
  //# JEQ/JZ Jump is equal/zero
  case 0x1:{
    printf("JZ 0x%04X\n", PC + (signed_offset*2));
    break;
  }
  
  //# JNC/JLO Jump if no carry/lower
  case 0x2:{
    printf("JNC 0x%04X\n", PC + (signed_offset*2));
    break;
  }

  //# JC/JHS Jump if carry/higher or same
  case 0x3:{
    printf("JC 0x%04X\n", PC + (signed_offset*2));
    break;
  }
  
  //# JN Jump if negative
  case 0x4:{
    printf("JN 0x%04X\n", PC + (signed_offset*2));
    break;
  }
   
  //# JGE Jump if greater or equal (N == V)
  case 0x5:{
    printf("JGE 0x%04X\n", PC + (signed_offset*2));
    break;
  }
    
  //# JL Jump if less (N != V)  
  case 0x6:{
    printf("JL 0x%04X\n", PC + (signed_offset*2));
    break;
  }
 
  //# JMP Jump Unconditionally
  case 0x7:{
    printf("JMP 0x%04X\n", PC + (signed_offset*2));
    break;
  }

  default:{
    printf("Undefined Jump operation\n");
    return;
  }
  
  } //# End of Switch
  
}
