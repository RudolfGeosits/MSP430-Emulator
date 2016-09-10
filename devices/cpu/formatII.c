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

//##########+++ Decode Format II Instructions +++#########
//# Format II are single operand of the form:
//#   [0001][00CC][CBAA][SSSS]
//# 
//# Where C = Opcode, B = Byte/Word flag, 
//#       A = Addressing mode for source
//#       S = Source 
//########################################################

#include "formatII.h"
#include "decoder.h"
#include "../utilities.h"

void decode_formatII(Emulator *emu, uint16_t instruction, bool disassemble)
{
  Cpu *cpu = emu->cpu;
  Debugger *debugger = emu->debugger;

  uint8_t opcode = (instruction & 0x0380) >> 7;
  uint8_t bw_flag = (instruction & 0x0040) >> 6;
  uint8_t as_flag = (instruction & 0x0030) >> 4;
  uint8_t source = (instruction & 0x000F);
  
  char reg_name[10];
  reg_num_to_name(source, reg_name);
  
  uint16_t *reg = get_reg_ptr(emu, source);
  uint16_t bogus_reg; /* For immediate values to be operated on */

  uint8_t constant_generator_active = 0;    /* Specifies if CG1/CG2 active */
  int16_t immediate_constant = 0;           /* Generated Constant */

  char mnemonic[100] = {0};
  /* String to show hex value of instruction */
  char hex_str[100] = {0};
  char hex_str_part[10] = {0};

  sprintf(hex_str, "%04X", instruction);

  /*
  printf("Opcode: 0x%01X  Source bits: 0x%01X\nAS_Flag: 0x%01X  "\
	 "BW_Flag: 0x%01X\n",
         opcode, source, as_flag, bw_flag);
  */

  /* Spot CG1 and CG2 Constant generator instructions */
  if ( (source == 2 && as_flag > 1) || source == 3 ) {
    constant_generator_active = 1;
    immediate_constant = run_constant_generator(source, as_flag);
  }
  else {
    constant_generator_active = 0;
  }

  /* Identify the nature of instruction operand addressing modes */
  int16_t source_value, source_offset;
  uint16_t *source_address;
  char asm_operand[50] = {0};

  /* Register;     Ex: PUSH Rd */
  /* Constant Gen; Ex: PUSH #C */   /* 0 */
  if (as_flag == 0) {
    if (constant_generator_active) {   /* Source Constant */
      source_value = bogus_reg = immediate_constant;
      source_address = &bogus_reg;

      sprintf(asm_operand, "#0x%04X", (uint16_t) source_value);
    }
    else {                             /* Source Register */
      source_value = *reg;
      source_address = reg;

      sprintf(asm_operand, "%s", reg_name);
    }

    bw_flag == BYTE ? *reg &= 0x00FF : 0;
  }

  /* Indexed;      Ex: PUSH 0x0(Rs) */
  /* Symbolic;     Ex: PUSH 0xS     */
  /* Absolute:     Ex: PUSH &0xS    */
  /* Constant Gen; Ex: PUSH #C      */ /* 1 */
  else if (as_flag == 1) {
    if (constant_generator_active) {   /* Source Constant */
      source_value = bogus_reg = immediate_constant;
      source_address = &bogus_reg;

      sprintf(asm_operand, "#0x%04X", source_value);
    }
    else if (source == 0) {            /* Source Symbolic */
      source_offset = fetch(emu);
      uint16_t virtual_addr = cpu->pc + source_offset;

      sprintf(hex_str_part, "%04X", (uint16_t) source_offset);
      strncat(hex_str, hex_str_part, sizeof hex_str);

      source_address = get_addr_ptr(virtual_addr);

      sprintf(asm_operand, "0x%04X", virtual_addr);
    }
    else if (source == 2) {            /* Source Absolute */
      source_offset = fetch(emu);
      source_address = get_addr_ptr(source_offset);
      source_value = *source_address;

      sprintf(hex_str_part, "%04X", (uint16_t) source_value);
      strncat(hex_str, hex_str_part, sizeof hex_str);

      sprintf(asm_operand, "&0x%04X", (uint16_t) source_offset);
    }
    else {                             /* Source Indexed */
      source_offset = fetch(emu);
      source_address = get_addr_ptr(*reg + source_offset);
      source_value = *source_address;

      sprintf(hex_str_part, "%04X", (uint16_t) source_offset);
      strncat(hex_str, hex_str_part, sizeof hex_str);

      sprintf(asm_operand, "0x%04X(%s)", (uint16_t) source_offset, reg_name);
    }
  }

  /* Indirect;     Ex: PUSH @Rs */
  /* Constant Gen; Ex: PUSH #C */ /* 2, 4 */
  else if (as_flag == 2) {
    if (constant_generator_active) {   /* Source Constant */
      source_value = bogus_reg = immediate_constant;
      source_address = &bogus_reg;

      sprintf(asm_operand, "#0x%04X", immediate_constant);
    }
    else {                             /* Source Indirect */
      source_address = get_addr_ptr(*reg);
      source_value = *source_address;

      sprintf(asm_operand, "@%s", reg_name);
    }
  }

  /* Indirect AutoIncrement; Ex: PUSH @Rs+ */
  /* Immediate;              Ex: PUSH #S   */
  /* Constant Gen;           Ex: PUSH #C   */ /* -1, 8 */
  else if (as_flag == 3) {
    if (constant_generator_active) {   /* Source Constant */
      source_value = bogus_reg = immediate_constant;
      source_address = &bogus_reg;

      sprintf(asm_operand, "#0x%04X", (uint16_t) source_value);
    }
    else if (source == 0) {            /* Source Immediate */
      source_value = bogus_reg = fetch(emu);
      source_address = &bogus_reg;

      sprintf(hex_str_part, "%04X", (uint16_t) source_value);
      strncat(hex_str, hex_str_part, sizeof hex_str);

      if (bw_flag == WORD) {
        sprintf(asm_operand, "#0x%04X", (uint16_t) source_value);
      }
      else if (bw_flag == BYTE) {
        sprintf(asm_operand, "#0x%04X", (uint8_t) source_value);
      }
    }
    else {                              /* Source Indirect AutoIncrement */
      source_address = get_addr_ptr(*reg);
      source_value = *source_address;

      sprintf(asm_operand, "@%s+", reg_name);
      bw_flag == WORD ? *reg += 2 : (*reg += 1);
    }
  }


  if (!disassemble) {
    switch (opcode) {
        
      /*  RRC Rotate right through carry
       *    C → MSB → MSB-1 .... LSB+1 → LSB → C
       *  
       *  Description The destination operand is shifted right one position 
       *  as shown in Figure 3-18. The carry bit (C) is shifted into the MSB, 
       *  the LSB is shifted into the carry bit (C).
       *
       * N: Set if result is negative, reset if positive
       * Z: Set if result is zero, reset otherwise
       * C: Loaded from the LSB
       * V: Reset
       * TODO: UNDEFINED BEHAVIOR DURRING CONSTANT MANIPULATION, BROKEN
       */
    case 0x0:{
      bool CF = cpu->sr.carry;

      if (bw_flag == WORD) {
	cpu->sr.carry = *source_address & 0x0001;  /* Set CF from LSB */
	*source_address >>= 1;                /* Shift one right */
	CF ? *source_address |= 0x8000 : 0;   /* Set MSB from prev CF */
      }
      else if (bw_flag == BYTE){
	cpu->sr.carry = *(uint8_t *) source_address & 0x01;
	*(uint8_t *) source_address >>= 1;
	CF ? *(uint8_t *) source_address |= 0x80 : 0;
      }

      cpu->sr.zero = is_zero(source_address, bw_flag);
      cpu->sr.negative = is_negative(source_address, bw_flag);
      cpu->sr.overflow = false;

      break;
    }
    
      /* SWPB Swap bytes
       * bw flag always 0 (word)
       * Bits 15 to 8 ↔ bits 7 to 0
       */
    case 0x1:{	
      uint8_t upper_nibble, lower_nibble;
      upper_nibble = (*source_address & 0xFF00) >> 8;
      lower_nibble = *source_address & 0x00FF;
    
      *source_address = ((uint16_t)0|(lower_nibble << 8)) | upper_nibble;

      break;
    }
    
      /* RRA Rotate right arithmetic 
       *   MSB → MSB, MSB → MSB-1, ... LSB+1 → LSB, LSB → C
       * 
       * N: Set if result is negative, reset if positive
       * Z: Set if result is zero, reset otherwise
       * C: Loaded from the LSB
       * V: Reset
       */
    case 0x2:{
      if (bw_flag == WORD) {
	cpu->sr.carry = *source_address & 0x0001;
	bool msb = *source_address >> 15;
	*source_address >>= 1;
	msb ? *source_address |= 0x8000 : 0; /* Extend Sign */
      }
      else if (bw_flag == BYTE) {
	cpu->sr.carry = *source_address & 0x0001;
	bool msb = *source_address >> 7;
	*source_address >>= 1;
	msb ? *source_address |= 0x0080 : 0;
      }

      cpu->sr.zero = is_zero(source_address, bw_flag);
      cpu->sr.negative = is_negative(source_address, bw_flag);
      cpu->sr.overflow = false;
      break;
    }

      /* SXT Sign extend byte to word
       *   bw flag always 0 (word)
       *
       * Bit 7 → Bit 8 ......... Bit 15
       * 
       * N: Set if result is negative, reset if positive
       * Z: Set if result is zero, reset otherwise
       * C: Set if result is not zero, reset otherwise (.NOT. Zero)
       * V: Reset
       */

    case 0x3:{
      if (*source_address & 0x0080) {
	*source_address |= 0xFF00;
      }
      else {
	*source_address &= 0x00FF;
      }
    
      cpu->sr.negative = is_negative(source_address, WORD);
      cpu->sr.zero = is_zero(source_address, WORD);
      cpu->sr.carry = ! cpu->sr.zero;
      cpu->sr.overflow = false;

      break;
    }
  
      /* PUSH push value on to the stack
       *   
       *   SP - 2 → SP
       *   src → @SP
       *
       */
    case 0x4:{

      cpu->sp -= 2; /* Yes, even for BYTE Instructions */
      uint16_t *stack_address = get_stack_ptr(emu);
    
      if (bw_flag == WORD) {
	*stack_address = source_value;
      }
      else if (bw_flag == BYTE) {
	*stack_address &= 0xFF00; /* Zero out bottom half for pushed byte */
	*stack_address |= (uint8_t) source_value;
      }

      break;
    }

      /* CALL SUBROUTINE: 
       *     PUSH PC and PC = SRC
       *     
       *     This is always a word instruction. Supporting all addressing modes
       */
    
    case 0x5:{
    
      cpu->sp -= 2;
      uint16_t *stack_address = get_stack_ptr(emu);
      *stack_address = cpu->pc;
      cpu->pc = *source_address;

      break;
    }
  
      //# RETI Return from interrupt: Pop SR then pop PC
    case 0x6:{
       
      break;
    }
    default:{
      printf("Unknown Single operand instruction.\n");
    }

    } //# End of Switch
  } //# end if
  

  else {    
    switch (opcode) {
    case 0x0: {
      bw_flag == WORD ?
	strncpy(mnemonic, "RRC", sizeof mnemonic) :
	strncpy(mnemonic, "RRC.B", sizeof mnemonic);    

      break;
    }
    case 0x1: {
      strncpy(mnemonic, "SWPB", sizeof mnemonic);    
      break;
    }
    case 0x2: {
      bw_flag == WORD ?
	strncpy(mnemonic, "RRA", sizeof mnemonic) :
	strncpy(mnemonic, "RRA.B", sizeof mnemonic);     

      break;
    }
    case 0x3: {
      strncpy(mnemonic, "SXT", sizeof mnemonic);    
      break;
    }
    case 0x4: {
      bw_flag == WORD ?
	strncpy(mnemonic, "PUSH", sizeof mnemonic) :
	strncpy(mnemonic, "PUSH.B", sizeof mnemonic);    

      break;
    }
    case 0x5: {
      strncpy(mnemonic, "CALL", sizeof mnemonic);
      break;
    }
    case 0x6: {
      strncpy(mnemonic, "RETI", sizeof mnemonic);           
      break;
    }
    default: {
      printf("Unknown Single operand instruction.\n");
    }

    } //# End of Switch

    strncat(mnemonic, "\t", sizeof mnemonic);
    strncat(mnemonic, asm_operand, sizeof mnemonic);
    strncat(mnemonic, "\n", sizeof mnemonic);
    
    if (disassemble && emu->debugger->debug_mode) {
      int i;
      char one = 0, two = 0;

      // Make little endian big endian
      for (i = 0;i < strlen(hex_str);i += 4) {
	one = hex_str[i];
	two = hex_str[i + 1];

	hex_str[i] = hex_str[i + 2];
        hex_str[i + 1] = hex_str[i + 3];

	hex_str[i + 2] = one;
	hex_str[i + 3] = two;
      }

      printf("%s", hex_str);
      print_console(emu, hex_str);

      for (i = strlen(hex_str);i < 12;i++) {
	printf(" ");
	print_console(emu, " ");
      }

      printf("\t%s", mnemonic);

      print_console(emu, "\t");
      print_console(emu, mnemonic);
    }

  } //# end else

}

