//# r2 or SR, the status register
typedef struct{
  uint8_t reserved : 7;   //# Reserved bits
  uint8_t overflow : 1;   //# Overflow flag
  uint8_t SCG1 : 1;       //# System Clock Generator SMCLK; ON = 0; OFF = 1;
  uint8_t SCG0 : 1;       //# System Clock Generator DCOCLK DCO ON = 0; DCO OFF = 1;

  uint8_t OSCOFF : 1;     //# Oscillator Off. LFXT1CLK ON = 0; LFXT1CLK OFF = 1;

  uint8_t CPUOFF : 1;     //# CPU off; CPU OFF = 1; CPU ON = 0;
  uint8_t GIE : 1;        //# General Interupt enable; Enable maskable ints = 1; 0 = dont 

  uint8_t negative : 1;   //# Negative flag
  uint8_t zero : 1;       //# Zero flag
  uint8_t carry : 1;      //# Carry flag; Set when result produces a carry
}status_reg;

status_reg SR;

//# Program Counter and Stack pointer
uint16_t PC, SP; // r0 & r1, respectively

//# r3 or CG2, Constant Generator #2
int16_t CG2;

//# General Purpose registers
int16_t r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r14, r15;

//##########+++ MSP430 Register initialization +++##########
void initialize_msp_registers()
{
  //# Initialize Program Counter to 0xFFFE at boot or reset
  PC = 0xC000;

  //# Stack pointer typically begins at the top of RAM after reset
  SP = 0x400;

  //# Initialize the status register
  memset( &SR, 0, sizeof(SR) );

  CG2 = 0;
  r4 = r5 = r6 = r7 = r8 = r9 = r10 = r11 = r12 = r13 = r14 = r15 = 0;
}

//##########+++ Return value from SR struct +++##########
uint16_t sr_to_value()
{ 
  uint16_t r2;
  r2 &= 0x0;

  if (SR.overflow) {
    r2 |= 0x0100; 
  }

  if (SR.SCG1) {
    r2 |= 0x0080;
  }

  if (SR.SCG0) {
    r2 |= 0x0040;
  }

  if (SR.OSCOFF) {
    r2 |= 0x0020;
  }

  if (SR.CPUOFF) {
    r2 |= 0x0010;
  }

  if (SR.GIE) {
    r2 |= 0x0008;
  }

  if (SR.negative) {
    r2 |= 0x0004;
  }
  
  if (SR.zero) {
    r2 |= 0x0002;
  }
  
  if (SR.carry) {
    r2 |= 0x0001;
  }
  
  return r2;
}
