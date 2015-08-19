#include    <msp430.h>

volatile int i, j;
void print (char *txt);
void setup ();
void receive ();

void buzzer (unsigned char birdie)
{
  //if ( (birdie >> 2) == 0x33 ) {
  P1OUT ^= BIT0;
  //}
  while (1) {i++;}
}

int main(void)
{
  setup();

  print("***************************************\n\r"\
	"\t[ MSP430 Micro Hacking ]\n\r"\
	"***************************************\n\r"\
	"\t(0) Toggle Red LED\n\r\t(1) Toggle Green LED\n\r\n\r"\
	"[Enter Escape Sequence \\h to enter base 16. Ex: \\hF1\\hF0]\n\r\n\r"
	);
  
  while (1) receive();
}

void receive () 
{
  char buf[5];
  int x = 0; 

  // Print out prompt
  print(">> ");

  do {
    while  (!(IFG2 & UCA0RXIFG));
    while  (!(IFG2 & UCA0TXIFG));
    
    IFG2 &= ~UCA0RXIFG;
    //buf[x++] = UCA0TXBUF = UCA0RXBUF;
    buf[x++] = UCA0RXBUF;
  } while (UCA0RXBUF != '\r');
  
  switch (buf[0]) {
    case '0': {
      P1OUT ^= BIT0;      	
      break;
    }
    case '1': {
      P1OUT ^= BIT6;
      break;
    }
  }
  
  // Print out NL and CR
  print("\n\r");
}

void print (char *txt)
{
  int k;
  
  for (k = 0;k < 0xFF;k++) {
    if (txt[k] == 0) break;

    while  (! (IFG2 & UCA0TXIFG) );
    UCA0TXBUF = txt[k];

    for(i = 0;i < 1000;i++);
  }
}

void setup () 
{
  WDTCTL = WDTPW + WDTHOLD;
  
  P1DIR |= BIT0 | BIT3 | BIT6;
  P1OUT &= ~BIT0 & ~BIT3 & ~BIT6;
  
  /* Use Calibration values  for 1MHz    Clock   DCO*/
  DCOCTL  = 0;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  
  /* Configure   Pin Muxing  P1.1    RXD and P1.2    TXD */
  P1SEL  = BIT1 | BIT2;
  P1SEL2 = BIT1 | BIT2;

  /* Place   UCA0    in  Reset   to  be  configured  */
  UCA0CTL1 = UCSWRST;

  /* Configure   */
  UCA0CTL1   |= UCSSEL_2;   //  SMCLK
  UCA0BR0    = 104;    //  1MHz    9600
  UCA0BR1    = 0;  //  1MHz    9600
  UCA0MCTL   = UCBRS0; //  Modulation  UCBRSx  =   1

  /* Take    UCA0    out of  reset   */
  UCA0CTL1 &= ~UCSWRST;
  
  /* Enable  USCI_A0 RX  interrupt   */
  IE2 |= UCA0RXIE;  

  //__bis_SR_register(GIE);   //  Enter   LPM0,   interrupts  enabled
}
