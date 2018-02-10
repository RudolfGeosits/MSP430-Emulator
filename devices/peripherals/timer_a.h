/*
  MSP430 Emulator
  Copyright (C) 2016 Rudolf Geosits (rgeosits@live.esu.edu)  
                                                                      
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
                                                                   
  This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.          
                                                       
  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses
*/

#ifndef _TIMER_A_H_
#define _TIMER_A_H_

#include "../cpu/registers.h"
#include "../utilities.h"

enum {STOP_MODE = 0, UP_MODE, CONTINOUS_MODE, UP_DOWN_MODE, NUM_MODES};

struct Timer_a {
  // (ALL RESET WITH POR)

  // --- TIMER_A0 ----------
  // Timer_A0 Control, @0x160
  uint16_t *TA0CTL;

  // Timer_A0 Counter, @0x170
  uint16_t *TA0R;

  // Timer_A0 Capture/Compare Control 0, @0x162
  uint16_t *TA0CCTL0;

  // Timer_A0 Capture/Compare 0, @0x172
  uint16_t *TA0CCR0;

  // Timer_A0 Capture/Compare Control 1, @0x164
  uint16_t *TA0CCTL1;

  // Timer_A0 Capture/Compare 1, @0x174
  uint16_t *TA0CCR1;

  // Timer_A0 Capture/Compare Control 2, @0x166
  uint16_t *TA0CCTL2;

  // Timer_A0 Capture/Compare 2, @0x176
  uint16_t *TA0CCR2;

  // Timer_A0 Interrupt Vector, @0x12E
  uint16_t *TA0IV; // READ ONLY

  // --- TIMER_A1
  // Timer_A1 Control
  uint16_t *TA1CTL;

  // Timer_A1 Counter
  uint16_t *TA1R;

  // Timer_A1 Capture/Compare Control 0
  uint16_t *TA1CCTL0;

  // Timer_A1 Capture/Compare 0
  uint16_t *TA1CCR0;

  // Timer_A1 Capture/Compare Control 1
  uint16_t *TA1CCTL1;

  // Timer_A1 Capture/Compare 1
  uint16_t *TA1CCR1;

  // Timer_A1 Capture/Compare Control 2
  uint16_t *TA1CCTL2;

  // Timer_A1 Capture/Compare 2
  uint16_t *TA1CCR2;

  // Timer_A1 Interrupt Vector
  uint16_t *TA1IV; // READ ONLY

  bool timer_0_running;
  bool capture_mode_0;
  bool compare_mode_0;
  uint8_t source_0;
  uint8_t idiv_0;
  uint8_t mode_0;

  bool timer_1_running;
  bool capture_mode_1;
  bool compare_mode_1;
  uint8_t source_1;
  uint8_t idiv_1;
  uint8_t mode_1;
};

void *timer_A0_thread (void *data);
void setup_timer_a (Emulator *emu);
void handle_timer_a (Emulator *emu);

#endif
