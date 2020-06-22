/*
  MSP430 Emulator
  Copyright (C) 2020 Rudolf Geosits (rgeosits@live.esu.edu)

  "MSP430 Emulator" is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  "MSP430 Emulator" is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/>.
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

#include "formatI.h"
#include "../../debugger/io.h"

void decode_formatI(Emulator *emu, uint16_t instruction, bool disassemble)
{
  Cpu *cpu = emu->cpu;
  Debugger *debugger = emu->debugger;

  uint8_t opcode = (instruction & 0xF000) >> 12;
  uint8_t source = (instruction & 0x0F00) >> 8;
  uint8_t as_flag = (instruction & 0x0030) >> 4;
  uint8_t destination = (instruction & 0x000F);
  uint8_t ad_flag = (instruction & 0x0080) >> 7;
  uint8_t bw_flag = (instruction & 0x0040) >> 6;

  char s_reg_name[10], d_reg_name[10];

  char mnemonic[100] = {0};
  /* String to show hex value of instruction */
  char hex_str[100] = {0};
  char hex_str_part[10] = {0};

  sprintf(hex_str, "%04hX", instruction);

  /* Source Register pointer */
  int16_t *s_reg = get_reg_ptr(emu, source);

  /* Destination Register pointer */
  int16_t *d_reg = get_reg_ptr(emu, destination);

  reg_num_to_name(source, s_reg_name);      /* Get source register name */
  reg_num_to_name(destination, d_reg_name); /* Get destination register name */

  uint8_t constant_generator_active = 0;    /* Specifies if CG1/CG2 active */
  int16_t immediate_constant = 0;           /* Generated Constant */

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
  int16_t destination_offset;
  uint16_t *destination_addr;
  char asm_operands[64] = {0}, asm_op2[20] = {0};

  /* Register - Register;     Ex: MOV Rs, Rd */
  /* Constant Gen - Register; Ex: MOV #C, Rd */ /* 0 */
  if (as_flag == 0 && ad_flag == 0) {
    if (constant_generator_active) {   /* Source Constant */
      source_value = immediate_constant;

      sprintf(asm_operands, "#0x%04hX, %s",
	      (uint16_t) source_value, d_reg_name);
    }
    else {                             /* Source register */
      source_value = *s_reg;
      sprintf(asm_operands, "%s, %s", s_reg_name, d_reg_name);
    }

    destination_addr = (uint16_t *)d_reg;         // Destination Register

    if (!disassemble) {
      bw_flag == BYTE ? *d_reg &= 0x00FF : 0;
    }
  }

  /* Register - Indexed;      Ex: MOV Rs, 0x0(Rd) */
  /* Register - Symbolic;     Ex: MOV Rs, 0xD     */
  /* Register - Absolute;     Ex: MOV Rs, &0xD    */
  /* Constant Gen - Indexed;  Ex: MOV #C, 0x0(Rd) */ /* 0 */
  /* Constant Gen - Symbolic; Ex: MOV #C, 0xD     */ /* 0 */
  /* Constant Gen - Absolute; Ex: MOV #C, &0xD    */ /* 0 */
  else if (as_flag == 0 && ad_flag == 1) {
    destination_offset = fetch(emu, false);

    sprintf(hex_str_part, "%04hX", (uint16_t) destination_offset);
    strncat(hex_str, hex_str_part, sizeof hex_str);

    destination_addr = get_addr_ptr(*d_reg + destination_offset);

    if (constant_generator_active) {   /* Source Constant */
      source_value = immediate_constant;
      sprintf(asm_operands, "#0x%04hX, ", source_value);
    }
    else {                             /* Source from register */
      source_value = *s_reg;
      sprintf(asm_operands, "%s, ", s_reg_name);
    }

    if (destination == 0) {            /* Destination Symbolic */
      uint16_t virtual_addr = *d_reg + destination_offset - 2;
      destination_addr = get_addr_ptr(virtual_addr);

      sprintf(asm_op2, "0x%04hX", (uint16_t) virtual_addr);
    }
    else if (destination == 2) {       /* Destination Absolute */
      destination_addr = get_addr_ptr(destination_offset);
      sprintf(asm_op2, "&0x%04hX", (uint16_t) destination_offset);
    }
    else {                             /* Destination Indexed */
      sprintf(asm_op2, "0x%04hX(%s)",
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
      sprintf(asm_operands, "#0x%04hX, %s", source_value, d_reg_name);
    }
    else if (source == 0) {            /* Source Symbolic */
      source_offset = fetch(emu, false);
      uint16_t virtual_addr = *s_reg + source_offset - 2;

      source_value = memory_read_word(get_addr_ptr(virtual_addr));


      sprintf(hex_str_part, "%04hX", (uint16_t) source_offset);
      strncat(hex_str, hex_str_part, sizeof hex_str);

      sprintf(asm_operands, "0x%04hX, %s", virtual_addr, d_reg_name);
    }
    else if (source == 2) {            /* Source Absolute */
      source_offset = fetch(emu, false);
      source_value = memory_read_word(get_addr_ptr(source_offset));

      sprintf(hex_str_part, "%04hX", (uint16_t) source_offset);
      strncat(hex_str, hex_str_part, sizeof hex_str);

      sprintf(asm_operands, "&0x%04hX, %s",
	      (uint16_t) source_offset, d_reg_name);
    }
    else {                             /* Source Indexed */
      source_offset = fetch(emu, false);
      source_value = memory_read_word(get_addr_ptr(*s_reg + source_offset));

      sprintf(hex_str_part, "%04hX", (uint16_t) source_offset);
      strncat(hex_str, hex_str_part, sizeof hex_str);

      sprintf(asm_operands, "0x%04hX(%s), %s",
	      (uint16_t) source_offset, s_reg_name, d_reg_name);
    }

    destination_addr = (uint16_t *)d_reg;          /* Destination register */

    if (!disassemble) {
      bw_flag == BYTE ? *d_reg &= 0x00FF : 0;
    }
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
      source_offset = fetch(emu, false);
      uint16_t virtual_addr = cpu->pc + source_offset - 4;

      source_value = memory_read_word(get_addr_ptr(virtual_addr));

      sprintf(hex_str_part, "%04X", (uint16_t) source_offset);
      strncat(hex_str, hex_str_part, sizeof hex_str);

      sprintf(asm_operands, "0x%04X, ", virtual_addr);
    }
    else if (source == 2) {            /* Source Absolute */
      source_offset = fetch(emu, false);
      source_value = memory_read_word(get_addr_ptr(source_offset));

      sprintf(hex_str_part, "%04X", (uint16_t) source_offset);
      strncat(hex_str, hex_str_part, sizeof hex_str);

      sprintf(asm_operands, "&0x%04X, ", (uint16_t) source_offset);
    }
    else {                             /* Source Indexed */
      source_offset = fetch(emu, false);
      source_value = memory_read_word(get_addr_ptr(*s_reg + source_offset));

      sprintf(hex_str_part, "%04X", (uint16_t)source_offset);
      strncat(hex_str, hex_str_part, sizeof hex_str);

      sprintf(asm_operands, "0x%04X(%s), ",
	      (uint16_t) source_offset, s_reg_name);
    }

    destination_offset = fetch(emu, false);

    sprintf(hex_str_part, "%04X", (uint16_t) destination_offset);
    strncat(hex_str, hex_str_part, sizeof hex_str);

    if (destination == 0) {        /* Destination Symbolic */
      uint16_t virtual_addr = cpu->pc + destination_offset - 2;

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
      source_value = memory_read_word(get_addr_ptr(*s_reg));
      sprintf(asm_operands, "@%s, %s", s_reg_name, d_reg_name);
    }

    destination_addr = (uint16_t*)d_reg;          /* Destination Register */

    if (!disassemble) {
      bw_flag == BYTE ? *d_reg &= 0x00FF : 0;
    }
  }

  /* Indirect - Indexed;      Ex: MOV @Rs, 0x0(Rd)   */
  /* Indirect - Symbolic;     Ex: MOV @Rs, 0xD       */
  /* Indirect - Absolute;     Ex: MOV @Rs, &0xD      */
  /* Constant Gen - Indexed;  Ex: MOV #C, 0x0(Rd)    */ /* 2, 4 */
  /* Constant Gen - Symbolic; Ex: MOV #C, 0xD        */ /* 2, 4 */
  /* Constant Gen - Absolute; Ex: MOV #C, &0xD       */ /* 2, 4 */
  else if (as_flag == 2 && ad_flag == 1) {
    destination_offset = fetch(emu, false);

    sprintf(hex_str_part, "%04X", (uint16_t) destination_offset);
    strncat(hex_str, hex_str_part, sizeof hex_str);

    if (constant_generator_active) {   /* Source Constant */
      source_value = immediate_constant;
      sprintf(asm_operands, "#0x%04X, ", source_value);
    }
    else {                             /* Source Indirect */
      source_value = memory_read_word(get_addr_ptr(*s_reg));
      sprintf(asm_operands, "@%s, ", s_reg_name);
    }

    if (destination == 0) {        /* Destination Symbolic */
      uint16_t virtual_addr = cpu->pc + destination_offset - 2;

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
      source_value = fetch(emu, false);

      sprintf(hex_str_part, "%04X", (uint16_t) source_value);
      strncat(hex_str, hex_str_part, sizeof hex_str);

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
      source_value = memory_read_word(get_addr_ptr(*s_reg));

      sprintf(asm_operands, "@%s+, %s", s_reg_name, d_reg_name);

      if (!disassemble) {
	      bw_flag == WORD ? *s_reg += 2 : (*s_reg += 1);
      }
    }

    destination_addr = (uint16_t*)d_reg;           /* Destination Register */

    if (!disassemble) {
      bw_flag == BYTE ? *d_reg &= 0x00FF : 0;
    }
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
      source_value = fetch(emu, false);

      sprintf(hex_str_part, "%04X", (uint16_t) source_value);
      strncat(hex_str, hex_str_part, sizeof hex_str);

      sprintf(asm_operands, "#0x%04X, ", (uint16_t)source_value);
    }
    else {                             /* Source Indirect Auto Increment */
      source_value = memory_read_word(get_addr_ptr(*s_reg));

      sprintf(asm_operands, "@%s+, ", s_reg_name);

      if (!disassemble) {
	      bw_flag == WORD ? *s_reg += 2 : (*s_reg += 1);
      }
    }

    destination_offset = fetch(emu, false);

    sprintf(hex_str_part, "%04X", (uint16_t) destination_offset);
    strncat(hex_str, hex_str_part, sizeof hex_str);

    if (destination == 0) {        /* Destination Symbolic */
      uint16_t virtual_addr = cpu->pc + destination_offset - 2;

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


  if (!disassemble) {
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

        if (bw_flag == WORD) {
          //*destination_addr = source_value;
          memory_write_word(destination_addr, source_value);
        }
        else if (bw_flag == BYTE) {
          //*((uint8_t *) destination_addr) = (uint8_t) source_value;
          memory_write_byte(destination_addr, (uint8_t)source_value);
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

        uint16_t original_dst_value = memory_read_word(destination_addr);

        if (bw_flag == WORD) {
          uint16_t x = memory_read_word(destination_addr);
          x += source_value;
          memory_write_word(destination_addr, x);
        }
        else if (bw_flag == BYTE) {
          uint8_t x = memory_read_byte(destination_addr);
          x += (uint8_t) source_value;
          memory_write_byte(destination_addr, x);
        }

        Status_reg fields = get_sr_fields(emu);

        fields.zero = is_zero(destination_addr, bw_flag);
        fields.negative = is_negative((int16_t*)destination_addr, bw_flag);
        fields.carry = is_carried(original_dst_value, source_value, bw_flag);
        fields.overflow = is_overflowed(source_value, original_dst_value,
          			  destination_addr, bw_flag);
        set_sr_from_fields(emu, fields);

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

        uint16_t original_dst_value = memory_read_word(destination_addr);
        Status_reg fields = get_sr_fields(emu);

        if (bw_flag == WORD) {
          uint16_t x = memory_read_word(destination_addr);
          x += source_value + fields.carry;
          memory_write_word(destination_addr, x);
        }
        else if (bw_flag == BYTE) {
          uint8_t x = memory_read_byte(destination_addr);
          x += (uint8_t) source_value + fields.carry;
          memory_write_byte(destination_addr, x);
        }

        fields.zero = is_zero(destination_addr, bw_flag);
        fields.negative = is_negative((int16_t*)destination_addr, bw_flag);
        fields.carry = is_carried(original_dst_value, source_value, bw_flag);
        fields.overflow = is_overflowed(source_value, original_dst_value,
          			  destination_addr, bw_flag);
        set_sr_from_fields(emu, fields);
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

        Status_reg fields = get_sr_fields(emu);
        int16_t original_dst_value = memory_read_word(destination_addr);
        source_value = ~source_value; /* 1's comp */

        if (bw_flag == WORD) {
          int16_t x = memory_read_word(destination_addr);
          x += source_value + fields.carry;
          memory_write_word(destination_addr, x);
        }
        else if (bw_flag == BYTE) {
          int8_t x = memory_read_byte(destination_addr);
          x += (int8_t) source_value + fields.carry;
          memory_write_byte(destination_addr, x);
        }

        fields.zero = is_zero(destination_addr, bw_flag);
        fields.negative = is_negative((int16_t*)destination_addr, bw_flag);
        fields.carry = is_carried(original_dst_value, source_value, bw_flag);
        fields.overflow = is_overflowed(source_value, original_dst_value,
          			  destination_addr, bw_flag);
        set_sr_from_fields(emu, fields);
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

        Status_reg fields = get_sr_fields(emu);
        const int16_t original_dst_value = memory_read_word(destination_addr);
        source_value = ~source_value + 1;

        if (bw_flag == WORD) {
          uint16_t x = memory_read_word(destination_addr);
          x += source_value;
          memory_write_word(destination_addr, x);
        }
        else if (bw_flag == BYTE) {
          uint8_t x = memory_read_byte(destination_addr);
          x += (uint8_t) source_value;
          memory_write_byte(destination_addr, x);
        }

        fields.zero = is_zero(destination_addr, bw_flag);
        fields.negative = is_negative((int16_t*)destination_addr, bw_flag);

        if ( is_carried(~source_value, 1, bw_flag) ||
             is_carried(original_dst_value, source_value, bw_flag) ) {

          fields.carry = true;
        }

        fields.overflow = is_overflowed(source_value, original_dst_value,
          			  destination_addr, bw_flag);
        set_sr_from_fields(emu, fields);
        break;
      }

      /* CMP SOURCE, DESTINATION
       *
       * N: Set if result is negative, reset if positive (src ≥ dst)
       * Z: Set if result is zero, reset otherwise (src = dst)
       * C: Set if there is a carry from the MSB of the result, reset otherwise
       * V: Set if an arithmetic overflow occurs, otherwise reset
       * TODO: Fix overflow error
       */
      case 0x9:{
        Status_reg fields = get_sr_fields(emu);
        const int16_t original_dst_value = memory_read_word(destination_addr);
        uint16_t unsigned_source_value = ((uint16_t)~source_value + 1);
        int16_t result;

        bool early_carry = is_carried((uint16_t)~source_value, 1, bw_flag);

        if (bw_flag == WORD) {
          result = memory_read_word(destination_addr) + (uint16_t) unsigned_source_value;
        }
        else if (bw_flag == BYTE) {
          result = memory_read_byte(destination_addr) + (uint8_t)unsigned_source_value;
        }

        fields.negative = is_negative(&result, bw_flag);
        fields.zero = is_zero((uint16_t*)&result, bw_flag);

        /* Check if the carry happens durring conversion to 2's comp */
        if (! early_carry) {
          fields.carry = is_carried(original_dst_value, unsigned_source_value, bw_flag);
        }
        else {
          fields.carry = true;
        }

        fields.overflow = is_overflowed(unsigned_source_value, original_dst_value, (uint16_t*)&result, bw_flag);
        set_sr_from_fields(emu, fields);
        break;
      }

      /* DADD SOURCE, DESTINATION
       *
       */
      case 0xA:{

        print_console(emu, "Unimplemented instruction = DADD\n");
        break;
      }

      /* BIT SOURCE, DESTINATION
       *
       * N: Set if MSB of result is set, reset otherwise
       * Z: Set if result is zero, reset otherwise
       * C: Set if result is not zero, reset otherwise (.NOT. Zero)
       * V: Reset
      */
      case 0xB:{
        Status_reg fields = get_sr_fields(emu);
        if (bw_flag == WORD) {
          uint16_t x = memory_read_word(destination_addr);
          uint16_t result = ((uint16_t) source_value) & x;

          fields.zero = (result == 0);
          fields.negative = result >> 15;
          fields.carry = (result != 0);
        }
        else if (bw_flag == BYTE) {
          uint8_t x = memory_read_byte(destination_addr);
          uint8_t result =
            ((uint8_t) source_value) & x;

          fields.zero = (result == 0);
          fields.negative = result >> 7;
          fields.carry = (result != 0);
        }

        fields.overflow = false;
        set_sr_from_fields(emu, fields);
        break;
      }

      /* BIC SOURCE, DESTINATION
       *
       * No status bits affected
       */
      case 0xC:{

        if (bw_flag == WORD) {
          //*destination_addr &= (uint16_t) ~source_value;
          uint16_t x = memory_read_word(destination_addr);
          const uint16_t old_x = x;
          x &= (uint16_t) ~source_value;
          //printf("BIC DST %x SRC %x -> DST %x\n", old_x, source_value, x);
          //printf("DST @ %p SR @ %p\n", destination_addr, &(emu->cpu->sr));
          memory_write_word(destination_addr, x);
        }
        else if (bw_flag == BYTE) {
          //*(uint8_t *) destination_addr &= (uint8_t) ~source_value;
          uint8_t x = memory_read_byte(destination_addr);
          x &= (uint8_t) ~source_value;
          memory_write_byte(destination_addr, x);
        }

        break;
      }

      /* BIS SOURCE, DESTINATION
       *
       */
      case 0xD:{

        if (bw_flag == WORD) {
          //*destination_addr |= (uint16_t) source_value;
          uint16_t x = memory_read_word(destination_addr);
          x |= (uint16_t) source_value;
          memory_write_word(destination_addr, x);
        }
        else if (bw_flag == BYTE) {
          //*(uint8_t *) destination_addr |= (uint8_t) source_value;
          uint8_t x = memory_read_byte(destination_addr);
          x |= (uint8_t) source_value;
          memory_write_byte(destination_addr, x);
        }

        break;
      }

      /* XOR SOURCE, DESTINATION
       *
       * N: Set if result MSB is set, reset if not set
       * Z: Set if result is zero, reset otherwise
       * C: Set if result is not zero, reset otherwise ( = .NOT. Zero)
       * V: Set if both operands are negative
       */
      case 0xE:{
        Status_reg fields = get_sr_fields(emu);
        if (bw_flag == WORD) {
          uint16_t x = memory_read_word(destination_addr);
          fields.overflow =
            (x >> 15) && ((uint16_t)source_value >> 15);

          x ^= (uint16_t)source_value;

          fields.negative = (x >> 15);
          fields.zero = (x == 0);
          fields.carry = (x != 0);
          memory_write_word(destination_addr, x);
        }
        else if (bw_flag == BYTE) {
          uint8_t x = memory_read_byte((uint8_t*)destination_addr);
          fields.overflow =
            (x >> 7) && ((uint8_t)source_value >> 7);

          x ^= (uint8_t) source_value;

          fields.negative = (x >> 7);
          fields.zero = (x == 0);
          fields.carry = (x != 0);
          set_sr_from_fields(emu, fields);
          memory_write_byte(destination_addr, x);
        }

        break;
      }

      /* AND SOURCE, DESTINATION
       *
       *  N: Set if result MSB is set, reset if not set
       *  Z: Set if result is zero, reset otherwise
       *  C: Set if result is not zero, reset otherwise ( = .NOT. Zero)
       *  V: Reset
       */
      case 0xF:{
        Status_reg fields = get_sr_fields(emu);
        if (bw_flag == WORD) {
          //*destination_addr &= (uint16_t)source_value;
          uint16_t x = memory_read_word(destination_addr);
          x &= (uint16_t)source_value;
          memory_write_word(destination_addr, x);

          fields.negative = (x >> 15);
          fields.zero = (x == 0);
          fields.carry = (x != 0);
        }
        else if (bw_flag == BYTE) {
          //*(uint8_t *) destination_addr &= (uint8_t) source_value;
          uint8_t x = memory_read_byte((uint8_t*)destination_addr);
          x &= (uint8_t) source_value;
          memory_write_byte(destination_addr, x);

          fields.negative = (x >> 7);
          fields.zero = (x == 0);
          fields.carry = (x != 0);
        }

        fields.overflow = false;
        set_sr_from_fields(emu, fields);
        break;
      }
    default:{
      print_console(emu, "Unknown Single operand instruction\n");
    }

    } //# End of switch

  } // End of if



  // DISASSEMBLY MODE
  else {
    switch (opcode) {
      case 0x4: {
	      bw_flag == WORD ?
          strncpy(mnemonic, "MOV", sizeof mnemonic) :
          strncpy(mnemonic, "MOV.B", sizeof mnemonic);

        break;
      }
      case 0x5: {
        bw_flag == WORD ?
          strncpy(mnemonic, "ADD", sizeof mnemonic) :
          strncpy(mnemonic, "ADD.B", sizeof mnemonic);

        break;
      }
      case 0x6: {
        bw_flag == WORD ?
          strncpy(mnemonic, "ADDC", sizeof mnemonic) :
          strncpy(mnemonic, "ADDC.B", sizeof mnemonic);

        break;
      }
      case 0x7: {
        bw_flag == WORD ?
          strncpy(mnemonic, "SUBC", sizeof mnemonic) :
          strncpy(mnemonic, "SUBC.B", sizeof mnemonic);

        break;
      }
      case 0x8: {
        bw_flag == WORD ?
          strncpy(mnemonic, "SUB", sizeof mnemonic) :
          strncpy(mnemonic, "SUB.B", sizeof mnemonic);

        break;
      }
      case 0x9: {
        bw_flag == WORD ?
          strncpy(mnemonic, "CMP", sizeof mnemonic) :
          strncpy(mnemonic, "CMP.B", sizeof mnemonic);

        break;
      }
      case 0xA: {
	      bw_flag == WORD ?
          strncpy(mnemonic, "DADD", sizeof mnemonic) :
          strncpy(mnemonic, "DADD.B", sizeof mnemonic);

        break;
      }
      case 0xB: {
        bw_flag == WORD ?
          strncpy(mnemonic, "BIT", sizeof mnemonic) :
          strncpy(mnemonic, "BIT.B", sizeof mnemonic);

        break;
      }
      case 0xC: {
        bw_flag == WORD ?
          strncpy(mnemonic, "BIC", sizeof mnemonic) :
          strncpy(mnemonic, "BIC.B", sizeof mnemonic);

        break;
      }
      case 0xD: {
        bw_flag == WORD ?
          strncpy(mnemonic, "BIS", sizeof mnemonic) :
          strncpy(mnemonic, "BIS.B", sizeof mnemonic);

        break;
      }
      case 0xE: {
        bw_flag == WORD ?
          strncpy(mnemonic, "XOR", sizeof mnemonic) :
          strncpy(mnemonic, "XOR.B", sizeof mnemonic);

        break;
      }
      case 0xF: {
        bw_flag == WORD ?
          strncpy(mnemonic, "AND", sizeof mnemonic) :
          strncpy(mnemonic, "AND.B", sizeof mnemonic);

        break;
      }

    } //# End of switch

    // Changed from strincat(mnemonic, X, sizeof mnemonic)
    // the previous form produced warnings
    // and made no sense anyway, as DST must be larger than num
    strcat(mnemonic, "\t");
    strcat(mnemonic, asm_operands);
    strcat(mnemonic, "\n");

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

      print_console(emu, hex_str);

      for (i = strlen(hex_str);i < 12;i++) {
      	print_console(emu, " ");
      }

      print_console(emu, "\t");
      print_console(emu, mnemonic);
    }

  }
};
