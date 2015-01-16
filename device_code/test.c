#include <msp430g2553.h>

unsigned int i = 0;

void main(void){

  asm(
      /*
      "MOV R5, R4\n"
      "MOV.B R5, R4\n"

      "MOV 0x2(R5), 0x4(R4)\n"
      "MOV.B 0x2(R5), 0x4(R4)\n"

      "MOV 0x2(R5), R4\n"
      "MOV.B 0x2(R5), R4\n"      

      "MOV R5, 0x2(R4)\n"
      "MOV.B R5, -0x2(R4)\n"      

      "MOV @R5, R4\n"
      "MOV.B @R5, R4\n"

      "MOV @R5, 0x2(R4)\n"
      "MOV.B @R5, 0x4(R4)\n"
      */
      "MOV #5, R5\n"
      "MOV.B #0x4560, R5\n"

      "MOV #5, 0x0(R5)\n"
      "MOV.B #0x1234, 0x2(R5)\n"

      "MOV @R5+, R4\n"
      "MOV.B @R5+, R4\n"

      "MOV @R5+, 0x0(R4)\n"
      "MOV.B @R5+, 0x0(R4)\n"

      //"CALL R5\n"
      //"CALL 0x2(R5)\n"
      //"CALL @R5\n"
      //"CALL @R5+\n"

      "PUSH R5\n"
      "PUSH.B R5\n"
      "PUSH 0x2(R5)\n"
      "PUSH.B 0x2(R5)\n"
      "PUSH @R5\n"
      "PUSH.B @R5\n"
      "PUSH @R5+\n"
      "PUSH.B @R5+\n"

      "SXT R5\n"
      "SXT -0x2(R5)\n"
      "SXT @R5\n"
      "SXT @R5+\n"

      "RRA R5\n"
      "RRA.B R5\n"
      "RRA 0x2(R5)\n"
      "RRA.B 0x2(R5)\n"
      "RRA @R5\n"
      "RRA.B @R5\n"
      "RRA @R5+\n"
      "RRA.B @R5+\n"

      "SWPB R5\n"
      "SWPB 0x2(R5)\n"
      "SWPB @R5\n"
      "SWPB @R5+\n"

      "RRC R5\n"
      "RRC.B R5\n"
      "RRC @R5\n"
      "RRC.B @R5\n"
      "RRC @R5+\n"
      "RRC.B @R5+\n"
      "RRC.B 0xF(R5)\n"
      "RRC -0x4(R5)\n"
  );
  
  // Stop the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;

  // A register that configures the direction of a port pin as an
  // input or output 
  P1DIR |= 0x10;

  while(1){
    P1OUT ^= 0x10;
    for(i = 0;i < 30000;i++);
  }
 
}
