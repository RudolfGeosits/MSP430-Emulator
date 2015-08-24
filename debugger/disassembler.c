void disassemble(Cpu *cpu, uint16_t start_addr, uint8_t times){
  uint16_t saved_pc = cpu->pc, opcode;
  uint32_t i;

  disassemble_mode = true;
  cpu->pc = start_addr;
  
  for (i = 0;i < times;i++) {    
    printf("0x%04X:\t", cpu->pc);

    opcode = fetch(cpu);    
    decode(cpu, opcode, DISASSEMBLE);

    fflush(stdout);
  }

  disassemble_mode = false;
  cpu->pc = saved_pc; /* Restore PC */
}
