void disassemble(uint8_t times, bool info){
  uint16_t saved_pc = PC;
  uint32_t i;

  disassemble_mode = true;

  for (i = 0;i < times;i++) {
    decode( fetch() );
  }

  disassemble_mode = false;
  PC = saved_pc; /* Restore PC */
}
