void disassemble(Cpu *cpu, uint8_t times, bool info){
  uint16_t saved_pc = cpu->pc;
  uint32_t i;

  disassemble_mode = true;

  for (i = 0;i < times;i++) {
    decode(cpu, fetch(cpu) );
  }

  disassemble_mode = false;
  cpu->pc = saved_pc; /* Restore PC */
}
