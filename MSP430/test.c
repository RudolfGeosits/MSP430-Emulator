#include <msp430g2553.h>

unsigned int i = 0;

void main(void){

  asm(
      "CALL R5\n"
      "CALL 0x10(R5)\n"
      "CALL @R5\n"
      "CALL @R5+\n"
      "SXT R5\n"
      "SXT 0x10(R5)\n"
      "SXT @R5\n"
      "SXT @R5+\n"
      "SXT #0xFFFF\n"
      "PUSH R5\n"
      "PUSH 0x10(R5)\n"
      "PUSH @R5\n"
      "PUSH @R5+\n"
      "SWPB 0x10(R5)\n"
      "RRC R5\n"
      "RRC 0x10(R5)\n"
      "RRC @R5\n"
      "RRC @R5+\n"
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
