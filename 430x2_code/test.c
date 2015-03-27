#include <msp430g2553.h>

unsigned int i = 0;

void main(void){
  asm(
      "LABEL1:\n"
      /*
      "MOV #0xF43F, R4\n"
      "MOV #0x0041, R5\n"
      "CMP R5, R4\n"
      "JL LABEL1\n"
      
      "MOV #0x7112, R4\n"
      "MOV #0x73F, R5\n"
      "AND.B R5, R4\n"
      
      "MOV #0xFFF4, R4\n"
      "MOV #0x8032, R5\n"
      "AND.B R5, R4\n"
      
      "MOV #0x10, R4\n"
      "MOV #0x0A11, R5\n"
      "AND.B R5, R4\n"

      // Official Tester (52 cases)
      "ADD R5, R4\n"    // Reg - reg 
      "ADD #0, R5\n"    // const gen - reg
 
      "ADD R5, 0x0(R4)\n"  // Register - Indexed 
      "ADD R5, 0xA\n"      // Register - Symbolic 
      "ADD R5, &0x0300\n"     // register - absolute 
      "ADD #0, 0x0(R4)\n"  // Const Gen - Indexed 
      "ADD #0, 0xA\n"      // Const Gen - Symbolic       
      "ADD #0, &0x0300\n"     // Const Gen - Absolute 

      "ADD 0x0(R5), R4\n" // Indexed - register 
      "ADD 0xA, R4\n"     // Symbolic - register 
      "ADD &0x0300, R4\n"    // Absolute - register 
      "ADD #1, R4\n"      // Const Gen - Register 
     
      "ADD 0x0(R5), 0x0(R4)\n" // Indexed - Indexed 
      "ADD 0xA, 0x0(R4)\n"     // Symbolic - Indexed 
      "ADD 0x0(R5), 0xA\n"     // Indexed - Symbolic 
      "ADD 0xA, 0x10\n"        // Symbolic - Symbolic 
      "ADD &0x0300, 0x0(R4)\n"    // Absolute - Indexed 
      "ADD 0x0(R4), &0x0300\n"    // Indexed - Absolute 
      "ADD &0x0300, &0x100\n"     // Absolute - Absolute  
      "ADD &0x0300, 0xA\n"        // Absdolute - symbolic 
      "ADD 0xA, &0x0300\n"        // Symbolic - absolute 
      "ADD #1, 0x0(R4)\n"      // Const Gen - Indexed 
      "ADD #1, 0xA\n"          // Const Gen - Symbolic 
      "ADD #1, &0xA\n"         // Const Gen - Absolute 
     
      "ADD @R5, R4\n"   // Indirect - register 
      "ADD #2, R5\n"    // Const Gen - Register 
      "ADD #4, R5\n"

      "ADD @R4, 0x0(R5)\n"   // Indirect - Indexed 
      "ADD @R4, 0xA\n"       // Indirect - symbolic 
      "ADD @R4, &0x0300\n"      // Indirect - Absolute 
      "ADD #2, 0x0(R4)\n" // Const Gen - Indexed 
      "ADD #4, 0x0(R4)\n"
      "ADD #2, 0xA\n"     // Const Gen - Symbolic 
      "ADD #4, 0xA\n"     
      "ADD #2, &0xA\n"    // Const Gen - Absolute 
      "ADD #4, &0xA\n"

      "ADD @R4+, R5\n"    // Indirect Inc - register 
      "ADD #0x1234, R5\n" // Immediate - register 
      "ADD #-1, R4\n"     // Const Gen - Absolute 
      "ADD #8, R4\n"
      
      "ADD @R5+, 0x0(R4)\n"    // indirect inc - indexed 
      "ADD @R5+, 0xA\n"        // Indirect inc - symbolic 
      "ADD @R5+, &0x0300\n"       // Indirect inc - absolute 
      "ADD #0x5678, 0x0(R4)\n" // Immediate - indexed 
      "ADD #0x1232, 0xA\n"    // Immediate - symbolic 
      "ADD #0x9928, &0x0300\n"   // Immediate - Absolute 
      "ADD #-1, 0x0(R4)\n"    // Const Gen - Indexed 
      "ADD #8, 0x0(R4)\n"
      "ADD #-1, 0xA\n"        // Const Gen - Symbolic 
      "ADD #8, 0xA\n"
      "ADD #-1, &0xA\n"       // Const Gen - Absolute 
      "ADD #8, &0xA\n"      

      // End Official Tester

      "CALL #0x1\n" 
      "CALL #0x8\n" 
      "CALL #-0x1\n" 
      "CALL &0x1234\n" 
      "CALL #0x72A\n" 
      "CALL 0x4\n" 
      "CALL &0x4\n" 

      "CALL R5\n"
      "CALL 0x2(R5)\n" 
      "CALL @R5\n" 
      "CALL @R5+\n" 

      "PUSH -0x2\n" 
      "PUSH 0x2\n" 
      "PUSH.B 0x4\n" 
      
      "PUSH &0xC000\n" 
      "PUSH.B &0xC010\n" 
      
      "PUSH #4\n" 
      "PUSH #8\n" 
      "PUSH #0\n"
      "PUSH #1\n"
      "PUSH #2\n"
      "PUSH #-1\n"
      
      "PUSH.B #4\n"
      "PUSH.B #8\n"
      "PUSH.B #0\n"
      "PUSH.B #1\n"
      "PUSH.B #2\n"
      "PUSH.B #-1\n"
      
      "PUSH #0x1234\n"
      "PUSH.B #0x78AB\n"

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
      */
      );
  
  // Stop the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;

  i = 0;
    
  // A register that configures the direction of a port pin as an
  // input or output 
  P1DIR |= BIT0 | BIT6;
  P1OUT |= BIT0;
  P1OUT &= ~BIT6;

  while(1){
    P1OUT ^= BIT0 | BIT6;
    for(i = 100;i > 0;i--);
  }
 
}
