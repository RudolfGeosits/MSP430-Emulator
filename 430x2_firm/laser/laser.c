#include <msp430g2553.h>

#define ORIGIN 1400
#define LASER  BIT3

volatile unsigned long i;
volatile long current_x = -1, current_y = -1;

void little_delay() 
{
  i = 0x500;
  do (i--);
  while (i != 0);
}

void delay() 
{
  i = 0x2000;
  do (i--);
  while (i != 0);
}

void toggle_laser (unsigned short state)
{
  if (state == 0) {
    P1OUT &= ~LASER;
    return;
  }

  P1OUT |= LASER;
}

// Positions are from (0,0) to (180, 180)
void move_x (long degree)
{
  if (current_x == degree) return;

  current_x = degree;
  degree *= -1;

  P1OUT |= BIT4;
  P1OUT &= ~BIT5;
  
  /*
  int i;
  if (degree >= 0) {
    for (i = current_x; i < degree;i++) {
      TA0CCR1 = ORIGIN + (i * 9); // 9 is the factor
      little_delay();          
    }
  }
  else {
    for (i = current_x; i > degree;i--) {
      TA0CCR1 = ORIGIN + (i * 9); // 9 is the factor
      little_delay();    
    }
  }    
  */

  TA0CCR1 = ORIGIN + (degree * 9); // 9 is the factor
  delay();    
}

void move_y (long degree)
{
  if (current_y == degree) return;

  current_y = degree;
  degree *= -1;

  P1OUT |= BIT5;
  P1OUT &= ~BIT4;  

  /*
  int i;

  for (i = -current_y; i < degree;i++) {
    TA0CCR1 = ORIGIN + (i * 9); // 9 is the factor
    little_delay();          
  }
  */

  TA0CCR1 = ORIGIN + (degree * 9); // 9 is the factor
  delay();    
}

void move (long x, long y)
{
  move_x(x);
  move_y(y);
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

  // P1.6 is used for servo PWM output
  P1DIR |= BIT6 | BIT5 | BIT4 | BIT3;
  P1OUT = 0;
  P1SEL |= BIT6;
  P1SEL2 &= ~BIT6;
  
  // if SMCLK is about 1 MHz (or 1000000Hz), 
  // and 1000ms are the equivalent of 1 Hz,
  // then, by setting CCR0 to 22000 (1100000 / 1000 * 20)
  // we get a period of 20ms  
  TA0CCR0 = 20000-1;

  // Pulse Width (ticks)
  //TA0CCR1 = 600;

  // Timer A1 capture/compare control register options:
  // Select (reset/set) option
  TA0CCTL1 = OUTMOD_7;
  
  // Timer A1 control register options: (and start)
  // Select SMCLK, UP mode, x1 divider, 
  TA0CTL = TASSEL_2 + MC_1 + ID_0;

  toggle_laser(1);
  
  while (1) {
    move(0, 0);
    move(0, 20);
    move(20, 20);
    move(20, 0);

    /*
    toggle_laser(0);
    move(0, 0);
    delay();
    toggle_laser(1);
    little_delay();

    move(0, 10);
    move(10, 10);
    move(10, 0);
    move(20, 0);
    */
  }
}
