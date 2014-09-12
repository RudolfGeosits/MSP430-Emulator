#include "msp_registers.h"

//# r2 or SR, the status register
struct{
  unsigned char reserved : 7;   //# Reserved bits
  unsigned char overflow : 1;   //# Overflow flag
  unsigned char SMCLK : 1;      //# System Clock Generator 1; ON = 0; OFF = 1;
  unsigned char DCOCLK : 1;     //# System Clock Generator 0; DCO OFF = 0; DCO ON = 1;
  unsigned char OSCOFF : 1;     //# Oscillator Off. LFXT1CLK OFF = 0; LFXT1CLK ON = 1;
  unsigned char CPUOFF : 1;     //# CPU off; CPU OFF = 1; CPU ON = 0;
  unsigned char GIE : 1;        //# General interrupt Enable, 
  unsigned char zero : 1;       //# Zero flag       
  unsigned char carry : 1;      //# Carry flag; Set when result produces a carry
  unsigned char negative : 1;   //# Negative flag
}SR;

//# Program Counter and Stack pointer
unsigned short PC, SP; // r0 & r1, respectively

//# r3 or CG2, Constant Generator #2
short CG2;

//# General Purpose registers
short r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r14, r15;

//##########+++ MSP430 Register initialization +++##########
void initialize_msp_registers(){

  //# Initialize Program Counter to 0xFFFE
  PC = 0xFFFE;

  //# Stack pointer typically begins at the top of RAM after reset
  SP = 0;

  memset( &SR, 0, sizeof(SR) );

  CG2 = 0;
  r4 = r5 = r6 = r7 = r8 = r9 = r10 = r11 = r12 = r13 = r14 = r15 = 0;

}
