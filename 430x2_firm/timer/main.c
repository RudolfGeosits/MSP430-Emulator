#include <msp430g2553.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;  // Stop WDT

  // Set DCO to 1 MHZ
  if(CALBC1_1MHZ!=0xFF){
    DCOCTL=0;            //SelectlowestDCOxandMODx
    BCSCTL1=CALBC1_1MHZ; //Setrange
    DCOCTL=CALDCO_1MHZ;  //SetDCOstep+modulation
  }

  P1DIR |= BIT2;             // P1.2 to output
  P1SEL |= BIT2;             // P1.2 to TA0.1
  
  CCR0 = 20000-1;            // PWM Period (uS)
  CCTL1 = OUTMOD_7;          // CCR1 reset/set
  CCR1 = 2700;                // CCR1 PWM duty cycle
  TACTL = TASSEL_2 + MC_1;   // SMCLK, up mode

  _BIS_SR(LPM0_bits);        // Enter LPM0
}
