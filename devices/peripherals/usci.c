void handle_usci (Cpu *cpu) {
  Usci *usci = cpu->usci;

  if (*usci->UCA0TXBUF & 0xFF) {
    printf("%c", *usci->UCA0TXBUF);
    fflush(stdout);

    *usci->UCA0TXBUF = 0;
  }
  else {
    //printf(".");
    //fflush(stdout);
  }
}

void setup_usci (Cpu *cpu) 
{
  Usci *usci = cpu->usci;

  static const uint16_t UCA0CTL0_VLOC = 0x60; // Control Register 0
  static const uint16_t UCA0CTL1_VLOC = 0x61; // Control Register 1
  static const uint16_t UCA0BR0_VLOC  = 0x62; // Baud Rate ctl Register 0
  static const uint16_t UCA0BR1_VLOC  = 0x63; // Baud Rate ctl Register 1
  static const uint16_t UCA0MCTL_VLOC = 0x64; // Modulation ctl Register
  static const uint16_t UCA0STAT_VLOC = 0x65; // Status Register
  static const uint16_t UCA0RXBUF_VLOC = 0x66; // RECV buffer register
  static const uint16_t UCA0TXBUF_VLOC = 0x67; // Transmit buffer register
  static const uint16_t UCA0ABCTL_VLOC = 0x5D; // Auto-Baud control register
  static const uint16_t UCA0IRTCTL_VLOC = 0x5E; // IrDA transmit control reg
  static const uint16_t UCA0IRRCTL_VLOC = 0x5F; // IrDA Receive control reg
  
  // Set initial values
  *(usci->UCA0CTL0   = (uint8_t *) get_addr_ptr(UCA0CTL0_VLOC))  = 0;
  *(usci->UCA0CTL1  = (uint8_t *) get_addr_ptr(UCA0CTL1_VLOC))   = 0x01;
  *(usci->UCA0BR0  = (uint8_t *) get_addr_ptr(UCA0BR0_VLOC))     = 0;
  *(usci->UCA0BR1  = (uint8_t *) get_addr_ptr(UCA0BR1_VLOC))     = 0;
  *(usci->UCA0MCTL  = (uint8_t *) get_addr_ptr(UCA0MCTL_VLOC))   = 0;
  *(usci->UCA0STAT  = (uint8_t *) get_addr_ptr(UCA0STAT_VLOC))   = 0;
  *(usci->UCA0RXBUF  = (uint8_t *) get_addr_ptr(UCA0RXBUF_VLOC)) = 0;
  *(usci->UCA0TXBUF  = (uint8_t *) get_addr_ptr(UCA0TXBUF_VLOC)) = 0;
  *(usci->UCA0ABCTL  = (uint8_t *) get_addr_ptr(UCA0ABCTL_VLOC))   = 0;
  *(usci->UCA0IRTCTL  = (uint8_t *) get_addr_ptr(UCA0IRTCTL_VLOC)) = 0;
  *(usci->UCA0IRRCTL  = (uint8_t *) get_addr_ptr(UCA0IRRCTL_VLOC)) = 0;  
}
