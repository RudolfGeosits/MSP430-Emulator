//##########+++ Decode Format I Instructions +++##########
//# Format I are single operand of the form:
//#   [0001][00CC][CBAA][SSSS]
//# 
//# Where C = Opcode, B = Byte/Word flag, 
//#       A = Addressing mode for source
//#       S = Source 
//########################################################
void decode_formatI( uint16_t instruction )
{
  char reg_name[10];
  uint8_t opcode = ( instruction & 0x0380 ) >> 7;
  uint8_t bw_flag = ( instruction & 0x0040 ) >> 6;
  uint8_t as_flag = ( instruction & 0x0030 ) >> 4;
  uint8_t source_reg = ( instruction & 0x000F );
  
  reg_num_to_name(source_reg, reg_name);
  uint16_t *reg = get_reg_ptr(source_reg);
  
  /* Spot CG1 and CG2 Constant generator instructions */
  if (source_reg == 2 && as_flag > 0) {
    printf("CG1 using %%r2\n");
  }
  else if (source_reg == 3) {
    printf("CG2 using %%r3\n");
  }

  switch (opcode) {
    
  //# RRC Rotate right through carry      
  case 0x0:{
    bw_flag == WORD ? printf("RRC ") : printf("RRC.B ");
    
    uint8_t CF = SR.carry;

    if (as_flag == 0x0) {   /* RRC Rn */

      if (bw_flag == WORD) {
        SR.carry = *reg & 0x0001;   /* Set Carry flag from LSB */
        *reg >>= 1;                 /* Shift register one right */
        CF ? *reg |= 0x8000 : 0;    /* Set MSB from previous Carry flag */

        *reg == 0 ? SR.zero = 1 : (SR.zero = 0);
        *reg < 0 ? SR.negative = 1 : (SR.negative = 0);
      }
      else if (bw_flag == BYTE) {
        uint8_t low_byte = *reg & 0x00FF;

        SR.carry = low_byte & 0x01; /* Set Carry flag from LSB */
        low_byte >>= 1;             /* Shift register's low byte one right */
        CF ? low_byte |= 0x80 : 0;  /* Set MSB of low byte from previous CF */
        
        *reg = 0;
        *reg |= low_byte;           /* Set low byte of register */

        low_byte == 0 ? SR.zero = 1 : (SR.zero = 0);
        low_byte < 0 ? SR.negative = 1 : (SR.negative = 0);
      }

    }

    else if (as_flag == 0x1) {   /* RRC 0x0(Rn) */
      int16_t source_offset = fetch();
      printf("0x%04X(%s)\n", (uint16_t)source_offset, reg_name);

      if (bw_flag == WORD) {
        uint16_t *address = (uint16_t *)
          ( (void *)MEMSPACE + *reg+source_offset );
        
        uint16_t val = *address;
        
        SR.carry = val & 0x0001;    /* Set Carry flag from LSB */
        val >>= 1;                  /* Shift mem value one right */
        CF ? val |= 0x8000 : 0;     /* Set MSB from previous Carry flag */

        val == 0 ? SR.zero = 1 : (SR.zero = 0);
        val < 0 ? SR.negative = 1 : (SR.negative = 0);
 
        *address = val;            /* Set adjusted value back in mem loc */ 
      }
      else if (bw_flag == BYTE) {
        uint8_t *address = (uint8_t *)
          ( (void *)MEMSPACE + *reg + source_offset ); 
        
        uint8_t val = *address;

        printf("Address is %p, value is now 0x%04X\n", address, *address);

        SR.carry = val & 0x01;    /* Set Carry flag from LSB */
        val >>= 1;                /* Shift mem value one right */
        CF ? val |= 0x80 : 0;     /* Set MSB from previous Carry flag */

        val == 0 ? SR.zero = 1 : (SR.zero = 0);
        val < 0 ? SR.negative = 1 : (SR.negative = 0);
 
        *address = 0;
        *address = val;            /* Set adjusted value back in mem loc */ 

        printf("Address is %p, value is 0x%04X\n", address, *address);
      }
    }

    else if (as_flag == 0x2) {   /* RRC @Rn */
      printf("@%s\n", reg_name);

      if (bw_flag == WORD) {
        uint16_t *address = (uint16_t *) ( (void *)MEMSPACE + *reg );
        uint16_t val = *address;
        
        SR.carry = val & 0x0001;    /* Set Carry flag from LSB */
        val >>= 1;                  /* Shift mem value one right */
        CF ? val |= 0x8000 : 0;     /* Set MSB from previous Carry flag */

        val == 0 ? SR.zero = 1 : (SR.zero = 0);
        val < 0 ? SR.negative = 1 : (SR.negative = 0);
 
        *address = val;            /* Set adjusted value back in mem loc */ 
      }
      else if (bw_flag == BYTE) {
        uint8_t *address = (uint8_t *) ( (void *)MEMSPACE + *reg );
        uint8_t val = *address;

        SR.carry = val & 0x01;    /* Set Carry flag from LSB */
        val >>= 1;                /* Shift mem value one right */
        CF ? val |= 0x80 : 0;     /* Set MSB from previous Carry flag */

        val == 0 ? SR.zero = 1 : (SR.zero = 0);
        val < 0 ? SR.negative = 1 : (SR.negative = 0);
 
        *address = 0;
        *address = val;            /* Set adjusted value back in mem loc */ 
      }
    }

    else if (as_flag == 0x3) {   /* RRC @Rn+ */
      printf("@%s+\n", reg_name);
      
      if (bw_flag == WORD) {
        uint16_t *address = (uint16_t *) ( (void *)MEMSPACE + *reg );
        uint16_t val = *address;
        
        SR.carry = val & 0x0001;    /* Set Carry flag from LSB */
        val >>= 1;                  /* Shift mem value one right */
        CF ? val |= 0x8000 : 0;     /* Set MSB from previous Carry flag */

        val == 0 ? SR.zero = 1 : (SR.zero = 0);
        val < 0 ? SR.negative = 1 : (SR.negative = 0);
 
        *address = val;            /* Set adjusted value back in mem loc */ 
        *reg += 2;                 /* Increment register value by 2 */
      }
      else if (bw_flag == BYTE) {
        uint8_t *address = (uint8_t *) ( (void *)MEMSPACE + *reg );
        uint8_t val = *address;

        printf("Address is %p, value is 0x%04X\n", address, *address);

        SR.carry = val & 0x01;    /* Set Carry flag from LSB */
        val >>= 1;                /* Shift mem value one right */
        CF ? val |= 0x80 : 0;     /* Set MSB from previous Carry flag */

        val == 0 ? SR.zero = 1 : (SR.zero = 0);
        val < 0 ? SR.negative = 1 : (SR.negative = 0);
 
        *address = 0;
        *address = val;            /* Set adjusted value back in mem loc */ 
        *reg += 1;                 /* Increment register value by 1 */
        printf("Address is %p, value is now 0x%04X\n", address, *address);
      }
    }
    
    SR.overflow = 0;             /* Reset Overflow flag */

    break;
  }
  
  //# SWPB Swap bytes
  //# bw flag always 0 (word)
  case 0x1:{

    //# Direct register SWPB Rn
    if (as_flag == 0x0) {
      printf("SWPB %s\n", reg_name);
      
      uint8_t new_low_byte = (*reg & 0xFF00) >> 8;   /* Swapped byte values */ 
      uint8_t new_high_byte = (*reg & 0x00FF);
     
      *reg = ( ((uint16_t)0 | new_high_byte) << 8 ) | new_low_byte;
    }

    //# Indexed register SWPB 0x0(Rn)
    else if (as_flag == 0x1) {
      int16_t source_offset = fetch();
      printf("SWPB 0x%04X(%s)\n", source_offset, reg_name);
    
      uint16_t *address = (uint16_t *) 
        ( (void *)MEMSPACE + *reg + source_offset );
      
      uint8_t new_low_byte = (*address & 0xFF00) >> 8;
      uint8_t new_high_byte = (*address & 0x00FF);

      *address = ( ((uint16_t)0 | new_high_byte) << 8 ) | new_low_byte;
    }

    //# indirect register SWPB @Rn
    else if (as_flag == 0x2) {
      printf("SWPB @%s\n", reg_name);

      uint16_t *address = (uint16_t *) ((void *)MEMSPACE + *reg);
      
      uint8_t new_low_byte = (*address & 0xFF00) >> 8;
      uint8_t new_high_byte = (*address & 0x00FF);

      *address = ( ((uint16_t)0 | new_high_byte) << 8 ) | new_low_byte;
    }

    //# Indirect register autoincrement SWPB @Rn+
    else if (as_flag == 0x3) {
      printf("SWPB @%s+\n", reg_name);

      uint16_t *address = (uint16_t *) ((void *)MEMSPACE + *reg);
      
      uint8_t new_low_byte = (*address & 0xFF00) >> 8;
      uint8_t new_high_byte = (*address & 0x00FF);

      *address = ( ((uint16_t)0 | new_high_byte) << 8 ) | new_low_byte;
      *reg += 2;
    }

    break;
  }
  
  //# RRA Rotate right arithmetic 
  case 0x2:{

    bw_flag == 0 ? printf("RRA ") : printf("RRA.B ");

    if (as_flag == 0x0) {        /* RRA Rn */
      printf("%s\n", reg_name);

      if (bw_flag == WORD) {
        uint16_t msb;
        
        0x8000 & *reg ? msb = 0x8000 : (msb = 0);
        0x0001 & *reg ? SR.carry = 1 : (SR.carry = 0);

        *reg >>= 1;
        *reg |= msb;

        (int16_t)*reg < 0 ? SR.negative = 1 : (SR.negative = 0);
        *reg == 0 ? SR.zero = 1 : (SR.zero = 0);
      }
      else if (bw_flag == BYTE) { 
        uint8_t msb, low_byte = *reg & 0x00FF;

        0x80 & low_byte ? msb = 0x80 : (msb = 0);
        0x01 & low_byte ? SR.carry = 1 : (SR.carry = 0);

        low_byte >>= 1;
        low_byte |= msb;

        (int8_t)low_byte < 0 ? SR.negative = 1 : (SR.negative = 0);
        low_byte == 0 ? SR.zero = 1 : (SR.zero = 0);

        *reg = 0;
        *reg |= low_byte;
      }

    }
    else if (as_flag == 0x1) {   /* RRA 0x0(Rn) */
      int16_t source_offset = fetch();
      printf("0x%04X(%s)\n", source_offset, reg_name);

      if (bw_flag == WORD) {
        uint16_t *address = (uint16_t *) 
          ((void *) MEMSPACE + *reg + source_offset);
        
        uint16_t msb;
        0x8000 & *address ? msb = 0x8000 : (msb = 0);
        0x0001 & *address ? SR.carry = 1 : (SR.carry = 0);

        *address >>= 1;
        *address |= msb;

        (int16_t)*address < 0 ? SR.negative = 1 : (SR.negative = 0);
        *address == 0 ? SR.zero = 1 : (SR.zero = 0);
      }
      else if (bw_flag == BYTE) {
        uint8_t *address = (uint8_t *) 
          ((void *) MEMSPACE + *reg + source_offset);
        
        uint8_t msb, low_byte = *address & 0x00FF;

        0x80 & low_byte ? msb = 0x80 : (msb = 0);
        0x01 & low_byte ? SR.carry = 1 : (SR.carry = 0);

        low_byte >>= 1;
        low_byte |= msb;

        (int8_t)low_byte < 0 ? SR.negative = 1 : (SR.negative = 0);
        low_byte == 0 ? SR.zero = 1 : (SR.zero = 0);

        *address = 0;
        *address |= low_byte;
      }
    }
    else if (as_flag == 0x2) {   /* RRA @Rn */
      printf("@%s\n", reg_name);

      if (bw_flag == WORD) {
        uint16_t *address = (uint16_t *) ((void *) MEMSPACE + *reg);
        uint16_t msb;

        0x8000 & *address ? msb = 0x8000 : (msb = 0);
        0x0001 & *address ? SR.carry = 1 : (SR.carry = 0);

        *address >>= 1;
        *address |= msb;

        (int16_t)*address < 0 ? SR.negative = 1 : (SR.negative = 0);
        *address == 0 ? SR.zero = 1 : (SR.zero = 0);
      }
      else if (bw_flag == BYTE) {
        uint8_t *address = (uint8_t *) ((void *) MEMSPACE + *reg);
        uint8_t msb, low_byte = *address & 0x00FF;

        0x80 & low_byte ? msb = 0x80 : (msb = 0);
        0x01 & low_byte ? SR.carry = 1 : (SR.carry = 0);

        low_byte >>= 1;
        low_byte |= msb;

        (int8_t)low_byte < 0 ? SR.negative = 1 : (SR.negative = 0);
        low_byte == 0 ? SR.zero = 1 : (SR.zero = 0);

        *address = 0;
        *address |= low_byte;
      }
    }
    else if (as_flag == 0x3) {   /* RRA @Rn+ */
      printf("@%s+\n", reg_name);

      if (bw_flag == WORD) {
        uint16_t *address = (uint16_t *) ((void *) MEMSPACE + *reg);
        uint16_t msb;

        0x8000 & *address ? msb = 0x8000 : (msb = 0);
        0x0001 & *address ? SR.carry = 1 : (SR.carry = 0);

        *address >>= 1;
        *address |= msb;

        (int16_t)*address < 0 ? SR.negative = 1 : (SR.negative = 0);
        *address == 0 ? SR.zero = 1 : (SR.zero = 0);
        
        *reg += 2;
      }
      else if (bw_flag == BYTE) {
        uint8_t *address = (uint8_t *) ((void *) MEMSPACE + *reg);
        uint8_t msb, low_byte = *address & 0x00FF;

        0x80 & low_byte ? msb = 0x80 : (msb = 0);
        0x01 & low_byte ? SR.carry = 1 : (SR.carry = 0);

        low_byte >>= 1;
        low_byte |= msb;

        (int8_t)low_byte < 0 ? SR.negative = 1 : (SR.negative = 0);
        low_byte == 0 ? SR.zero = 1 : (SR.zero = 0);

        *address = 0;
        *address |= low_byte;

        *reg += 1;
      }
    }

    SR.overflow = 0;   /* Reset Overflow flag */
    break;
  }

  //# SXT Sign extend byte to word
  //# bw flag always 0 (word)
  case 0x3:{

    if(as_flag == 0x0){   /* SXT Rn */
      printf("SXT %s\n", reg_name);

      uint8_t low_byte_sign = (*reg & 0x0080) >> 7;
      low_byte_sign ? *reg |= 0xFF00 : (*reg &= 0x00FF);
      
      (int16_t)*reg < 0 ? SR.negative = 1 : (SR.negative = 0);
      *reg == 0 ? SR.zero = 1 : (SR.zero = 0, SR.carry = 1);
    }
    else if(as_flag == 0x1){   /* SXT 0x0(Rn) */
      int16_t source_offset = fetch();      
      printf("SXT 0x%04X(%s)\n", (uint16_t)source_offset, reg_name);
    
      uint16_t *address = (uint16_t *)
        ((void *) MEMSPACE + *reg + source_offset);

      uint8_t low_byte_sign = (*address & 0x0080) >> 7;
      low_byte_sign ? *address |= 0xFF00 : (*address &= 0x00FF);
      
      (int16_t)*address < 0 ? SR.negative = 1 : (SR.negative = 0);
      *address == 0 ? SR.zero = 1 : (SR.zero = 0, SR.carry = 1);
    }
    else if(as_flag == 0x2){   /* SXT @Rn */
      printf("SXT @%s\n", reg_name);

      uint16_t *address = (uint16_t *) ((void *) MEMSPACE + *reg);
      uint8_t low_byte_sign = (*address & 0x0080) >> 7;

      low_byte_sign ? *address |= 0xFF00 : (*address &= 0x00FF);      
      (int16_t)*address < 0 ? SR.negative = 1 : (SR.negative = 0);
      *address == 0 ? SR.zero = 1 : (SR.zero = 0, SR.carry = 1);
    }
    else if(as_flag == 0x3){   /* SXT @Rn+ */
      printf("SXT @%s+\n", reg_name);

      uint16_t *address = (uint16_t *) ((void *) MEMSPACE + *reg);
      uint8_t low_byte_sign = (*address & 0x0080) >> 7;

      low_byte_sign ? *address |= 0xFF00 : (*address &= 0x00FF);      
      (int16_t)*address < 0 ? SR.negative = 1 : (SR.negative = 0);
      *address == 0 ? SR.zero = 1 : (SR.zero = 0, SR.carry = 1);

      *reg += 2;
    }

    SR.overflow = 0;
    break;
  }
  
  //# PUSH push value on to the stack
  case 0x4:{

    bw_flag == 0 ? printf("PUSH ") : printf("PUSH.B ");    
    SP -= 2;   /* Decrement Stack pointer by two, always */
    
    uint16_t *stack_addr = (uint16_t *) ( (void *) MEMSPACE + SP );
      
    if(as_flag == 0x0){   /* PUSH Rn */
      printf("%s\n", reg_name);

      if (bw_flag == WORD) {
	*stack_addr = *reg;
      }
      else if (bw_flag == BYTE) {
	
      }
      
    }
    else if(as_flag == 0x1){   /* PUSH 0x0(Rn) */
      int16_t source_offset = fetch();
      printf("0x%04X(%s)\n", (uint16_t)source_offset, reg_name);


    }
    else if(as_flag == 0x2){   /* PUSH @Rn */
      printf("@%s\n", reg_name);


    }
    else if(as_flag == 0x3){   /* PUSH @Rn+ */
      printf("@%s+\n", reg_name);


    }

    break;
  }

  //# Call subroutine; push PC and move source to PC
  case 0x5:{

    printf("CALL ");

    if(as_flag == 0x0){   /* CALL Rn */
      printf("%s\n", reg_name);
    }
    else if(as_flag == 0x1){   /* CALL 0x0(Rn) */
      int16_t source_offset;

      source_offset = fetch();
      printf("0x%04X(%s)\n", (uint16_t)source_offset, reg_name);
    }
    else if(as_flag == 0x2){   /* CALL @Rn */
      printf("@%s\n", reg_name);
    }
    else if(as_flag == 0x3){   /* CALL @Rn+ */
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

