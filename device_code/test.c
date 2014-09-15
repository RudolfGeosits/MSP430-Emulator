#include <msp430g2553.h>

unsigned int i = 0;

void main(void){

  asm(
      "MOV @R5, 0x8(R8)\n"
  );
  
  // Stop the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;

  // A register that configures the direction of a port pin as an
  // input or output 
  P1DIR |= 0x10;

  while(1){
    P1OUT ^= 0x10;
    for(i = 0;i < 15000;i++);
  }
 
}
