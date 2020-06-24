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

//##########+++ Decode Format III Instructions +++#########
//# Format III are jump instructions of the form:
//#   [001C][CCXX][XXXX][XXXX]
//#
//# Where C = Condition, X = 10-bit signed offset
//#
//########################################################

#include "decoder.h"
#include "../../debugger/io.h"

void decode_formatIII(Emulator *emu, uint16_t instruction, bool disassemble)
{
  Cpu *cpu = emu->cpu;
  Debugger *debugger = emu->debugger;

  uint8_t condition = (instruction & 0x1C00) >> 10;
  int16_t signed_offset = (instruction & 0x03FF);
  bool negative = signed_offset >> 9;

  char value[20];

  char mnemonic[100] = {0};
  /* String to show hex value of instruction */
  char hex_str[100] = {0};

  sprintf(hex_str, "%04hX", instruction);

  if (negative) { /* Sign Extend for Arithmetic Operations */
    signed_offset |= 0xFC00;
  }

  signed_offset *= 2;

  if (!disassemble) {
  switch(condition){

  /* JNE/JNZ Jump if not equal/zero
  *
  * If Z = 0: PC + 2 offset → PC
  * If Z = 1: execute following instruction
  */
  case 0x0:{
    const Status_reg fields = get_sr_fields(emu);
    if (fields.zero == false) {
      cpu->pc += signed_offset;
    }

    break;
  }

  /* JEQ/JZ Jump is equal/zero
   * If Z = 1: PC + 2 offset → PC
   * If Z = 0: execute following instruction
  */
  case 0x1:{
    const Status_reg fields = get_sr_fields(emu);
    if (fields.zero == true) {
      cpu->pc += signed_offset;
    }

    break;
  }

  /* JNC/JLO Jump if no carry/lower
  *
  *  if C = 0: PC + 2 offset → PC
  *  if C = 1: execute following instruction
  */
  case 0x2:{
    const Status_reg fields = get_sr_fields(emu);
    if (fields.carry == false) {
      cpu->pc += signed_offset;
    }

    break;
  }

  /* JC/JHS Jump if carry/higher or same
  *
  * If C = 1: PC + 2 offset → PC
  * If C = 0: execute following instruction
  */
  case 0x3:{
    const Status_reg fields = get_sr_fields(emu);
    if (fields.carry == true) {
      cpu->pc += signed_offset;
    }

    break;
  }

  /* JN Jump if negative
  *
  *  if N = 1: PC + 2 ×offset → PC
  *  if N = 0: execute following instruction
  */
  case 0x4:{
    const Status_reg fields = get_sr_fields(emu);
    if (fields.negative == true) {
      cpu->pc += signed_offset;
    }

    break;
  }

  /* JGE Jump if greater or equal (N == V)
  *
  *  If (N .XOR. V) = 0 then jump to label: PC + 2 P offset → PC
  *  If (N .XOR. V) = 1 then execute the following instruction
  */
  case 0x5:{
    const Status_reg fields = get_sr_fields(emu);
    if ((fields.negative ^ fields.overflow) == false) {
      cpu->pc += signed_offset;
    }

    break;
  }

  /* JL Jump if less (N != V)
  *
  *  If (N .XOR. V) = 1 then jump to label: PC + 2 offset → PC
  *  If (N .XOR. V) = 0 then execute following instruction
  */
  case 0x6:{
    const Status_reg fields = get_sr_fields(emu);
    if ((fields.negative ^ fields.overflow) == true) {
      cpu->pc += signed_offset;
    }

    break;
  }

  /* JMP Jump Unconditionally
   *
   *  PC + 2 × offset → PC
   *
   */
  case 0x7:{
    cpu->pc += signed_offset;
    break;
  }

  default:{
    puts("Undefined Jump operation!\n");
    return;
  }

  } //# End of Switch
  } //# end if


  else {
    switch(condition){

    case 0x0:{
      sprintf(mnemonic, "JNZ");
      sprintf(value, "0x%04hX", cpu->pc + signed_offset);
      break;
    }
    case 0x1:{
      sprintf(mnemonic, "JZ");
      sprintf(value, "0x%04hX", cpu->pc + signed_offset);
      break;
    }
    case 0x2:{
      sprintf(mnemonic, "JNC");
      sprintf(value, "0x%04hX", cpu->pc + signed_offset);
      break;
    }
    case 0x3:{
      sprintf(mnemonic, "JC");
      sprintf(value, "0x%04hX", cpu->pc + signed_offset);
      break;
    }
    case 0x4:{
      sprintf(mnemonic, "JN");
      sprintf(value, "0x%04hX", cpu->pc + signed_offset);
      break;
    }
    case 0x5:{
      sprintf(mnemonic, "JGE");
      sprintf(value, "0x%04hX", cpu->pc + signed_offset);

      break;
    }
    case 0x6:{
      sprintf(mnemonic, "JL");
      sprintf(value, "0x%04hX", cpu->pc + signed_offset);

      break;
    }
    case 0x7:{
      sprintf(mnemonic, "JMP");
      sprintf(value, "0x%04hX", cpu->pc + signed_offset);
      break;
    }
    default:{
      puts("Undefined Jump operation!\n");
      return;
    }

    } //# End of Switch

    // Changed from strincat(mnemonic, X, sizeof mnemonic)
    // the previous form produced warnings
    // and made no sense anyway, as DST must be larger than num
    strcat(mnemonic, "\t");
    strcat(mnemonic, value);
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
}
