#include <msp430.h>

int blink, i;

void main (void) {
  WDTCTL = WDTPW + WDTHOLD;

  P1SEL &= ~BIT0; // Select P1.0 as output
  P1OUT &= ~BIT0; // Select default as off
  P1DIR |= BIT0;  // Select direction as output

  while(1){
    for (blink = 0; blink < 10; ++blink) {
      P1OUT ^= BIT0;
      for(i = 0;i < 20000;++i);
    }

    if (CALBC1_8MHZ != 0xFF) {
      DCOCTL = 0; // select loweest DCOx
      BCSCTL1 = CALBC1_8MHZ;
      DCOCTL = CALDCO_8MHZ;
    }

    for (blink = 0; blink < 10; ++blink) {
      P1OUT ^= BIT0;
      for(i = 0;i < 20000;++i);
    }  
  }
}
