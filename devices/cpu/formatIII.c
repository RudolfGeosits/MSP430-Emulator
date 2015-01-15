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

//##########+++ Decode Format III Instructions +++##########
//# Format III are double operand of the form:
//#   [CCCC][SSSS][ABaa][DDDD]
//# 
//# Where C = Opcode, B = Byte/Word flag,
//#       A = Addressing mode for destination
//#       a = Addressing mode for source_reg
//#       S = Source_Reg, D = Destination
//########################################################
void decode_formatIII( uint16_t instruction )
{
  uint8_t opcode = ( instruction & 0xF000 ) >> 12;
  uint8_t source_reg = ( instruction & 0x0F00 ) >> 8;
  uint8_t as_flag = (instruction & 0x0030 ) >> 4;
  uint8_t destination = ( instruction & 0x000F );
  uint8_t ad_flag = (instruction & 0x0080 ) >> 7;
  uint8_t bw_flag = (instruction & 0x0040 ) >> 6;

  char source_reg_reg[10], dst_reg[10];

  printf("Source_Reg: %04X\nas_flag: %02X\nDest: %04X\nad_flag: %X\nbw_flag: %X\n",
  source_reg, as_flag, destination, ad_flag, bw_flag);

  /* Spot CG1 and CG2 Constant generator instructions */
  if (source_reg == 2 && as_flag > 0) {
    printf("CG1 using %%r2\n");
  }
  else if (source_reg == 3) {
    printf("CG2 using %%r3\n");
  }

  switch (opcode) {
  
    //# MOV source_reg to destination
  case 0x4:{
    bw_flag == 0 ? printf("MOV ") : printf("MOV.B ");

    //# SRC = reg contents; DST = reg contents
    if (as_flag == 0 && ad_flag == 0) {
      reg_num_to_name(source_reg, source_reg_reg);
      reg_num_to_name(destination, dst_reg);
      printf("%s, %s\n", source_reg_reg, dst_reg);

      int16_t *source_reg_reg = get_reg_ptr(source_reg);
      int16_t *dst_reg = get_reg_ptr(destination);

      *dst_reg = *source_reg_reg;

      break;
    }

    //# SRC = absolute addr; DST = absolute addr
    else if (as_flag == 1 && ad_flag == 1) {
      uint16_t source_reg_offset, dst_offset;
      reg_num_to_name(source_reg, source_reg_reg);
      reg_num_to_name(destination, dst_reg);
      
      source_reg_offset = fetch();
      dst_offset = fetch();
      
      if(source_reg == 2){
	printf("&0x%04X, &0x%04X\n", source_reg_offset, dst_offset);      
      }
      else{
	printf("0x%04X(%s), 0x%04X(%s)\n", source_reg_offset, source_reg_reg, 
	       dst_offset, dst_reg);
      }

      break;
    }

    //# SRC = absolute addr; DST = register contents
    else if (as_flag == 1 && ad_flag == 0) {
      uint16_t source_reg_offset, source_reg_contents;
      uint16_t* source_reg_ptr;
      int16_t* dst_ptr;

      reg_num_to_name(source_reg, source_reg_reg);
      reg_num_to_name(destination, dst_reg);
      
      //# Here, source_reg reg is acting as a constant generator
      source_reg_offset = fetch();

      if(source_reg == 0x2)
	printf("&0x%04X, %s\n", source_reg_offset, dst_reg);
      else
	printf("0x%04X(%s), %s\n", source_reg_offset, source_reg_reg, dst_reg);

      source_reg_ptr = get_reg_ptr(source_reg);
      dst_ptr = get_reg_ptr(destination);
      source_reg_contents = *source_reg_ptr;

      void* addr = (void*)MEMSPACE;
      addr += (source_reg_contents + source_reg_offset);

      *dst_ptr = *( (int16_t*)addr );

      break;
    }

    //# SRC = reg contents, DST = absolute address
    //# Check for constant generator
    else if (as_flag == 0 && ad_flag == 1) {      
      int16_t dst_offset;
      reg_num_to_name(source_reg, source_reg_reg);
      reg_num_to_name(destination, dst_reg);
      
      dst_offset = fetch();
 
      if (destination == 0x2) {
	printf("%s, &0x%04X\n", source_reg_reg, dst_offset);
      }
      
      break;
    }

    //# SRC = Constant; DST = register contents
    else if (as_flag == 3 && ad_flag == 0) {
      int16_t source_reg_const;
      reg_num_to_name(destination, dst_reg);
      
      source_reg_const = fetch();
      printf("#0x%04X, %s\n", source_reg_const, dst_reg);

      break;
    }

    //# SRC = Constant, DST = Absolute Addressing mode
    else if (as_flag == 3 && ad_flag == 1) {
      int16_t source_reg_const;
      uint16_t dst_addr;
      
      source_reg_const = fetch();
      dst_addr = fetch();
      printf("#0x%04X, &0x%04X\n", source_reg_const, dst_addr);

    }

    //# SRC = Indirect Register; DST = Register Contents
    else if (as_flag == 2 && ad_flag == 0) {
      int16_t source_reg_const;
      reg_num_to_name(destination, dst_reg);
      reg_num_to_name(source_reg, source_reg_reg);

      printf("@%s, %s\n", source_reg_reg, dst_reg);

      break; 
    }

    break;
  }

  //# ADD source_reg to destination               
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
