#include <msp430g2553.h>

unsigned int i = 0;

void main(void){

  asm(
      "MOV &0x3030, R5\n"
      "MOV R5, &0x3030\n"
      "MOV R5, R8\n"
      "MOV 0x10(R5), R8\n"
      "MOV R5, 0x10(R8)\n"
      "MOV 0x10(R5), 0x10(R8)\n"
      "MOV &0x1010, &0x2020\n"
      "MOV @R5, R8\n"
      "MOV R5, @R8\n"
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
