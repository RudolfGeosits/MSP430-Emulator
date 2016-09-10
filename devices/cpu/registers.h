#ifndef _REGISTERS_H_
#define _REGISTERS_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../../main.h"

/* r2 or SR, the status register */
typedef struct Status_reg {
  uint8_t reserved : 7;   // Reserved bits    
  uint8_t overflow : 1;   // Overflow flag
  uint8_t SCG1 : 1;  // System Clock Generator SMCLK; ON = 0; OFF = 1;
  uint8_t SCG0 : 1;  // System Clock Generator DCOCLK DCO ON = 0; DCO OFF = 1;
  uint8_t OSCOFF : 1;    // Oscillator Off. LFXT1CLK ON = 0; LFXT1CLK OFF = 1; 
  uint8_t CPUOFF : 1;     // CPU off; CPU OFF = 1; CPU ON = 0;                
  uint8_t GIE : 1;    // General Inter enabl; Enbl maskable ints = 1; 0 = dont 
  uint8_t negative : 1;   // Negative flag                                  
  uint8_t zero : 1;       // Zero flag                                     
  uint8_t carry : 1;      // Carry flag; Set when result produces a carry   
} Status_reg;

// Main CPU structure //
typedef struct Cpu {
  bool running;      /* CPU running or not */

  uint16_t pc, sp;   /* R0 and R1 respectively */
  Status_reg sr;     /* Status register fields */
  int16_t cg2;       /* R3 or Constant Generator #2 */
  
  int16_t r4, r5, r6, r7;   /* R4-R15 General Purpose Registers */
  int16_t r8, r9, r10, r11;
  int16_t r12, r13, r14, r15;

  Port_1 *p1;
  //Port_2 *p2;
  Usci *usci;
  Bcm *bcm;
  Timer_a *timer_a;
} Cpu;

uint16_t sr_to_value (Emulator *emu);
void set_sr_value (Emulator *emu, uint16_t value);
void initialize_msp_registers (Emulator *emu);
void update_register_display (Emulator *emu);

#endif
