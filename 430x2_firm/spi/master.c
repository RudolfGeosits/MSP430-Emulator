#include <msp430.h>

volatile    char    received_ch =   0;
volatile char send = 0xAA;
int main(void)
{
  WDTCTL =   WDTPW   +   WDTHOLD;    //  Stop    WDT

  P1OUT  |=  BIT5;
  P1DIR  |=  BIT5 | BIT0;

  P1OUT &= ~BIT0;

  P1SEL  =   BIT1    |   BIT2    |   BIT4;
  P1SEL2 =   BIT1    |   BIT2    |   BIT4;

  UCA0CTL1   =   UCSWRST;
  UCA0CTL0   |=  UCCKPH  +   UCMSB   +   UCMST   +   UCSYNC; //  3-pin,  8-bit   SPI master
  UCA0CTL1   |=  UCSSEL_2;   //  SMCLK
  UCA0BR0    |=  0x02;   //  /2
  UCA0BR1    =   0;  //
  UCA0MCTL   =   0;  //  No  modulation
  UCA0CTL1   &=  ~UCSWRST;   //  **Initialize    USCI    state   machine**

  P1OUT  &=  (~BIT5);    //  Select  Device

  while (1) {
    while  (!(IFG2 &   UCA0TXIFG));    //  USCI_A0 TX  buffer  ready?
    P1OUT ^= BIT0;
    UCA0TXBUF = ~send;   //  Send    0xAA    over    SPI to  Slave
    while  (!(IFG2 &   UCA0RXIFG));    //  USCI_A0 RX  Received?
    received_ch    =   UCA0RXBUF;  //  Store   received    data
  }

  P1OUT  |=  (BIT5); //  Unselect    Device
}
