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
void decode_formatIII( unsigned short instruction )
{
  unsigned char opcode = ( instruction & 0xF000 ) >> 12;
  unsigned char source = ( instruction & 0x0F00 ) >> 8;
  unsigned char as_flag = (instruction & 0x0030 ) >> 4;
  unsigned char destination = ( instruction & 0x000F );
  unsigned char ad_flag = (instruction & 0x0080 ) >> 7;
  unsigned char bw_flag = (instruction & 0x0040 ) >> 6;

  char source_reg[10], dst_reg[10];

  printf("Source: %04X\nas_flag: %02X\nDest: %04X\nad_flag: %X\nbw_flag: %X\n",
  source, as_flag, destination, ad_flag, bw_flag);

  switch( opcode ){
  
  //# MOV source to destination                                                              
  case 0x4:{
    bw_flag == 0 ? printf("MOV ") : printf("MOV.B ");

    //# SRC = reg contents; DST = reg contents
    if( as_flag == 0 && ad_flag == 0 ){
      reg_num_to_name(source, source_reg);
      reg_num_to_name(destination, dst_reg);
      printf("%s, %s\n", source_reg, dst_reg);

      short *source_reg = get_reg_ptr(source);
      short *dst_reg = get_reg_ptr(destination);

      *dst_reg = *source_reg;

      break;
    }

    //# SRC = absolute addr; DST = absolute addr
    else if( as_flag == 1 && ad_flag == 1 ){
      unsigned short source_offset, dst_offset;
      reg_num_to_name(source, source_reg);
      reg_num_to_name(destination, dst_reg);
      
      source_offset = fetch();
      dst_offset = fetch();
      
      if(source == 2){
	printf("&0x%04X, &0x%04X\n", source_offset, dst_offset);      
      }
      else{
	printf("0x%04X(%s), 0x%04X(%s)\n", source_offset, source_reg, dst_offset, dst_reg);
      }

      break;
    }

    //# SRC = absolute addr; DST = register contents
    else if( as_flag == 1 && ad_flag == 0 ){
      unsigned short source_offset, source_contents;
      unsigned short* source_ptr;
      short* dst_ptr;

      reg_num_to_name(source, source_reg);
      reg_num_to_name(destination, dst_reg);
      
      //# Here, source reg is acting as a constant generator
      source_offset = fetch();

      if(source == 0x2)
	printf("&0x%04X, %s\n", source_offset, dst_reg);
      else
	printf("0x%04X(%s), %s\n", source_offset, source_reg, dst_reg);

      source_ptr = get_reg_ptr(source);
      dst_ptr = get_reg_ptr(destination);
      source_contents = *source_ptr;

      void* addr = (void*)MEMSPACE;
      addr += (source_contents + source_offset);

      *dst_ptr = *( (short*)addr );

      break;
    }

    //# SRC = reg contents, DST = absolute address
    //# Check for constant generator
    else if( as_flag == 0 && ad_flag == 1 ){      
      short dst_offset;
      reg_num_to_name(source, source_reg);
      reg_num_to_name(destination, dst_reg);
      
      dst_offset = fetch();
 
      if(destination == 0x2){
	printf("%s, &0x%04X\n", source_reg, dst_offset);
	short* dst_addr = (short*)((void*)MEMSPACE + dst_offset);
        short* src_reg = get_reg_ptr(source);
	
	*dst_addr = *src_reg;
      }
      else{
	r5 = 0x4567;
	printf("%s, 0x%04X(%s)\n", source_reg, dst_offset, dst_reg);
	short *src_reg = get_reg_ptr(source);
	short *dst_reg = get_reg_ptr(destination);

	short dst_reg_val = *dst_reg;
	void* ptr = (void*)(uintptr_t)(dst_reg_val + dst_offset + MEMSPACE);
	printf("0x%04X\nptr = %p\n", dst_reg_val + dst_offset, ptr);
	short* real_ptr = (short*)ptr;

	*real_ptr = *src_reg;
      }

      break;
    }

    //# SRC = Constant; DST = register contents
    else if( as_flag == 3 && ad_flag == 0 ){
      short source_const;
      reg_num_to_name(destination, dst_reg);
      
      source_const = fetch();
      printf("#0x%04X, %s\n", source_const, dst_reg);

      break;
    }

    //# SRC = Constant, DST = Absolute Addressing mode
    else if( as_flag == 3 && ad_flag == 1 ){
      short source_const;
      unsigned short dst_addr;
      
      source_const = fetch();
      dst_addr = fetch();
      printf("#0x%04X, &0x%04X\n", source_const, dst_addr);

    }

    //# SRC = Indirect Register; DST = Register Contents
    else if( as_flag == 2 && ad_flag == 0 ){
      short source_const;
      reg_num_to_name(destination, dst_reg);
      reg_num_to_name(source, source_reg);

      printf("@%s, %s\n", source_reg, dst_reg);

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
