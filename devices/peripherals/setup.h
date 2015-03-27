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

/* Peripherals activation flags */
bool P1DIR_0, P1OUT_0, P1IFG_0, P1IE_0;
bool P1DIR_1, P1OUT_1, P1IFG_1, P1IE_1;
bool P1DIR_2, P1OUT_2, P1IFG_2, P1IE_2;
bool P1DIR_3, P1OUT_3, P1IFG_3, P1IE_3;
bool P1DIR_4, P1OUT_4, P1IFG_4, P1IE_4;
bool P1DIR_5, P1OUT_5, P1IFG_5, P1IE_5;
bool P1DIR_6, P1OUT_6, P1IFG_6, P1IE_6;
bool P1DIR_7, P1OUT_7, P1IFG_7, P1IE_7;

void ports_setup();
void power_up_clear();

#include "setup.c"
#include "port1.h"
