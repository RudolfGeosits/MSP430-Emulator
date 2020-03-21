#include <msp430g2553.h>

volatile int i, j;

void print(char *txt, int len){
  int k;
  
  for (k = 0;k < len;k++) {
    while  (! (IFG2 & UCA0TXIFG) );    //  USCI_A0 TX  buffer  ready?
    P1OUT ^= BIT6;
    
    UCA0TXBUF = txt[k];
    
    for(i = 0;i < 5000;i++);
  }
}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD; // Stop WDT

  P1DIR |= BIT0 | BIT6;
  P1OUT &= ~BIT0;
  P1OUT &= ~BIT6;

  /* Use Calibration values  for 1MHz    Clock   DCO*/
  DCOCTL  = 0;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;

  // Configure Pin Muxing P1.1 RXD and P1.2 TXD
  P1SEL  = BIT1 | BIT2;
  P1SEL2 = BIT1 | BIT2;
  
  // Place   UCA0    in  Reset   to  be  configured
  UCA0CTL1 = UCSWRST;
  
  // configure
  UCA0CTL1   |=  UCSSEL_2;   //  SMCLK
  UCA0BR0    =   104;        //  1MHz    9600
  UCA0BR1    =   0;          //  1MHz    9600
  UCA0MCTL   =   UCBRS0;     //  Modulation  UCBRSx  =   1
  
  // Take    UCA0    out of  reset
  UCA0CTL1   &=  ~UCSWRST;
  
  /* Enable  USCI_A0 RX  interrupt   */
  IE2    |=  UCA0RXIE;  

  __bis_SR_register(GIE);   //  Enter   LPM0,   interrupts  enabled

  while(1){
    print("\n\tMSP430 Emulator Virtual UART Working...\n", 42);
  };
}

/*  Echo    back    RXed    character,  confirm TX  buffer  is  ready   first   */
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR (void)
{
  P1OUT ^= BIT0;
}
