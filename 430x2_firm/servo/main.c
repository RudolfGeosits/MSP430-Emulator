#include <msp430g2553.h>

volatile unsigned long i;
unsigned char ir_active;
unsigned char person_there;

void delay(volatile unsigned long d) 
{
  //i = 49999;
  do (d--);
  while (d != 0);
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

  // P1.6/TA0.0 is used for servo output
  P1DIR |= BIT6 | BIT0;
  P1OUT = 0;
  P1SEL |= BIT6;
  P1SEL2 &= ~BIT6;

  // P1.5 as input
  //P1DIR &= ~BIT5;
  
  // if SMCLK is about 1 MHz (or 1000000Hz), 
  // and 1000ms are the equivalent of 1 Hz,
  // then, by setting CCR0 to 22000 (1100000 / 1000 * 20)
  // we get a period of 20ms  
  TA0CCR0 = 20000-1;

  // Pulse Width (ticks)
  TA0CCR1 = 600;

  // Timer A1 capture/compare control register options:
  // Select (reset/set) option
  TA0CCTL1 = OUTMOD_7;
  
  // Timer A1 control register options: (and start)
  // Select SMCLK, UP mode, x1 divider, 
  TA0CTL = TASSEL_2 + MC_1 + ID_0;

  P1IE |= BIT5; // P1.3 interrupt enabled
  P1IFG &= ~BIT5; // P1.3 IFG cleared
  __enable_interrupt(); // enable all interrupts
  P1IES &= ~BIT5; // Lo/High Edge
  P1REN &= ~BIT5; // pull down // enable internal pull up, 1
  ir_active = 0;
  person_there = 0;

  while (1) {
    if (ir_active && !person_there) {
      delay(0xFFFF);
      delay(0xFFFF);
      delay(0xFFFF);
      delay(0xFFFF);
      delay(0xFFFF);
      delay(0xFFFF);
      delay(0xFFFF);

      if (ir_active) {
	person_there = 1;
      }
    }

    if (person_there) {
      if (!ir_active) {
	delay(0xFFFF);
	delay(0xFFFF);
	delay(0xFFFF);
	delay(0xFFFF);
	delay(0xFFFF);
	delay(0xFFFF);
	delay(0xFFFF);

	if (!ir_active) {
	  // Flush toilet

	  delay(50000);
	  TA0CCR1 = 1333;
	  delay(50000);
	  TA0CCR1 = 600;
	  delay(50000);

	  person_there = 0;
	}
      }
    }

  }
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  if (P1IN & BIT5) {
    P1OUT |= BIT0;  // P1.0 = toggle
    ir_active = 1;
  }
  else {
    P1OUT &= ~BIT0;
    ir_active = 0;
  }

  P1IES ^= BIT5;  // toggle the interrupt edge,
  P1IFG &= ~BIT5; // P1.3 IFG cleared  
}
