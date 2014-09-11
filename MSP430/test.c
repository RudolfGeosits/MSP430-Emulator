#include <msp430g2553.h>

unsigned int i = 0;

void main(void){

  asm(
      "RRC @R7\n"   //Indirect register mode
      "RRC @R7+\n"  //Indirect autoincrement
      "RRC R7\n"    //Register mode
      "RRC.B R7\n"  //Register mode byte
      "SWPB R7\n"
      "SWPB R7\n"
      "RRC R4\n"
      "SXT R5\n"
      "PUSH R8\n"
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
