//##########+++ CPU Fetch Cycle +++##########
uint16_t fetch()
{
  uint16_t word;
  word = *( (uint16_t*)(MEMSPACE + PC) );
  PC += 2;

  return word;
}

//##########+++ CPU Decode Cycle +++##########
void decode(uint16_t instruction)
{  
  int done = 0;
  uint8_t format_id;

  format_id = (uint8_t)(instruction >> 12);
  printf("Got instruction 0x%04X\n", instruction);

  if (format_id == 0x1) {
    decode_formatI(instruction);    /* format I (single operand) instruction */
  }    
  else if (format_id >= 0x2 && format_id <= 3) {
    decode_formatII(instruction);   /* format II (jump) instruction */
  }
  else if (format_id >= 0x4) {
    decode_formatIII(instruction);  /* format III (two operand) instruction */
  }
}
