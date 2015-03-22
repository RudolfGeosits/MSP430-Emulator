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

//##########+++ Decode Format I Instructions +++##########
//# Format I are double operand of the form:
//# [CCCC][SSSS][ABaa][DDDD]
//#
//# Where C = Opcode, B = Byte/Word flag,
//# A = Addressing mode for destination
//# a = Addressing mode for s_reg_name
//# S = S_Reg_Name, D = Destination
//########################################################

uint8_t is_overflowed (uint16_t source, uint16_t original_destination, 
		       uint16_t *result_addr, uint8_t bw_flag);
uint8_t is_negative (int16_t *result_addr, uint8_t bw_flag);
uint8_t is_zero (uint16_t *result_addr, uint8_t bw_flag);
uint8_t is_carried (uint16_t original_dst_value, uint16_t source_value, 
		    uint8_t bw_flag);

void decode_formatI(uint16_t instruction)
{
  uint8_t opcode = (instruction & 0xF000) >> 12;
  uint8_t source = (instruction & 0x0F00) >> 8;
  uint8_t as_flag = (instruction & 0x0030) >> 4;
  uint8_t destination = (instruction & 0x000F);
  uint8_t ad_flag = (instruction & 0x0080) >> 7;
  uint8_t bw_flag = (instruction & 0x0040) >> 6;

  char s_reg_name[10], d_reg_name[10];

  int16_t *s_reg = get_reg_ptr(source);     /* Source Register pointer */
  int16_t *d_reg = get_reg_ptr(destination);/* Destination Register pointer */

  reg_num_to_name(source, s_reg_name);      /* Get source register name */
  reg_num_to_name(destination, d_reg_name); /* Get destination register name */

  uint8_t constant_generator_active = 0;    /* Specifies if CG1/CG2 active */
  int16_t immediate_constant = 0;           /* Generated Constant */

  /*
  printf("Opcode: 0x%01X  Source bits: 0x%01X  Destination bits: 0x%01X\n" \
	 "AS_Flag: 0x%01X  AD_Flag: 0x%01X  BW_Flag: 0x%01X\n",
	 opcode, source, destination, as_flag, ad_flag, bw_flag);
  */

  /* Spot CG1 and CG2 Constant generator instructions */
  if ( (source == 2 && as_flag > 1) || source == 3 ) {
    constant_generator_active = 1;
    immediate_constant = run_constant_generator(source, as_flag);
    printf("Got constant: %d, using it.\n", immediate_constant);
  }
  else {
    constant_generator_active = 0;
  }

  /* Identify the nature of instruction operand addressing modes */
  int16_t source_value, source_offset;
  int16_t destination_offset;
  uint16_t *destination_addr;
  char asm_operands[20], asm_op2[20];
  
  memset(asm_operands, 0, sizeof asm_operands);
  memset(asm_op2, 0, sizeof asm_op2);

  /* Register - Register;     Ex: MOV Rs, Rd */
  /* Constant Gen - Register; Ex: MOV #C, Rd */ /* 0 */
  if (as_flag == 0 && ad_flag == 0) {
    if (constant_generator_active) {   /* Source Constant */
      source_value = immediate_constant;

      sprintf(asm_operands, "#0x%04X, %s", 
	      (uint16_t) source_value, d_reg_name);
    }
    else {                             /* Source register */
      source_value = *s_reg;
      sprintf(asm_operands, "%s, %s", s_reg_name, d_reg_name);
    }

    destination_addr = d_reg;          /* Destination Register */

    bw_flag == BYTE ? *d_reg &= 0x00FF : 0;
  }
  
  /* Register - Indexed;      Ex: MOV Rs, 0x0(Rd) */
  /* Register - Symbolic;     Ex: MOV Rs, 0xD     */
  /* Register - Absolute;     Ex: MOV Rs, &0xD    */
  /* Constant Gen - Indexed;  Ex: MOV #C, 0x0(Rd) */ /* 0 */
  /* Constant Gen - Symbolic; Ex: MOV #C, 0xD     */ /* 0 */
  /* Constant Gen - Absolute; Ex: MOV #C, &0xD    */ /* 0 */
  else if (as_flag == 0 && ad_flag == 1) {
    destination_offset = fetch();
    destination_addr = get_addr_ptr(*d_reg + destination_offset);
    
    if (constant_generator_active) {   /* Source Constant */
      source_value = immediate_constant;
      sprintf(asm_operands, "#0x%04X, ", source_value);
    }
    else {                             /* Source from register */
      source_value = *s_reg;
      sprintf(asm_operands, "%s, ", s_reg_name);
    }

    if (destination == 0) {            /* Destination Symbolic */
      uint16_t virtual_addr = *d_reg + destination_offset - 2;
      destination_addr = get_addr_ptr(virtual_addr);

      sprintf(asm_op2, "0x%04X", (uint16_t) virtual_addr);
    }
    else if (destination == 2) {       /* Destination Absolute */
      destination_addr = get_addr_ptr(destination_offset);
      sprintf(asm_op2, "&0x%04X", (uint16_t) destination_offset);
    }
    else {                             /* Destination Indexed */
      sprintf(asm_op2, "0x%04X(%s)", 
	      (uint16_t) destination_offset, d_reg_name);
    }

    strncat(asm_operands, asm_op2, sizeof asm_op2);
  }    

  /* Indexed - Register;      Ex: MOV 0x0(Rs), Rd */
  /* Symbolic - Register;     Ex: MOV 0xS, Rd     */
  /* Absolute - Register;     Ex: MOV &0xS, Rd    */
  /* Constant Gen - Register; Ex: MOV #C, Rd      */ /* 1 */
  else if (as_flag == 1 && ad_flag == 0) {
    if (constant_generator_active) {   /* Source Constant */
      source_value = immediate_constant;
      sprintf(asm_operands, "#%04X, %s", source_value, d_reg_name);
    }
    else if (source == 0) {            /* Source Symbolic */
      source_offset = fetch();
      uint16_t virtual_addr = *s_reg + source_offset - 2;

      source_value = *get_addr_ptr(virtual_addr);	
      sprintf(asm_operands, "0x%04X, %s", virtual_addr, d_reg_name);
    }
    else if (source == 2) {            /* Source Absolute */
      source_offset = fetch();
      source_value = *get_addr_ptr(source_offset);

      sprintf(asm_operands, "&0x%04X, %s", 
	      (uint16_t) source_offset, d_reg_name);
    }
    else {                             /* Source Indexed */
      source_offset = fetch();
      source_value = *get_addr_ptr(*s_reg + source_offset);

      sprintf(asm_operands, "0x%04X(%s), %s", 
	      (uint16_t) source_offset, s_reg_name, d_reg_name);  
    }

    destination_addr = d_reg;          /* Destination register */
    bw_flag == BYTE ? *d_reg &= 0x00FF : 0;
  }

  /* Indexed - Indexed;       Ex: MOV 0x0(Rs), 0x0(Rd) */
  /* Symbolic - Indexed;      Ex: MOV 0xS, 0x0(Rd)     */
  /* Indexed - Symbolic;      Ex: MOV 0x0(Rd), 0xD     */
  /* Symbolic - Symbolic;     Ex: MOV 0xS, 0xD         */
  /* Absolute - Indexed;      Ex: MOV &0xS, 0x0(Rd)    */
  /* Indexed - Absolute;      Ex: MOV 0x0(Rs), &0xD    */
  /* Absolute - Absolute;     Ex: MOV &0xS, &0xD       */
  /* Absolute - Symbolic;     Ex: MOV &0xS, 0xD        */
  /* Symbolic - Absolute;     Ex: MOV 0xS, &0xD        */
  /* Constant Gen - Indexed;  Ex: MOV #C, 0x0(Rd)      */ /* 1 */
  /* Constant Gen - Symbolic; Ex: MOV #C, 0xD          */ /* 1 */
  /* Constant Gen - Absolute; Ex: MOV #C, &0xD         */ /* 1 */
  else if (as_flag == 1 && ad_flag == 1) {
    if (constant_generator_active) {   /* Source Constant */
      source_value = immediate_constant;
      sprintf(asm_operands, "#0x%04X, ", source_value);
    }
    else if (source == 0) {            /* Source Symbolic */
      source_offset = fetch();
      uint16_t virtual_addr = PC + source_offset - 4;

      source_value = *get_addr_ptr(virtual_addr);
      sprintf(asm_operands, "0x%04X, ", virtual_addr);
    }
    else if (source == 2) {            /* Source Absolute */
      source_offset = fetch();
      source_value = *get_addr_ptr(source_offset);
      sprintf(asm_operands, "&0x%04X, ", (uint16_t) source_offset);
    }
    else {                             /* Source Indexed */
      source_offset = fetch();
      source_value = *get_addr_ptr(*s_reg + source_offset);

      sprintf(asm_operands, "0x%04X(%s), ", 
	      (uint16_t) source_offset, s_reg_name);
    }
      
    destination_offset = fetch();

    if (destination == 0) {        /* Destination Symbolic */
      uint16_t virtual_addr = PC + destination_offset - 2;

      destination_addr = get_addr_ptr(virtual_addr);
      sprintf(asm_op2, "0x%04X", virtual_addr);
    }
    else if (destination == 2) {   /* Destination Absolute */
      destination_addr = get_addr_ptr(destination_offset);
      sprintf(asm_op2, "&0x%04X", (uint16_t) destination_offset);
    }
    else {                         /* Destination indexed */
      destination_addr = get_addr_ptr(*d_reg + destination_offset);
      sprintf(asm_op2, "0x%04X(%s)", (uint16_t)destination_offset, d_reg_name);
    }

    strncat(asm_operands, asm_op2, sizeof asm_op2);
  }

  /* Indirect - Register;     Ex: MOV @Rs, Rd */
  /* Constant Gen - Register; Ex: MOV #C, Rd  */ /* 2, 4 */
  else if (as_flag == 2 && ad_flag == 0) {
    if (constant_generator_active) {   /* Source Constant */
      source_value = immediate_constant;
      sprintf(asm_operands, "#0x%04X, %s", immediate_constant, d_reg_name);
    }
    else {                             /* Source Indirect */
      source_value = *get_addr_ptr(*s_reg);
      sprintf(asm_operands, "@%s, %s", s_reg_name, d_reg_name);
    }

    destination_addr = d_reg;          /* Destination Register */
    bw_flag == BYTE ? *d_reg &= 0x00FF : 0;
  }

  /* Indirect - Indexed;      Ex: MOV @Rs, 0x0(Rd)   */
  /* Indirect - Symbolic;     Ex: MOV @Rs, 0xD       */
  /* Indirect - Absolute;     Ex: MOV @Rs, &0xD      */
  /* Constant Gen - Indexed;  Ex: MOV #C, 0x0(Rd)    */ /* 2, 4 */
  /* Constant Gen - Symbolic; Ex: MOV #C, 0xD        */ /* 2, 4 */
  /* Constant Gen - Absolute; Ex: MOV #C, &0xD       */ /* 2, 4 */
  else if (as_flag == 2 && ad_flag == 1) {
    destination_offset = fetch();

    if (constant_generator_active) {   /* Source Constant */
      source_value = immediate_constant;
      sprintf(asm_operands, "#0x%04X, ", source_value);
    }
    else {                             /* Source Indirect */
      source_value = *get_addr_ptr(*s_reg);
      sprintf(asm_operands, "@%s, ", s_reg_name);
    }

    if (destination == 0) {        /* Destination Symbolic */
      uint16_t virtual_addr = PC + destination_offset - 2;

      destination_addr = get_addr_ptr(virtual_addr);
      sprintf(asm_op2, "0x%04X", virtual_addr);
    }
    else if (destination == 2) {   /* Destination Absolute */
      destination_addr = get_addr_ptr(destination_offset);
      sprintf(asm_op2, "&0x%04X", destination_offset);
    }
    else {                         /* Destination Indexed */
      destination_addr = get_addr_ptr(*d_reg + destination_offset);
      sprintf(asm_op2, "0x%04X(%s)", (uint16_t)destination_offset, d_reg_name);
    }

    strncat(asm_operands, asm_op2, sizeof asm_op2);
  }

  /* Indirect Inc - Register; Ex: MOV @Rs+, Rd */
  /* Immediate - Register;    Ex: MOV #S, Rd   */
  /* Constant Gen - Register; Ex: MOV #C, Rd   */ /* -1, 8 */
  else if (as_flag == 3 && ad_flag == 0) {  
    if (constant_generator_active) {   /* Source Constant */
      source_value = immediate_constant;
      sprintf(asm_operands, "#0x%04X, %s", 
	      (uint16_t) source_value, d_reg_name);
    }
    else if (source == 0) {            /* Source Immediate */
      source_value = fetch();
	
      if (bw_flag == WORD) {
	sprintf(asm_operands, "#0x%04X, %s", 
		(uint16_t) source_value, d_reg_name);
      }
      else if (bw_flag == BYTE) {
	sprintf(asm_operands, "#0x%04X, %s",
		(uint8_t) source_value, d_reg_name);
      }
    }
    else {                              /* Source Indirect AutoIncrement */
      source_value = *get_addr_ptr(*s_reg);

      sprintf(asm_operands, "@%s+, %s", s_reg_name, d_reg_name);
      bw_flag == WORD ? *s_reg += 2 : (*s_reg += 1);
    }

    destination_addr = d_reg;           /* Destination Register */
    bw_flag == BYTE ? *d_reg &= 0x00FF : 0;
  }

  /* Indirect Inc - Indexed;  Ex: MOV @Rs+, 0x0(Rd) */
  /* Indirect Inc - Symbolic; Ex: MOV @Rs+, 0xD     */
  /* Indirect Inc - Absolute; Ex: MOV @Rs+, &0xD    */
  /* Immediate - Indexed;     Ex: MOV #S, 0x0(Rd)   */
  /* Immediate - Symbolic;    Ex: MOV #S, 0xD       */
  /* Immediate - Absolute;    Ex: MOV #S, &0xD      */
  /* Constant Gen - Indexed;  Ex: MOV #C, 0x0(Rd)   */ /* -1, 8 */
  /* Constant Gen - Symbolic; Ex: MOV #C, 0xD       */ /* -1, 8 */
  /* Constant Gen - Absolute; Ex: MOV #C, &0xD      */ /* -1, 8 */
  else if (as_flag == 3 && ad_flag == 1) {
    if (constant_generator_active) {   /* Source Constant */
      source_value = immediate_constant;
      sprintf(asm_operands, "#0x%04X, ", (uint16_t)source_value);
    }
    else if (source == 0) {            /* Source Immediate */
      source_value = fetch();
      sprintf(asm_operands, "#0x%04X, ", (uint16_t)source_value);
    }
    else {                             /* Source Indirect Auto Increment */
      source_value = *get_addr_ptr(*s_reg); 

      sprintf(asm_operands, "@%s+, ", s_reg_name);	
      bw_flag == WORD ? *s_reg += 2 : (*s_reg += 1);
    }

    destination_offset = fetch();

    if (destination == 0) {        /* Destination Symbolic */
      uint16_t virtual_addr = PC + destination_offset - 2;
	
      destination_addr = get_addr_ptr(virtual_addr);
      sprintf(asm_op2, "0x%04X", virtual_addr);
    }
    else if (destination == 2) {   /* Destination Absolute */
      destination_addr = get_addr_ptr(destination_offset);
      sprintf(asm_op2, "&0x%04X", (uint16_t) destination_offset);
    }
    else {                         /* Destination Indexed */
      destination_addr = get_addr_ptr(*d_reg + destination_offset);

      sprintf(asm_op2, "0x%04X(%s)", 
	      (uint16_t) destination_offset, d_reg_name);
    }

    strncat(asm_operands, asm_op2, sizeof asm_op2);
  }
  
  
  switch (opcode) {
    
    /* MOV SOURCE, DESTINATION
     *   Ex: MOV #4, R6
     *
     * SOURCE = DESTINATION
     *
     * The source operand is moved to the destination. The source operand is 
     * not affected. The previous contents of the destination are lost.
     * 
     */
    case 0x4: {
      bw_flag == 0 ? printf("MOV ") : printf("MOV.B ");

      if (bw_flag == WORD) {		
	*destination_addr = source_value;
      }
      else if (bw_flag == BYTE) {
	*((uint8_t *) destination_addr) = (uint8_t) source_value;
      }
      
      break;
    }

    /* ADD SOURCE, DESTINATION 
     *   Ex: ADD R5, R4
     * 
     * The source operand is added to the destination operand. The source op
     * is not affected. The previous contents of the destination are lost.
     *
     * DESTINATION = SOURCE + DESTINATION
     *   
     * N: Set if result is negative, reset if positive
     * Z: Set if result is zero, reset otherwise
     * C: Set if there is a carry from the result, cleared if not
     * V: Set if an arithmetic overflow occurs, otherwise reset  
     *
     */
    case 0x5:{
      bw_flag == 0 ? printf("ADD ") : printf("ADD.B ");
      uint16_t original_dst_value = *destination_addr;

      if (bw_flag == WORD) {
	*destination_addr += source_value;
      }
      else if (bw_flag == BYTE) {
	*((uint8_t *) destination_addr) += (uint8_t) source_value;
      }

      SR.zero = is_zero(destination_addr, bw_flag);
      SR.negative = is_negative(destination_addr, bw_flag);

      SR.carry = is_carried(original_dst_value, source_value, bw_flag);

      SR.overflow = is_overflowed(source_value, original_dst_value, 
				  destination_addr, bw_flag);

      break;
    }

    /* ADDC SOURCE, DESTINATION 
     *   Ex: ADDC R5, R4
     *
     * DESTINATION += (SOURCE + C)
     *
     * N: Set if result is negative, reset if positive
     * Z: Set if result is zero, reset otherwise
     * C: Set if there is a carry from the result, cleared if not
     * V: Set if an arithmetic overflow occurs, otherwise reset  
     *
     */
    case 0x6:{
      bw_flag == 0 ? printf("ADDC ") : printf("ADDC.B ");
      uint16_t original_dst_value = *destination_addr;

      if (bw_flag == WORD) {
	*destination_addr += source_value + SR.carry;
      }
      else if (bw_flag == BYTE) {
	*((uint8_t *) destination_addr) += (uint8_t) source_value + SR.carry;
      }
      
      SR.zero = is_zero(destination_addr, bw_flag);
      SR.negative = is_negative(destination_addr, bw_flag);

      SR.carry = is_carried(original_dst_value, source_value, bw_flag);

      SR.overflow = is_overflowed(source_value, original_dst_value, 
				  destination_addr, bw_flag);
      break;
    }
 
    /* SUBC SOURCE, DESTINATION
     *   Ex: SUB R4, R5
     *
     *   DST += ~SRC + C
     *
     *  N: Set if result is negative, reset if positive
     *  Z: Set if result is zero, reset otherwise
     *  C: Set if there is a carry from the MSB of the result, reset otherwise.
     *     Set to 1 if no borrow, reset if borrow.
     *  V: Set if an arithmetic overflow occurs, otherwise reset
     *
     *
     */
    case 0x7:{
      bw_flag == 0 ? printf("SUBC ") : printf("SUBC.B ");
      int16_t original_dst_value = *destination_addr;
      source_value = ~source_value; /* 1's comp */

      if (bw_flag == WORD) {
	*(int16_t *)destination_addr += source_value + SR.carry; 
      }
      else if (bw_flag == BYTE) {
	*(int8_t *)destination_addr += (int8_t) source_value + SR.carry;
      }

      SR.zero = is_zero(destination_addr, bw_flag);
      SR.negative = is_negative(destination_addr, bw_flag);

      SR.carry = is_carried(original_dst_value, source_value, bw_flag);

      SR.overflow = is_overflowed(source_value, original_dst_value, 
				  destination_addr, bw_flag);
      break;
    }

    /* SUB SOURCE, DESTINATION
     *   Ex: SUB R4, R5
     *
     *   DST -= SRC
     *
     *  N: Set if result is negative, reset if positive
     *  Z: Set if result is zero, reset otherwise
     *  C: Set if there is a carry from the MSB of the result, reset otherwise.
     *     Set to 1 if no borrow, reset if borrow.
     *  V: Set if an arithmetic overflow occurs, otherwise reset
     *  TODO: SUBTRACTION OVERFLOW FLAG ERROR
     */  

    case 0x8:{
      bw_flag == 0 ? printf("SUB ") : printf("SUB.B ");
      int16_t original_dst_value = *destination_addr;
      source_value = ~source_value + 1;

      if (bw_flag == WORD) {
	*(uint16_t *)destination_addr += source_value; 
      }
      else if (bw_flag == BYTE) {
	*(uint8_t *)destination_addr += (uint8_t) source_value;
      }

      SR.zero = is_zero(destination_addr, bw_flag);
      SR.negative = is_negative(destination_addr, bw_flag);

      SR.carry = is_carried(original_dst_value, source_value, bw_flag);

      SR.overflow = is_overflowed(source_value, original_dst_value, 
				  destination_addr, bw_flag);
      break;
    }

    /* CMP SOURCE, DESTINATION
     *
     */
    case 0x9:{
      bw_flag == 0 ? printf("CMP ") : printf("CMP.B ");
      
      if (bw_flag == WORD) {
	
      }
      else if (bw_flag == BYTE) {
	
      }
      
      break;
    }

    /* DADD SOURCE, DESTINATION
     *
     */
    case 0xA:{
      bw_flag == 0 ? printf("DADD ") : printf("DADD.B ");
    
      if (bw_flag == WORD) {
	
      }
      else if (bw_flag == BYTE) {
	
      }
      
      break;
    }

    /* BIT SOURCE, DESTINATION
     *
     */
    case 0xB:{
      bw_flag == 0 ? printf("BIT ") : printf("BIT.B ");
    
      if (bw_flag == WORD) {
	
      }
      else if (bw_flag == BYTE) {
	
      }

      break;
    }     

    /* BIC SOURCE, DESTINATION
     *
     */
    case 0xC:{
      bw_flag == 0 ? printf("BIC ") : printf("BIC.B ");
      
      if (bw_flag == WORD) {
	
      }
      else if (bw_flag == BYTE) {
	
      }
      
      break;
    }

    /* BIS SOURCE, DESTINATION
     *
     */
    case 0xD:{
      bw_flag == 0 ? printf("BIS ") : printf("BIS.B ");
      
      if (bw_flag == WORD) {
	
      }
      else if (bw_flag == BYTE) {
	
      }
      
      break;
    }
    
    /* XOR SOURCE, DESTINATION
     *
     */
    case 0xE:{
      bw_flag == 0 ? printf("XOR ") : printf("XOR.B ");

      if (bw_flag == WORD) {
	
      }
      else if (bw_flag == BYTE) {
	
      }
      
      break;
    }

    /* AND SOURCE, DESTINATION
     *
     */
    case 0xF:{
      bw_flag == 0 ? printf("AND ") : printf("AND.B ");

      if (bw_flag == WORD) {
	
      }
      else if (bw_flag == BYTE) {
	
      }
      
      break;
    }

  } //# End of switch

  
  printf("%s\n", asm_operands);
}

