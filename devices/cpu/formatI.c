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

  printf("Opcode: 0x%01X  Source bits: 0x%01X  Destination bits: 0x%01X\n" \
	 "AS_Flag: 0x%01X  AD_Flag: 0x%01X  BW_Flag: 0x%01X\n",
	 opcode, source, destination, as_flag, ad_flag, bw_flag);

  /* Spot CG1 and CG2 Constant generator instructions */
  if ( (source == 2 && as_flag > 1) || source == 3 ) {
    constant_generator_active = 1;
    immediate_constant = run_constant_generator(source, as_flag);
  }
  else {
    constant_generator_active = 0;
  }

  if (constant_generator_active) {
    printf("Got constant: %d, using it.\n", immediate_constant);
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

    /* Register - Register;     Ex: MOV Rs, Rd */
    /* Constant Gen - Register; Ex: MOV #C, Rd */    
    if (as_flag == 0 && ad_flag == 0) {
      int16_t source_value;

      if (constant_generator_active) {   /* Source Constant */
	source_value = immediate_constant;
	printf("#0x%04X, ", source_value);
      }
      else {                             /* Source from register */
	source_value = *s_reg;
	printf("%s, ", s_reg_name);
      }

      printf("%s\n", d_reg_name);
      
      if (bw_flag == WORD) {		
	*d_reg = source_value;
      }
      else if (bw_flag == BYTE) {
	*d_reg = (uint8_t) source_value;
      }

      break;
    }

    /* Register - Indexed;      Ex: MOV Rs, 0x0(Rd) */
    /* Register - Symbolic;     Ex: MOV Rs, 0xD     */
    /* Register - Absolute;     Ex: MOV Rs, &0xD    */
    /* Constant Gen - Indexed;  Ex: MOV #C, 0x0(Rd) */
    /* Constant Gen - Symbolic; Ex: MOV #C, 0xD     */
    /* Constant Gen - Absolute; Ex: MOV #C, &0xD    */
    else if (as_flag == 0 && ad_flag == 1) {
      int16_t destination_offset = fetch();
      uint16_t *destination_addr = get_addr_ptr(*d_reg + destination_offset);
      int16_t source_value; 

      if (constant_generator_active) {   /* Source Constant */
	source_value = immediate_constant;
	printf("#0x%04X, ", source_value);
      }
      else {                             /* Source from register */
	source_value = *s_reg;
	printf("%s, ", s_reg_name);
      }

      if (destination == 0) {            /* Destination Symbolic */
	uint16_t virtual_addr = *d_reg + destination_offset - 2;

	destination_addr = get_addr_ptr(virtual_addr);
	printf("0x%04X\n", (uint16_t) virtual_addr);
      }
      else if (destination == 2) {       /* Destination Absolute */
	destination_addr = get_addr_ptr(destination_offset);
	printf("&0x%04X\n", (uint16_t) destination_offset);
      }
      else {                             /* Destination Indexed */
	printf("0x%04X(%s)\n", (uint16_t) destination_offset, d_reg_name);
      }

      if (bw_flag == WORD) {
	*destination_addr = source_value;
      }
      else if (bw_flag == BYTE) {
	*( (uint8_t *)destination_addr ) = (uint8_t) source_value;
      }
	
      break;
    }

    /* Indexed - Register;      Ex: MOV 0x0(Rs), Rd */
    /* Symbolic - Register;     Ex: MOV 0xS, Rd     */
    /* Absolute - Register;     Ex: MOV &0xS, Rd    */
    /* Constant Gen - Register; Ex: MOV #C, Rd      */
    else if (as_flag == 1 && ad_flag == 0) {
      int16_t source_offset;
      uint16_t source_value;

      if (constant_generator_active) {   /* Source Constant */
	source_value = immediate_constant;
	printf("#%04X, %s\n", source_value, d_reg_name);
      }
      else if (source == 0) {            /* Source Symbolic */
	source_offset = fetch();
	uint16_t virtual_addr = *s_reg + source_offset - 2;

	source_value = *get_addr_ptr(virtual_addr);	
	printf("0x%04X, %s\n", virtual_addr, d_reg_name);
      }
      else if (source == 2) {            /* Source Absolute */
	source_offset = fetch();
	source_value = *get_addr_ptr(source_offset);
	printf("&0x%04X, %s\n", (uint16_t) source_offset, d_reg_name);
      }
      else {                             /* Source Indexed */
	source_offset = fetch();
	source_value = *get_addr_ptr(*s_reg + source_offset);

	printf("0x%04X(%s), %s\n", (uint16_t) source_offset, s_reg_name, 
	       d_reg_name);  
      }

      if (bw_flag == WORD) {
	*d_reg = source_value;
      }
      else if (bw_flag == BYTE) {
	*d_reg = (uint8_t) source_value;
      }

      break;
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
    /* Constant Gen - Indexed;  Ex: MOV #C, 0x0(Rd)      */
    /* Constant Gen - Symbolic; Ex: MOV #C, 0xD          */
    /* Constant Gen - Absolute; Ex: MOV #C, &0xD         */
    else if (as_flag == 1 && ad_flag == 1) {
      int16_t source_offset;
      uint16_t source_value;

      if (constant_generator_active) {   /* Source Constant */
	source_value = immediate_constant;
	printf("#0x%04X, ", source_value);
      }
      else if (source == 0) {            /* Source Symbolic */
	source_offset = fetch();
	uint16_t virtual_addr = PC + source_offset - 4;

	source_value = *get_addr_ptr(virtual_addr);
	printf("0x%04X, ", virtual_addr);
      }
      else if (source == 2) {            /* Source Absolute */
	source_offset = fetch();
	source_value = *get_addr_ptr(source_offset);
	printf("&0x%04X, ", (uint16_t) source_offset);
      }
      else {                             /* Source Indexed */
	source_offset = fetch();
	source_value = *get_addr_ptr(*s_reg + source_offset);
	printf("0x%04X(%s), ", (uint16_t) source_offset, s_reg_name);
      }
      
      int16_t destination_offset = fetch();
      uint16_t *destination_addr;

      if (destination == 0) {        /* Destination Symbolic */
	uint16_t virtual_addr = PC + destination_offset - 2;

	destination_addr = get_addr_ptr(virtual_addr);
	printf("0x%04X\n", virtual_addr);
      }
      else if (destination == 2) {   /* Destination Absolute */
	destination_addr = get_addr_ptr(destination_offset);
	printf("&0x%04X\n", (uint16_t) destination_offset);
      }
      else {                         /* Destination indexed */
	destination_addr = get_addr_ptr(*d_reg + destination_offset);
	printf("0x%04X(%s)\n", (uint16_t) destination_offset, d_reg_name);
      }
      
      if (bw_flag == WORD) {
	*destination_addr = source_value;
      }
      else if (bw_flag == BYTE) {
	*((uint8_t *) destination_addr) = (uint8_t) source_value;
      }

      break;
    }

    /* Indirect - Register;     Ex: MOV @Rs, Rd */
    /* Constant Gen - Register; Ex: MOV #C, Rd  */
    else if (as_flag == 2 && ad_flag == 0) {
      uint16_t source_value;

      if (constant_generator_active) {   /* Source Constant */
	source_value = immediate_constant;
	printf("#0x%04X, %s\n", immediate_constant, d_reg_name);
      }
      else {                             /* Source Indirect */
	source_value = *get_addr_ptr(*s_reg);
	printf("@%s, %s\n", s_reg_name, d_reg_name);
      }
    
      if (bw_flag == WORD) {
	*d_reg = source_value;
      }
      else if (bw_flag == BYTE) {
	*d_reg = (uint8_t) source_value;
      }
      
      break;
    }

    /* Indirect - Indexed;      Ex: MOV @Rs, 0x0(Rd)   */
    /* Indirect - Symbolic;     Ex: MOV @Rs, 0xD       */
    /* Indirect - Absolute;     Ex: MOV @Rs, &0xD      */
    /* Constant Gen - Indexed;  Ex: MOV #C, 0x0(Rd)    */
    /* Constant Gen - Symbolic; Ex: MOV #C, 0xD        */
    /* Constant Gen - Absolute; Ex: MOV #C, &0xD       */
    else if (as_flag == 2 && ad_flag == 1) {
      uint16_t source_value;
      uint16_t *destination_addr;
      int16_t destination_offset = fetch();

      if (constant_generator_active) {   /* Source Constant */
	source_value = immediate_constant;
	printf("#0x%04X, ", source_value);
      }
      else {                             /* Source Indirect */
	source_value = *get_addr_ptr(*s_reg);
	printf("@%s, ", s_reg_name);
      }

      if (destination == 0) {        /* Destination Symbolic */
	uint16_t virtual_addr = PC + destination_offset - 2;

	destination_addr = get_addr_ptr(virtual_addr);
	printf("0x%04X\n", virtual_addr);
      }
      else if (destination == 2) {   /* Destination Absolute */
	destination_addr = get_addr_ptr(destination_offset);
	printf("&0x%04X\n", destination_offset);
      }
      else {                         /* Destination Indexed */
	destination_addr = get_addr_ptr(*d_reg + destination_offset);
	printf("0x%04X(%s)\n", (uint16_t) destination_offset, d_reg_name);
      }

      if (bw_flag == WORD) {
	*destination_addr = source_value;
      }
      else if (bw_flag == BYTE) {
	*((uint8_t *) destination_addr) = (uint8_t) source_value;
      }
      
      break;
    }

    /* Indirect Inc - Register; Ex: MOV @Rs+, Rd */
    /* Immediate - Register;    Ex: MOV #S, Rd   */
    /* Constant Gen - Register; Ex: MOV #C, Rd   */
    else if (as_flag == 3 && ad_flag == 0) {
      uint16_t source_value;
      
      if (constant_generator_active) {   /* Source Constant */
	source_value = immediate_constant;
	printf("#0x%04X, %s\n", (uint16_t) source_value, d_reg_name);
      }
      else if (source == 0) {            /* Source Immediate */
	source_value = fetch();
	
	printf("#0x%04X, %s\n", (uint16_t) source_value, d_reg_name);
      }
      else {                              /* Source Indirect AutoIncrement */
	source_value = *get_addr_ptr(*s_reg);

	printf("@%s+, %s\n", s_reg_name, d_reg_name);
	bw_flag == WORD ? *s_reg += 2 : (*s_reg += 1);
      }

      if (bw_flag == WORD) {
	*d_reg = source_value;
      }
      else if (bw_flag == BYTE) {
	*d_reg = (uint8_t) source_value;
      }

      break;
    }

    /* Indirect Inc - Indexed;  Ex: MOV @Rs+, 0x0(Rd) */
    /* Indirect Inc - Symbolic; Ex: MOV @Rs+, 0xD     */
    /* Indirect Inc - Absolute; Ex: MOV @Rs+, &0xD    */
    /* Immediate - Indexed;     Ex: MOV #S, 0x0(Rd)   */
    /* Immediate - Symbolic;    Ex: MOV #S, 0xD       */
    /* Immediate - Absolute;    Ex: MOV #S, &0xD      */
    /* Constant Gen - Indexed;  Ex: MOV #C, 0x0(Rd)   */
    /* Constant Gen - Symbolic; Ex: MOV #C, 0xD       */
    /* Constant Gen - Absolute; Ex: MOV #C, &0xD      */
    else if (as_flag == 3 && ad_flag == 1) {
      uint16_t source_value;

      if (constant_generator_active) {   /* Source Constant */
	source_value = immediate_constant;
	printf("#0x%04X, ", (uint16_t)source_value);
      }
      else if (source == 0) {            /* Source Immediate */
	source_value = fetch();
	printf("#0x%04X, ", (uint16_t)source_value);
      }
      else {                             /* Source Indirect Auto Increment */
	source_value = *get_addr_ptr(*s_reg); 

	printf("@%s+, ", s_reg_name);	
	bw_flag == WORD ? *s_reg += 2 : (*s_reg += 1);
      }

      int16_t destination_offset = fetch();
      uint16_t *destination_addr; 

      if (destination == 0) {        /* Destination Symbolic */
	uint16_t virtual_addr = PC + destination_offset - 2;
	
	destination_addr = get_addr_ptr(virtual_addr);
	printf("0x%04X\n", virtual_addr);
      }
      else if (destination == 2) {   /* Destination Absolute */
	destination_addr = get_addr_ptr(destination_offset);
	printf("&0x%04X\n", (uint16_t) destination_offset);
      }
      else {                         /* Destination Indexed */
	destination_addr = get_addr_ptr(*d_reg + destination_offset);
	printf("0x%04X(%s)\n", (uint16_t) destination_offset, d_reg_name);
      }

      if (bw_flag == WORD) {
	*destination_addr = source_value;
      }
      else if (bw_flag == BYTE) {
	*((uint8_t *)destination_addr) = (uint8_t) source_value;
      }

      break;
    }
  }
 
  /* ADD SOURCE, DESTINATION 
   *   Ex: ADD R5, R4
   * 
   * The source operand is added to the destination operand. The source operand
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

    /* Register - Register;     Ex: ADD Rs, Rd */
    /* Constant Gen - Register; Ex: ADD #C, Rd */    
    if (as_flag == 0 && ad_flag == 0) {   
      
    }

    /* Register - Indexed;      Ex: ADD Rs, 0x0(Rd) */
    /* Register - Symbolic;     Ex: ADD Rs, 0xD     */
    /* Register - Absolute;     Ex: ADD Rs, &0xD    */
    /* Constant Gen - Indexed;  Ex: ADD #C, 0x0(Rd) */
    /* Constant Gen - Symbolic; Ex: ADD #C, 0xD     */
    /* Constant Gen - Absolute; Ex: ADD #C, &0xD    */
    else if (as_flag == 0 && ad_flag == 1) {   
    
    }

    /* Indexed - Register;      Ex: ADD 0x0(Rs), Rd */
    /* Symbolic - Register;     Ex: ADD 0xS, Rd     */
    /* Absolute - Register;     Ex: ADD &0xS, Rd    */
    /* Constant Gen - Register; Ex: ADD #C, Rd      */
    else if (as_flag == 1 && ad_flag == 0) {  

    }

    /* Indexed - Indexed;       Ex: ADD 0x0(Rs), 0x0(Rd) */
    /* Symbolic - Indexed;      Ex: ADD 0xS, 0x0(Rd)     */
    /* Indexed - Symbolic;      Ex: ADD 0x0(Rd), 0xD     */
    /* Symbolic - Symbolic;     Ex: ADD 0xS, 0xD         */
    /* Absolute - Indexed;      Ex: ADD &0xS, 0x0(Rd)    */
    /* Indexed - Absolute;      Ex: ADD 0x0(Rs), &0xD    */
    /* Absolute - Absolute;     Ex: ADD &0xS, &0xD       */
    /* Absolute - Symbolic;     Ex: ADD &0xS, 0xD        */
    /* Symbolic - Absolute;     Ex: ADD 0xS, &0xD        */
    /* Constant Gen - Indexed;  Ex: ADD #C, 0x0(Rd)      */
    /* Constant Gen - Symbolic; Ex: ADD #C, 0xD          */
    /* Constant Gen - Absolute; Ex: ADD #C, &0xD         */
    else if (as_flag == 1 && ad_flag == 1) {  
      
    }
    
    /* Indirect - Register; Ex: ADD @Rs, Rd     */
    /* Constant Gen - Register; Ex: ADD #C, Rd  */
    else if (as_flag == 2 && ad_flag == 0) {   
    
    }

    /* Indirect - Indexed;      Ex: ADD @Rs, 0x0(Rd)   */
    /* Indirect - Symbolic;     Ex: ADD @Rs, 0xD       */
    /* Indirect - Absolute;     Ex: ADD @Rs, &0xD      */
    /* Constant Gen - Indexed;  Ex: ADD #C, 0x0(Rd)    */
    /* Constant Gen - Symbolic; Ex: ADD #C, 0xD        */
    /* Constant Gen - Absolute; Ex: ADD #C, &0xD       */
    else if (as_flag == 2 && ad_flag == 1) {   
      
    }
    
    /* Indirect Inc - Register; Ex: ADD @Rs+, Rd */
    /* Immediate - Register;    Ex: ADD #S, Rd   */
    /* Constant Gen - Register; Ex: ADD #C, Rd   */
    else if (as_flag == 3 && ad_flag == 0) {   
      
    }
    
    /* Indirect Inc - Indexed;  Ex: ADD @Rs+, 0x0(Rd) */
    /* Indirect Inc - Symbolic; Ex: ADD @Rs+, 0xD     */
    /* Indirect Inc - Absolute; Ex: ADD @Rs+, &0xD    */
    /* Immediate - Indexed;     Ex: ADD #S, 0x0(Rd)   */
    /* Immediate - Symbolic;    Ex: ADD #S, 0xD       */
    /* Immediate - Absolute;    Ex: ADD #S, &0xD      */
    /* Constant Gen - Indexed;  Ex: ADD #C, 0x0(Rd)   */
    /* Constant Gen - Symbolic; Ex: ADD #C, 0xD       */
    /* Constant Gen - Absolute; Ex: ADD #C, &0xD      */
    else if (as_flag == 3 && ad_flag == 1) {
      
    }

    break;
  }

  /* ADDC SOURCE, DESTINATION 
   *   Ex: ADDC R5, R4
   *    
   *
   * DESTINATION += (SOURCE + C)
   *   
   
   
   *
  */
  case 0x6:{
    bw_flag == 0 ? printf("ADDC ") : printf("ADDC.B ");

    /* Register - Register;     Ex: ADDC Rs, Rd */
    /* Constant Gen - Register; Ex: ADDC #C, Rd */    
    if (as_flag == 0 && ad_flag == 0) {   
      
    }

    /* Register - Indexed;      Ex: ADDC Rs, 0x0(Rd) */
    /* Register - Symbolic;     Ex: ADDC Rs, 0xD     */
    /* Register - Absolute;     Ex: ADDC Rs, &0xD    */
    /* Constant Gen - Indexed;  Ex: ADDC #C, 0x0(Rd) */
    /* Constant Gen - Symbolic; Ex: ADDC #C, 0xD     */
    /* Constant Gen - Absolute; Ex: ADDC #C, &0xD    */
    else if (as_flag == 0 && ad_flag == 1) {   
    
    }

    /* Indexed - Register;      Ex: ADDC 0x0(Rs), Rd */
    /* Symbolic - Register;     Ex: ADDC 0xS, Rd     */
    /* Absolute - Register;     Ex: ADDC &0xS, Rd    */
    /* Constant Gen - Register; Ex: ADDC #C, Rd      */
    else if (as_flag == 1 && ad_flag == 0) {  

    }

    /* Indexed - Indexed;       Ex: ADDC 0x0(Rs), 0x0(Rd) */
    /* Symbolic - Indexed;      Ex: ADDC 0xS, 0x0(Rd)     */
    /* Indexed - Symbolic;      Ex: ADDC 0x0(Rd), 0xD     */
    /* Symbolic - Symbolic;     Ex: ADDC 0xS, 0xD         */
    /* Absolute - Indexed;      Ex: ADDC &0xS, 0x0(Rd)    */
    /* Indexed - Absolute;      Ex: ADDC 0x0(Rs), &0xD    */
    /* Absolute - Absolute;     Ex: ADDC &0xS, &0xD       */
    /* Absolute - Symbolic;     Ex: ADDC &0xS, 0xD        */
    /* Symbolic - Absolute;     Ex: ADDC 0xS, &0xD        */
    /* Constant Gen - Indexed;  Ex: ADDC #C, 0x0(Rd)      */
    /* Constant Gen - Symbolic; Ex: ADDC #C, 0xD          */
    /* Constant Gen - Absolute; Ex: ADDC #C, &0xD         */
    else if (as_flag == 1 && ad_flag == 1) {  
      
    }
    
    /* Indirect - Register; Ex: ADDC @Rs, Rd     */
    /* Constant Gen - Register; Ex: ADDC #C, Rd  */
    else if (as_flag == 2 && ad_flag == 0) {   
    
    }

    /* Indirect - Indexed;  Ex: ADDC @Rs, 0x0(Rd)       */
    /* Indirect - Symbolic; Ex: ADDC @Rs, 0xD           */
    /* Indirect - Absolute; Ex: ADDC @Rs, &0xD          */
    /* Constant Gen - Indexed;  Ex: ADDC #C, 0x0(Rd)    */
    /* Constant Gen - Symbolic; Ex: ADDC #C, 0xD        */
    /* Constant Gen - Absolute; Ex: ADDC #C, &0xD       */
    else if (as_flag == 2 && ad_flag == 1) {   
      
    }
    
    /* Indirect Inc - Register; Ex: ADDC @Rs+, Rd */
    /* Immediate - Register;    Ex: ADDC #S, Rd   */
    /* Constant Gen - Register; Ex: ADDC #C, Rd   */
    else if (as_flag == 3 && ad_flag == 0) {   
      
    }
    
    /* Indirect Inc - Indexed;  Ex: ADDC @Rs+, 0x0(Rd) */
    /* Indirect Inc - Symbolic; Ex: ADDC @Rs+, 0xD     */
    /* Indirect Inc - Absolute; Ex: ADDC @Rs+, &0xD    */
    /* Immediate - Indexed;     Ex: ADDC #S, 0x0(Rd)   */
    /* Immediate - Symbolic;    Ex: ADDC #S, 0xD       */
    /* Immediate - Absolute;    Ex: ADDC #S, &0xD      */
    /* Constant Gen - Indexed;  Ex: ADDC #C, 0x0(Rd)   */
    /* Constant Gen - Symbolic; Ex: ADDC #C, 0xD       */
    /* Constant Gen - Absolute; Ex: ADDC #C, &0xD      */
    else if (as_flag == 3 && ad_flag == 1) {
      
    }    

    break;
  }
    //# SUBC Sub w/carry dst -= (src+C)
  case 0x7:{
    bw_flag == 0 ? printf("SUBC ") : printf("SUBC.B ");

    /* Register - Register;     Ex: SUBC Rs, Rd */
    /* Constant Gen - Register; Ex: SUBC #C, Rd */    
    if (as_flag == 0 && ad_flag == 0) {   
      
    }

    /* Register - Indexed;      Ex: SUBC Rs, 0x0(Rd) */
    /* Register - Symbolic;     Ex: SUBC Rs, 0xD     */
    /* Register - Absolute;     Ex: SUBC Rs, &0xD    */
    /* Constant Gen - Indexed;  Ex: SUBC #C, 0x0(Rd) */
    /* Constant Gen - Symbolic; Ex: SUBC #C, 0xD     */
    /* Constant Gen - Absolute; Ex: SUBC #C, &0xD    */
    else if (as_flag == 0 && ad_flag == 1) {   
    
    }

    /* Indexed - Register;      Ex: SUBC 0x0(Rs), Rd */
    /* Symbolic - Register;     Ex: SUBC 0xS, Rd     */
    /* Absolute - Register;     Ex: SUBC &0xS, Rd    */
    /* Constant Gen - Register; Ex: SUBC #C, Rd      */
    else if (as_flag == 1 && ad_flag == 0) {  

    }

    /* Indexed - Indexed;       Ex: SUBC 0x0(Rs), 0x0(Rd) */
    /* Symbolic - Indexed;      Ex: SUBC 0xS, 0x0(Rd)     */
    /* Indexed - Symbolic;      Ex: SUBC 0x0(Rd), 0xD     */
    /* Symbolic - Symbolic;     Ex: SUBC 0xS, 0xD         */
    /* Absolute - Indexed;      Ex: SUBC &0xS, 0x0(Rd)    */
    /* Indexed - Absolute;      Ex: SUBC 0x0(Rs), &0xD    */
    /* Absolute - Absolute;     Ex: SUBC &0xS, &0xD       */
    /* Absolute - Symbolic;     Ex: SUBC &0xS, 0xD        */
    /* Symbolic - Absolute;     Ex: SUBC 0xS, &0xD        */
    /* Constant Gen - Indexed;  Ex: SUBC #C, 0x0(Rd)      */
    /* Constant Gen - Symbolic; Ex: SUBC #C, 0xD          */
    /* Constant Gen - Absolute; Ex: SUBC #C, &0xD         */
    else if (as_flag == 1 && ad_flag == 1) {  
      
    }
    
    /* Indirect - Register; Ex: SUBC @Rs, Rd */
    /* Constant Gen - Register; Ex: SUBC #C, Rd  */
    else if (as_flag == 2 && ad_flag == 0) {   
    
    }

    /* Indirect - Indexed;      Ex: SUBC @Rs, 0x0(Rd)   */
    /* Indirect - Symbolic;     Ex: SUBC @Rs, 0xD       */
    /* Indirect - Absolute;     Ex: SUBC @Rs, &0xD      */
    /* Constant Gen - Indexed;  Ex: SUBC #C, 0x0(Rd)    */
    /* Constant Gen - Symbolic; Ex: SUBC #C, 0xD        */
    /* Constant Gen - Absolute; Ex: SUBC #C, &0xD       */
    else if (as_flag == 2 && ad_flag == 1) {   
      
    }
    
    /* Indirect Inc - Register; Ex: SUBC @Rs+, Rd */
    /* Immediate - Register;    Ex: SUBC #S, Rd   */
    /* Constant Gen - Register; Ex: SUBC #C, Rd   */
    else if (as_flag == 3 && ad_flag == 0) {   
      
    }
    
    /* Indirect Inc - Indexed;  Ex: SUBC @Rs+, 0x0(Rd) */
    /* Indirect Inc - Symbolic; Ex: SUBC @Rs+, 0xD     */
    /* Indirect Inc - Absolute; Ex: SUBC @Rs+, &0xD    */
    /* Immediate - Indexed;     Ex: SUBC #S, 0x0(Rd)   */
    /* Immediate - Symbolic;    Ex: SUBC #S, 0xD       */
    /* Immediate - Absolute;    Ex: SUBC #S, &0xD      */
    /* Constant Gen - Indexed;  Ex: SUBC #C, 0x0(Rd)   */
    /* Constant Gen - Symbolic; Ex: SUBC #C, 0xD       */
    /* Constant Gen - Absolute; Ex: SUBC #C, &0xD      */
    else if (as_flag == 3 && ad_flag == 1) {
      
    }    

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
