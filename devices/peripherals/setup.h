/* Peripherals register virtual locations */
enum {

  /* Port 1 [7...][...0] */
  P1IN_VIRTUAL_LOC   = 0x20,   /* Input */
  P1OUT_VIRTUAL_LOC  = 0x21,   /* Output */
  P1DIR_VIRTUAL_LOC  = 0x22,   /* Direction */
  P1IFG_VIRTUAL_LOC  = 0x23,   /* Interrupt flag */
  P1IES_VIRTUAL_LOC  = 0x24,   /* Interrupt Edge Select */
  P1IE_VIRTUAL_LOC   = 0x25,   /* Interrupt Enable */
  P1SEL_VIRTUAL_LOC  = 0x26,   /* Select */
  P1SEL2_VIRTUAL_LOC = 0x41,   /* Select 2 */
  P1REN_VIRTUAL_LOC  = 0x27,   /* Resistor Enable */
};
  
/* Peripherals register pointers */

/* Port 1        = r/w =   = reset? = */
uint8_t *P1IN;   /* r          -      */
uint8_t *P1OUT;  /* r/w     unchanged */
uint8_t *P1DIR;  /* r/w     PUC reset */
uint8_t *P1IFG;  /* r/w     PUC reset */
uint8_t *P1IES;  /* r/w     unchanged */
uint8_t *P1IE;   /* r/w     PUC reset */
uint8_t *P1SEL;  /* r/w     PUC reset */
uint8_t *P1SEL2; /* r/w     PUC reset */
uint8_t *P1REN;  /* r/w     PUC reset */

void ports_setup();
void power_up_clear();

#include "setup.c"
#include "port1.h"
