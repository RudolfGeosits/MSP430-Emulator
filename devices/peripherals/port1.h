/* Peripherals activation flags */
bool P1DIR_0, P1OUT_0, P1IFG_0, P1IE_0;
bool P1DIR_1, P1OUT_1, P1IFG_1, P1IE_1;
bool P1DIR_2, P1OUT_2, P1IFG_2, P1IE_2;
bool P1DIR_3, P1OUT_3, P1IFG_3, P1IE_3;
bool P1DIR_4, P1OUT_4, P1IFG_4, P1IE_4;
bool P1DIR_5, P1OUT_5, P1IFG_5, P1IE_5;
bool P1DIR_6, P1OUT_6, P1IFG_6, P1IE_6;
bool P1DIR_7, P1OUT_7, P1IFG_7, P1IE_7;

typedef struct Port {
  // Peripheral register pointers

  // Port 1        = r/w =   = reset? =
  uint8_t *IN;   /* r          -      */
  uint8_t *OUT;  /* r/w     unchanged */
  uint8_t *DIR;  /* r/w     PUC reset */
  uint8_t *IFG;  /* r/w     PUC reset */
  uint8_t *IES;  /* r/w     unchanged */
  uint8_t *IE;   /* r/w     PUC reset */
  uint8_t *SEL;  /* r/w     PUC reset */
  uint8_t *SEL2; /* r/w     PUC reset */
  uint8_t *REN;  /* r/w     PUC reset */
} Port;

void setup_port_1(Cpu *cpu);
void handle_port_1(Cpu *cpu);

#include "port1.c"
