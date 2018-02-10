/*                                                                             
  This file is part of MSP430 Emulator             

  MSP430 Emulator is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by         
  the Free Software Foundation, either version 3 of the License, or            
  (at your option) any later version.                                          
                                                                           
  MSP430 Emulator is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of               
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                
  GNU General Public License for more details.                                 
                                                                               
  You should have received a copy of the GNU General Public License            
  along with MSP430 Emulator.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _BCM_H_
#define _BCM_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#include "../cpu/registers.h"
#include "../utilities.h"

enum {DCOCLK = 0, XT2CLK, VLOCLK, TACLK, ACLK, SMCLK, MCLK, INCLK, NUM_CLOCKS};

struct Bcm {
  // Peripheral register pointers //

  // DCO Control Register
  uint8_t *DCOCTL;  // r/w, @0x56, PUC:0x60

  // Basic Clock System Control 1
  uint8_t *BCSCTL1; // r/w, @0x57, POR: 0x87

  // Basic Clock System Control 2
  uint8_t *BCSCTL2; // r/w, @0x58, PUC: 0x0

  // Basic Clock System Control 3
  uint8_t *BCSCTL3; // r/w, @0x53, PUC: 0x5

  // SFR Interrupt Enable Register 1
  uint8_t *IE1;     // r/w, @0x0, PUC: 0x0

  // SFR Interrupt Flag Register 1
  uint8_t *IFG1;    // r/w, @0x2, PUC: 0x0

  // -----
  uint64_t dco_freq; // In Hz
  uint64_t dco_period; // In nanosecs  
  uint64_t dco_pulse_width; // In nanosecs  

  // ----
  uint8_t mclk_source;
  uint64_t mclk_div;
  uint64_t mclk_freq;
};

uint64_t nanosec_diff(struct timespec *timeA_p, struct timespec *timeB_p);
void mclk_wait_cycles (Emulator *emu, uint64_t cycles);
void smclk_wait_cycles (Emulator *emu, uint64_t cycles);
void aclk_wait_cycles (Emulator *emu, uint64_t cycles);
//void *DCO_source (void *data);
void setup_bcm (Emulator *emu);
void handle_bcm (Emulator *emu);

#endif
