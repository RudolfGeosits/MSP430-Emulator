#include "decoder.h"

//##########+++ Decode Format III Instructions +++##########
//# Format III are double operand of the form:
//#   [CCCC][SSSS][ABaa][DDDD]
//# 
//# Where C = Opcode, B = Byte/Word flag,
//#       A = Addressing mode for destination
//#       a = Addressing mode for source
//#       S = Source, D = Destination
//########################################################
void decode_formatIII( unsigned short instruction ){

  unsigned char opcode = ( instruction & 0xF000 ) >> 12;
  unsigned char source = ( instruction & 0x0F00 ) >> 8;
  unsigned char as_flag = (instruction & 0x0030 ) >> 4;
  unsigned char destination = ( instruction & 0x000F );
  unsigned char ad_flag = (instruction & 0x0080 ) >> 7;
  unsigned char bw_flag = (instruction & 0x0040 ) >> 6;

  char source_reg[10], dest_reg[10];

  printf("Source: %04X\nas_flag: %02X\nDest: %04X\nad_flag: %X\nbw_flag: %X\n",
  source, as_flag, destination, ad_flag, bw_flag);

  switch( opcode ){
  
  //# MOV source to destination                                                              
  case 0x4:{
    bw_flag == 0 ? printf("MOV ") : printf("MOV.B ");

    //# Both operands are register contents
    if( as_flag == 0 && ad_flag == 0 ){
      reg_num_to_name(source, source_reg);
      reg_num_to_name(destination, dest_reg);
      printf("%s, %s\n", source_reg, dest_reg);

      short *s = get_reg_ptr(source);
      short *d = get_reg_ptr(destination);

      *d = *s;

      break;
    }

    //# Indexed mode X(Rn) (Rn+X) points to the operand
    //# X is stored in the next word
    else if( as_flag == 1 && ad_flag == 1 ){
      unsigned short source_offset, dest_offset;
      reg_num_to_name(source, source_reg);
      reg_num_to_name(destination, dest_reg);
      
      source_offset = fetch();
      dest_offset = fetch();
      printf("0x%04X(%s), 0x%04X(%s)\n", source_offset, source_reg, dest_offset, dest_reg);

      break;
    }

    //# Indexed mode for only source operand
    else if( as_flag == 1 && ad_flag == 0 ){
      
      unsigned short source_offset;
      reg_num_to_name(source, source_reg);
      reg_num_to_name(destination, dest_reg);
      
      source_offset = fetch();
      printf("0x%04X(%s), %s\n", source_offset, source_reg, dest_reg);

      break;
    }

    //# Indexed mode for only destination operand
    else if( as_flag == 0 && ad_flag == 1 ){
      
      unsigned short dest_offset;
      reg_num_to_name(source, source_reg);
      reg_num_to_name(destination, dest_reg);
      
      dest_offset = fetch();
      printf("%s, 0x%04X(%s)\n", source_reg, dest_offset, dest_reg);

      break;
    }

    //# Constant mode for only source operand
    else if( as_flag == 3 && ad_flag == 0 ){
      short source_const;
      reg_num_to_name(destination, dest_reg);
      
      source_const = fetch();
      printf("0x%04X, %s\n", source_const, dest_reg);

      break;
    }

    //# SRC = Constant, DST = Address
    else if( as_flag == 3 && ad_flag == 1 ){
      short source_const;
      unsigned short dest_addr;
      
      source_const = fetch();
      dest_addr = fetch();
      printf("#0x%04X, &0x%04X\n", source_const, dest_addr);

    }

    //# Indirect register mode for only source operand
    else if( as_flag == 2 && ad_flag == 0 ){
      short source_const;
      reg_num_to_name(destination, dest_reg);
      reg_num_to_name(source, source_reg);

      printf("@%s, %s\n", source_reg, dest_reg);

      break;
    }

    break;
  }

  //# ADD source to destination                                                              
  case 0x5:{
    printf("ADD\n");
    break;
  }

  //# ADDC Add w/ carry dst += (src+C)                                                        
  case 0x6:{
    printf("ADDC\n");
    break;
  }

  //# SUBC Sub w/carry dst -= (src+C)                                                        
  case 0x7:{
    printf("SUBC\n");
    break;
  }

  //# SUB dst -= src                                                                         
  case 0x8:{
    printf("SUB\n");
    break;
  }

  //# CMP compare (dst-src) discard result                                                   
  case 0x9:{
    printf("CMP\n");
    break;
  }
    
  //# DADD Decimal addition dst += src                                                       
  case 0xA:{
    printf("DADD\n");
    break;
  }

  //# BIT Test bits dst & src, discard result                                                
  case 0xB:{
    printf("BIT\n");
    break;
  }

  //# BIC, bit clear dest &= ~src                                                            
  case 0xC:{
    printf("BIC\n");
    break;
  }

  //# BIS Bit set - logical OR dest |= src                                                   
  case 0xD:{
    printf("BIS\n");
    break;
  }

  //# XOR, bitwise XOR; dst ^= src                                                           
  case 0xE:{
    printf("XOR\n");
    break;
  }

  //# AND, bitwise AND; dst &= src                                                           
  case 0xF:{
    printf("AND\n");
    break;
  }

  } //# End of switch  
  
}
