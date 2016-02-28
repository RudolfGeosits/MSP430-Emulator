#include <msp430g2553.h>

void delay() 
{
  volatile unsigned long i;
  i = 49999;
  do (i--);
  while (i != 0);
}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;              

  // P2.2/TA1.1 is used for servo output
  P2DIR |= BIT2;
  P2OUT = 0; // Clear all outputs P2
  P2SEL |= BIT2; // P2.2 select TA1.1 option
  
  // if SMCLK is about 1.1MHz (or 1100000Hz), 
  // and 1000ms are the equivalent of 1 Hz,
  // then, by setting CCR0 to 22000 (1100000 / 1000 * 20)
  // we get a period of 20ms  
  TA1CCR0 = 21881;

  // Pulse Width (ticks)
  TA1CCR1 = 600;

  // Timer A1 capture/compare control register options:
  // Select (reset/set) option
  TA1CCTL1 = OUTMOD_6;
  
  // Timer A1 control register options: (and start)
  // Select SMCLK, UP mode, x1 divider, 
  TA1CTL = TASSEL_2 + MC_1 + ID_0;

  while (1) {
    delay();
    TA1CCR1 = 600;

    delay();
    TA1CCR1 = 1500;    
  }
}
