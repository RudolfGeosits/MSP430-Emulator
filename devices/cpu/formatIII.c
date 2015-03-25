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
  int16_t signed_offset = (instruction & 0x03FF) * 2;
  bool negative = signed_offset >> 9;

  if (negative) { /* Sign Extend for Arithmetic Operations */
    signed_offset |= 0xF800;
  }

  switch(condition){
  
  /* JNE/JNZ Jump if not equal/zero             
  *
  * If Z = 0: PC + 2 offset → PC
  * If Z = 1: execute following instruction
  */
  case 0x0:{
    printf("JNZ 0x%04X\n", PC + signed_offset);

    if (SR.zero == false) {
      PC += signed_offset;
    }

    break;
  }
  
  /* JEQ/JZ Jump is equal/zero
   * If Z = 1: PC + 2 offset → PC
   * If Z = 0: execute following instruction
  */
  case 0x1:{
    printf("JZ 0x%04X\n", PC + signed_offset);

    if (SR.zero == true) {
      PC += signed_offset;
    }

    break;
  }
  
  /* JNC/JLO Jump if no carry/lower
  *
  *  if C = 0: PC + 2 offset → PC
  *  if C = 1: execute following instruction
  */
  case 0x2:{
    printf("JNC 0x%04X\n", PC + signed_offset);

    if (SR.carry == false) {
      PC += signed_offset;
    }    
    
    break;
  }

  /* JC/JHS Jump if carry/higher or same
  *
  * If C = 1: PC + 2 offset → PC
  * If C = 0: execute following instruction
  */
  case 0x3:{
    printf("JC 0x%04X\n", PC + signed_offset);
    
    if (SR.carry == true) {
      PC += signed_offset;
    }    

    break;
  }
  
  /* JN Jump if negative
  *
  *  if N = 1: PC + 2 ×offset → PC
  *  if N = 0: execute following instruction
  */
  case 0x4:{
    printf("JN 0x%04X\n", PC + signed_offset);

    if (SR.negative == true) {
      PC += signed_offset;
    }    

    break;
  }
   
  /* JGE Jump if greater or equal (N == V)
  *
  *  If (N .XOR. V) = 0 then jump to label: PC + 2 P offset → PC
  *  If (N .XOR. V) = 1 then execute the following instruction
  */
  case 0x5:{
    printf("JGE 0x%04X\n", PC + signed_offset);

    if ((SR.negative ^ SR.overflow) == false) {
      PC += signed_offset;
    }    
    
    break;
  }
    
  /* JL Jump if less (N != V)  
  *
  *  If (N .XOR. V) = 1 then jump to label: PC + 2 offset → PC
  *  If (N .XOR. V) = 0 then execute following instruction
  */
  case 0x6:{
    printf("JL 0x%04X\n", PC + signed_offset);

    if ((SR.negative ^ SR.overflow) == true) {
      PC += signed_offset;
    }    
    
    break;
  }
 
  /* JMP Jump Unconditionally
   *   
   *  PC + 2 × offset → PC
   *
   */
  case 0x7:{
    printf("JMP 0x%04X\n", PC + signed_offset);

    PC += signed_offset;
    break;
  }

  default:{
    printf("Undefined Jump operation\n");
    return;
  }
  
  } //# End of Switch
  
}
