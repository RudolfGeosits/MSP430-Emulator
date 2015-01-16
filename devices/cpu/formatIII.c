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
//# [CCCC][SSSS][ABaa][DDDD]
//#
//# Where C = Opcode, B = Byte/Word flag,
//# A = Addressing mode for destination
//# a = Addressing mode for s_reg_name
//# S = S_Reg_Name, D = Destination
//########################################################

void decode_formatIII( uint16_t instruction )
{
  uint8_t opcode = (instruction & 0xF000) >> 12;
  uint8_t source = (instruction & 0x0F00) >> 8;
  uint8_t as_flag = (instruction & 0x0030) >> 4;
  uint8_t destination = (instruction & 0x000F);
  uint8_t ad_flag = (instruction & 0x0080) >> 7;
  uint8_t bw_flag = (instruction & 0x0040) >> 6;

  char s_reg_name[10], d_reg_name[10];

  int16_t *s_reg = get_reg_ptr(source);      /* Source Register pointer */
  int16_t *d_reg = get_reg_ptr(destination); /* Destination Register pointer */

  reg_num_to_name(source, s_reg_name);      /* Get source register name */
  reg_num_to_name(destination, d_reg_name); /* Get destination register name */

  printf("Opcode: 0x%01X  Source bits: 0x%01X  Destination bits: 0x%01X\n" \
	 "AS_Flag: 0x%01X  AD_Flag: 0x%01X  BW_Flag: 0x%01X\n",
	 opcode, source, destination, as_flag, ad_flag, bw_flag);

  /* Spot CG1 and CG2 Constant generator instructions */
  if (source == 2 && as_flag > 0) {
    printf("CG1 using %%r2 - Skipping for now\n");
    return;
  }
  else if (source == 3) {
    printf("CG2 using %%r3 - Skipping for now\n");
    return;
  }

  switch (opcode) {

  case 0x4:{   /* MOV SOURCE, DEST */
    bw_flag == 0 ? printf("MOV ") : printf("MOV.B ");

    if (as_flag == 0 && ad_flag == 0) {   /* MOV Rs, Rd */
	      
      printf("%s, %s\n", s_reg_name, d_reg_name);
      
      if (bw_flag == WORD) {		
	*d_reg = *s_reg;
      }
      else if (bw_flag == BYTE) {
	*d_reg = *s_reg;
	*d_reg &= 0x00FF;
      }

      break;
    }

    else if (as_flag == 0 && ad_flag == 1) {   /* MOV Rs, 0x0(Rd) */
      int16_t destination_offset = fetch();

      printf("%s, 0x%04X(%s)\n", s_reg_name, (uint16_t)destination_offset, 
	     d_reg_name);

      uint16_t *destination_addr = get_addr_ptr(*d_reg + destination_offset);

      if (bw_flag == WORD) {
	*destination_addr = *s_reg;
      }
      else if (bw_flag == BYTE) {
	*( (uint8_t *)destination_addr ) = *( (uint8_t *)s_reg );
      }
	
      break;
    }

    else if (as_flag == 1 && ad_flag == 0) {   /* MOV 0x0(Rs), Rd */
      int16_t source_offset = fetch();

      printf("0x%04X(%s), %s\n", (uint16_t)source_offset, s_reg_name, 
	     d_reg_name);
  
      uint16_t *source_addr = get_addr_ptr(*s_reg + source_offset);
    
      if (bw_flag == WORD) {
	*d_reg = *source_addr;
      }
      else if (bw_flag == BYTE) {
	*( (uint8_t *)d_reg ) = *( (uint8_t *)source_addr );	
	*d_reg &= 0x00FF;
      }

      break;
    }

    else if (as_flag == 1 && ad_flag == 1) {   /* MOV 0x0(Rs), 0x0(Rd) */
      int16_t source_offset = fetch();
      int16_t destination_offset = fetch();

      uint16_t source_val = (uint16_t) *s_reg;
      uint16_t destination_val = (uint16_t) *d_reg;

      printf("0x%04X(%s), 0x%04X(%s)\n", (uint16_t)source_offset, s_reg_name,
	     (uint16_t)destination_offset, d_reg_name);

      uint16_t *source_addr = get_addr_ptr(source_val + source_offset);
      uint16_t *destination_addr = 
	get_addr_ptr(destination_val + destination_offset);
      
      if (bw_flag == WORD) {
	*destination_addr = *source_addr;
      }
      else if (bw_flag == BYTE) {
	*((uint8_t *)destination_addr) = *((uint8_t *)source_addr);
      }

      break;
    }

    else if (as_flag == 2 && ad_flag == 0) {   /* MOV @Rs, Rd */
      printf("@%s, %s\n", s_reg_name, d_reg_name);

      uint16_t *source_addr = get_addr_ptr(*s_reg); 
    
      if (bw_flag == WORD) {
	*d_reg = *source_addr;
      }
      else if (bw_flag == BYTE) {
	*( (uint8_t *)d_reg ) = *( (uint8_t *)source_addr );
	*d_reg &= 0x00FF;
      }
      
      break;
    }

    else if (as_flag == 2 && ad_flag == 1) {   /* MOV @Rs, 0x0(Rd) */
      int16_t destination_offset = fetch();
      uint16_t *source_addr = get_addr_ptr(*s_reg);
      uint16_t *destination_addr = get_addr_ptr(*d_reg + destination_offset);

      printf("@%s, 0x%04X(%s)\n", s_reg_name, destination_offset, d_reg_name);

      if (bw_flag == WORD) {
	*destination_addr = *source_addr;
      }
      else if (bw_flag == BYTE) {
	*( (uint8_t *)destination_addr ) = *( (uint8_t *)source_addr );
      }
      
      break;
    }

    else if (as_flag == 3 && ad_flag == 0) {   /* MOV @Rs+, Rd */
                                               /* MOV #S, Rd   */

      if (source == 0) {   /* If the source Reg is PC/R1 */
	int16_t source_value = fetch();   /* Constant at PC */
	printf("#0x%04X, %s\n", source_value, d_reg_name);
	
	bw_flag == WORD ? 
	  *d_reg = source_value 
	  : (*d_reg = source_value, *d_reg &= 0x00FF);

	break;
      }
      else {
	uint16_t *source_addr = get_addr_ptr(*s_reg);
	printf("@%s+, %s\n", s_reg_name, d_reg_name);
	
	bw_flag == WORD ? 
	  *d_reg = *source_addr, *s_reg += 2
	  : (*d_reg = *source_addr, *d_reg &= 0x00FF, *s_reg += 1);
      }

      break;
    }

    else if (as_flag == 3 && ad_flag == 1) {   /* MOV @S+, 0x0(Rd) */
                                               /* MOV #S, 0x0(Rd)  */
      int16_t destination_offset = fetch();
      uint16_t *destination_addr = get_addr_ptr(*d_reg + destination_offset);

      if (source == 0) {   /* If the source Reg is PC/R1 */
	int16_t source_value = fetch();
	
	printf("#0x%04X, 0x%04X(%s)\n", (uint16_t)source_value, (uint16_t)
	       destination_offset, d_reg_name);

	bw_flag == WORD ? 
	  *destination_addr = source_value
	  : (*destination_addr = source_value, *destination_addr &= 0x00FF);
      
	break;
      }
      else {
	uint16_t *source_addr = get_addr_ptr(*s_reg); 
	printf("@%s+, 0x%04X(%s)\n", s_reg_name, (uint16_t) destination_offset,
	       d_reg_name);

	bw_flag == WORD ? 
	  *destination_addr = *s_reg, *s_reg += 2
	  : (*d_reg = *source_addr, *d_reg &= 0x00FF, *s_reg += 1);	

	bw_flag == WORD ? *s_reg += 2 : (*s_reg += 1);
      }
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
