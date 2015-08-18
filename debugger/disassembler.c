void disassemble(Cpu *cpu, uint8_t times, bool info){
  uint16_t saved_pc = cpu->pc, opcode;
  uint32_t i;

  disassemble_mode = true;

  for (i = 0;i < times;i++) {    
    printf("0x%04X:\t", cpu->pc);

    opcode = fetch(cpu);
    //printf("%04X\t", opcode);
    
    decode(cpu, opcode);

    fflush(stdout);
  }

  disassemble_mode = false;
  cpu->pc = saved_pc; /* Restore PC */
}
