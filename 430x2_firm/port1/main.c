#include <msp430g2553.h>

unsigned int i = 0;

void main (void)
{      
  // Stop the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  
  // Set all as output
  P1DIR |= 0xFF;

  // Turn all on
  P1OUT |= 0xFF;
  
  while(1){
    P1OUT ^= 0xFF;

    for(i = 0;i < 40000;i++);
  }
 
}
