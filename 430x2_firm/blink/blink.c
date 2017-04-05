#include <msp430g2553.h>

// Don't optimize out the counter
volatile unsigned int i = 0;

void main (void)
{
  // Stop the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  
  // Initialize GPIO pins for LED operation
  P1OUT |= BIT0;        // Set red LED on initially  (P1.0)
  P1OUT &= ~BIT6;       // Set green LED off initially (P1.6)
  P1DIR |= BIT0 | BIT6; // Set the direction of P1.0/P1.6 to OUTPUT
  
  // Toggle LEDs forever
  while(1){    
    // Wait for 33300 Ticks
    for(i = 0;i < 33300;i++);

    // Toggle LED1 and LED2
    P1OUT ^= BIT0 | BIT6;
  }
}
