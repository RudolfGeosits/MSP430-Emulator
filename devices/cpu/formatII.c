#include "decoder.h"

//##########+++ Decode Format II Instructions +++##########
//# Format II are jump instructions of the form:
//#   [001C][CCXX][XXXX][XXXX]
//# 
//# Where C = Condition, X = 10-bit signed offset 
//# 
//########################################################
void decode_formatII( unsigned short instruction )
{
  unsigned char condition = ( instruction & 0x1C00 ) >> 10;
  short signed_offset = ( instruction & 0x03FF );

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
  }
  
  } //# End of Switch
  
}
