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

  // Set DCO to 1 MHZ                                                          
  if(CALBC1_1MHZ!=0xFF){
    DCOCTL=0;            //SelectlowestDCOxandMODx                             
    BCSCTL1=CALBC1_1MHZ; //Setrange                                            
    DCOCTL=CALDCO_1MHZ;  //SetDCOstep+modulation                               
  }

  // P1.5/TA0.0 is used for servo output
  P1DIR |= BIT6;
  P1OUT = 0;
  P1SEL |= BIT6;
  P1SEL2 &= ~BIT6;
  
  // if SMCLK is about 1 MHz (or 1000000Hz), 
  // and 1000ms are the equivalent of 1 Hz,
  // then, by setting CCR0 to 22000 (1100000 / 1000 * 20)
  // we get a period of 20ms  
  TA0CCR0 = 20000-1;

  // Pulse Width (ticks)
  TA0CCR1 = 600;

  // Timer A1 capture/compare control register options:
  // Select (reset/set) option
  TA0CCTL1 = OUTMOD_6;
  
  // Timer A1 control register options: (and start)
  // Select SMCLK, UP mode, x1 divider, 
  TA0CTL = TASSEL_2 + MC_1 + ID_0;

  while (1) {
    delay();
    TA0CCR1 = 600;

    delay();
    TA0CCR1 = 1500;    
  }
}
