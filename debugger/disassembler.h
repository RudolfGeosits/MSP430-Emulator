#ifndef _DISASSEMBLER_H_
#define _DISASSEMBLER_H_

void disassemble(Cpu *cpu, uint16_t start_addr, uint8_t times);

#include "disassembler.c"
#endif
