#include "decoder.h"
#include "formatI.c"
#include "formatII.c"
#include "formatIII.c"

//##########+++ CPU Fetch Cycle +++##########
unsigned short fetch()
{
  unsigned short word;
  word = *( (unsigned short*)(MEMSPACE + PC) );
  PC += 2;

  return word;
}

//##########+++ CPU Decode Cycle +++##########
void decode(unsigned short instruction)
{  
  int done = 0;
  unsigned char format_id;

  format_id = (unsigned char)(instruction >> 12);
  printf("Got instruction 0x%04X\n", instruction);

  if( format_id == 0x1 ){

    //# Decide firnat I (single operand) instruction
    decode_formatI(instruction);

  }    
  else if(format_id >= 0x2 && format_id <= 3){

    //# Decode format II (jump) instruction
    decode_formatII(instruction);

  }
  else if(format_id >= 0x4){

    //# Decode format III (two operand) instruction
    decode_formatIII(instruction);

  }
}