uint8_t is_zero (uint16_t *result_addr, uint8_t bw_flag) 
{
  if (bw_flag == WORD) {
    if (*result_addr == 0 ) {
      return 1;
    }
    
    return 0;
  }
  else if (bw_flag == BYTE) {
    if (*(int8_t *) result_addr == 0) {
      return 1;
    }

    return 0;
  }
}

uint8_t is_negative(int16_t *result_addr, uint8_t bw_flag) 
{
  if (bw_flag == WORD) {
    if (*result_addr < 0) {
      return 1;
    }

    return 0;
  }
  else if (bw_flag == BYTE) {
    if (*((int8_t *) result_addr) < 0) { 
      return 1;
    }

    return 0;
  }
}

uint8_t is_carried(uint16_t original_dst_value, uint16_t source_value, 
		   uint8_t bw_flag)
{
  if (bw_flag == WORD) {
    if ((65535 - source_value) < original_dst_value) {
      return 1;
    }

    return 0;
  }
  else if (bw_flag == BYTE) {
    if ((255 - (uint8_t)source_value) < (uint8_t)original_dst_value) {
      return 1;
    }

    return 0;
  }
}

uint8_t is_overflowed(uint16_t source_value, uint16_t destination_value, 
		      uint16_t *result, uint8_t bw_flag) 
{
  if (bw_flag == WORD) {
    if ( (source_value >> 15) == (destination_value >> 15) &&
	 (*result >> 15) != (destination_value >> 15) ) {
      return 1;
    }

    return 0;
  }
  else if (bw_flag == BYTE) {
    uint8_t dst_prev_value = (uint8_t) destination_value;
    uint8_t src_value = (uint8_t) source_value;

    if ( (src_value >> 7) == (dst_prev_value >> 7) && 
	 (*(uint8_t *)result >> 7) != (dst_prev_value >> 7)) {
      return 1;
    }

    return 0;
  }
  else {
    printf("Error, overflowed function: invalid bw_flag\n");
  }
}
