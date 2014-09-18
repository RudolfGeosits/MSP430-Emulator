#include "decoder.h"

//##########+++ Decode Format I Instructions +++##########
//# Format I are single operand of the form:
//#   [0001][00CC][CBAA][SSSS]
//# 
//# Where C = Opcode, B = Byte/Word flag, 
//#       A = Addressing mode for source
//#       S = Source 
//########################################################
void decode_formatI( unsigned short instruction ){

  char reg_name[10];
  unsigned char opcode = ( instruction & 0x0380 ) >> 7;
  unsigned char bw_flag = ( instruction & 0x0040 ) >> 6;
  unsigned char as_flag = ( instruction & 0x0030 ) >> 4;
  unsigned char source_reg = ( instruction & 0x000F );
  reg_num_to_name(source_reg, reg_name);

  switch(opcode){
    
  //# RRC Rotate right through carry      
  case 0x0:{

    bw_flag == 0 ? printf("RRC ") : printf("RRC.B ");

    //# direct register
    if(as_flag == 0x0){
      printf("%s\n", reg_name);
    }

    //# Indexed register
    else if(as_flag == 0x1){
      short source_offset;

      source_offset = fetch();
      printf("0x%04X(%s)\n", source_offset, reg_name);
    }

    //# indirect register
    else if(as_flag == 0x2){
      printf("@%s\n", reg_name);
    }

    //# Indirect autoincrement 
    else if(as_flag == 0x3){
      printf("@%s+\n", reg_name);
    }

    break;
  }
  
  //# SWPB Swap bytes
  //# bw flag always 0 (word)
  case 0x1:{

    //# Direct register
    if( as_flag == 0x0 ){
      printf("SWPB %s\n", reg_name);
    }

    //# Indexed register
    else if( as_flag == 0x1 ){
      short source_offset;

      source_offset = fetch();
      printf("SWPB 0x%04X(%s)\n", source_offset, reg_name);
    }

    //# indirect register
    else if( as_flag == 0x2 ){
      printf("SWPB @%s\n", reg_name);
    }

    //# Indirect register autoincrement
    else if( as_flag == 0x3 ){
      printf("SWPB @%s+\n", reg_name);
    }

    break;
  }
  
  //# RRA Rotate right arithmetic 
  case 0x2:{

    bw_flag == 0 ? printf("RRA ") : printf("RRA.B ");

    if(as_flag == 0x0){
      printf("%s\n", reg_name);
    }
    else if(as_flag == 0x1){
      short source_offset;

      source_offset = fetch();
      printf("0x%04X(%s)\n", source_offset, reg_name);
    }
    else if(as_flag == 0x2){
      printf("@%s\n", reg_name);
    }
    else if(as_flag == 0x3){
      printf("@%s+\n", reg_name);
    }

    break;
  }

  //# SXT Sign extend byte to word
  //# bw flag always 0 (word)
  case 0x3:{

    if(as_flag == 0x0){
      printf("SXT %s\n", reg_name);
    }
    else if(as_flag == 0x1){
      short source_offset;
      
      source_offset = fetch();
      printf("SXT 0x%04X(%s)\n", source_offset, reg_name);
    }
    else if(as_flag == 0x2){
      printf("SXT @%s\n", reg_name);
    }
    else if(as_flag == 0x3){
      printf("SXT @%s+\n", reg_name);
    }

    break;
  }
  
  //# PUSH push value on to the stack
  case 0x4:{

    bw_flag == 0 ? printf("PUSH ") : printf("PUSH.B ");    

    if(as_flag == 0x0){
      printf("%s\n", reg_name);
    }
    else if(as_flag == 0x1){
      short source_offset;

      source_offset = fetch();
      printf("0x%04X(%s)\n", source_offset, reg_name);
    }
    else if(as_flag == 0x2){
      printf("@%s\n", reg_name);
    }
    else if(as_flag == 0x3){
      printf("@%s+\n", reg_name);
    }

    break;
  }

  //# Call subroutine; push PC and move source to PC
  case 0x5:{

    printf("CALL ");

    if(as_flag == 0x0){
      printf("%s\n", reg_name);
    }
    else if(as_flag == 0x1){
      short source_offset;

      source_offset = fetch();
      printf("0x%04X(%s)\n", source_offset, reg_name);
    }
    else if(as_flag == 0x2){
      printf("@%s\n", reg_name);
    }
    else if(as_flag == 0x3){
      printf("@%s+\n", reg_name);
    }

    break;
  }
  
  //# RETI Return from interrupt: Pop SR then pop PC
  case 0x6:{
    printf("RETI\n");
    break;
  }
  default:{
    printf("Unknown Single operand instruction.\n");
  }

  } //# End of Switch
  
}

