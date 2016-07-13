#include <msp430g2553.h>

volatile int i = 0;

void main (void)
{
  WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

  P1DIR |= 0x13; // P1.0,1 and P1.4 outputs
  P1SEL |= 0x11; // P1.0,4 ACLK, SMCLK output

  while (1) {i++;}

  /*
  if (CALBC1_1MHZ != 0xFF)
    {
      DCOCTL = 0; // Select lowest DCOx and MODx
      BCSCTL1 = CALBC1_1MHZ; // Set range
      DCOCTL = CALDCO_1MHZ; // Set DCO step + modulation
    }

  if (CALBC1_8MHZ != 0xFF)
    {
      DCOCTL = 0; // Select lowest DCOx and MODx
      BCSCTL1 = CALBC1_8MHZ; // Set range
      DCOCTL = CALDCO_8MHZ; // Set DCO step + modulation
    }

  if (CALBC1_12MHZ != 0xFF)
    {
      DCOCTL = 0; // Select lowest DCOx and MODx
      BCSCTL1 = CALBC1_12MHZ; // Set range
      DCOCTL = CALDCO_12MHZ; // Set DCO step + modulation
    }

  if (CALBC1_16MHZ != 0xFF)
    {
      DCOCTL = 0; // Select lowest DCOx and MODx
      BCSCTL1 = CALBC1_16MHZ; // Set range
      DCOCTL = CALDCO_16MHZ; // Set DCO step + modulation
    }
  */
}
