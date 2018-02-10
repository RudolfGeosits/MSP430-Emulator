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

#include "timer_a.h"

void handle_timer_a (Emulator *emu)
{
  Cpu *cpu = emu->cpu;
  Timer_a *timer = cpu->timer_a;

  uint16_t TA0CTL = *timer->TA0CTL;

  // --------------------------------------------------------------
  // Handle Timer_A0 Control Register
  uint8_t TASSEL0 = (TA0CTL >> 8) & 0x03;
  uint8_t ID0     = (TA0CTL >> 6) & 0x03;
  uint8_t MC0     = (TA0CTL >> 4) & 0x03;
  uint8_t TA0CLR   = (TA0CTL >> 2) & 0x01;
  uint8_t TA0IE    = (TA0CTL >> 1) & 0x01;
  uint8_t TA0IFG   = TA0CTL & 0x01;

  switch (TASSEL0) {
  case 0b00: {timer->source_0 = TACLK; break;}
  case 0b01: {timer->source_0 = ACLK; break;}
  case 0b10: {timer->source_0 = SMCLK; break;}
  case 0b11: {timer->source_0 = INCLK; break;}
  default: break;
  }

  switch (ID0) {
  case 0b00: {timer->idiv_0 = 1; break;}
  case 0b01: {timer->idiv_0 = 2; break;}
  case 0b10: {timer->idiv_0 = 4; break;}
  case 0b11: {timer->idiv_0 = 8; break;}
  default: break;
  }

  switch (MC0) {
  case 0b00: {timer->mode_0 = STOP_MODE; break;}
  case 0b01: {timer->mode_0 = UP_MODE; break;}
  case 0b10: {timer->mode_0 = CONTINOUS_MODE; break;}
  case 0b11: {timer->mode_0 = UP_DOWN_MODE; break;}
  default: break;
  }

  /* Timer_A clear; setting this bit resets TAR, the clock divider,
     and the count direction. The TACLR bit is automatically 
     reset and is always read as zero. */
  if (TA0CLR) {    
    *timer->TA0R = 0;
    *timer->TA0CTL &= 0xFF0B; // 0b00001011
  }
  // --------------------------------------------------------------

  // --------------------------------------------------------------
  // Handle Timer_A0 Capture/Compare Control Register
  
  uint16_t TA0CCTL1 = *timer->TA0CCTL1;
 
  uint8_t OUTMOD1 = (TA0CCTL1 >> 5) & 0x07;  
  uint8_t CAP     = (TA0CCTL1 >> 8) & 0x01;

  switch (OUTMOD1) {
  case 0b000: {break;}
  case 0b001: {break;}
  case 0b010: {break;}
  case 0b011: {break;}
  case 0b100: {break;}
  case 0b101: {break;}
  case 0b110: {break;}
  case 0b111: {break;}
  default: break;
  }  

  // CAP field (Capture or compare mode?)
  timer->capture_mode_0 = CAP;
  timer->compare_mode_0 = !CAP;

  // --------------------------------------------------------------


  static double last_period = 0;
  static double last_pulse_width = 0;

  // Figure Out Frequency in up mode
  if (timer->compare_mode_0) {
    if (timer->mode_0 == UP_MODE) {
      uint16_t period_ct = *timer->TA0CCR0 + 1;       
      uint64_t frequency = emu->cpu->bcm->mclk_freq;

      double period = (1.0/frequency) * period_ct;// In seconds
      double pulse_width = (1.0/frequency) * (*timer->TA0CCR1);
      double duty_cycle  = pulse_width / period; 
      
      /*
	printf("period: %lf\npulse_width: %lf\nduty: %lf%%\n", 
	period, pulse_width, duty_cycle);
      */
      
      if (last_period != period || last_pulse_width != pulse_width) {
	if (period >= 0.015 && period <= 0.025) { // 0.020 is sweet spot 50 Hz
	  //printf("period: %lf, last_period: %lf\n", period, last_period);
	  //printf("pw: %lf, last_pw: %lf\n", pulse_width, last_pulse_width);

	  if (pulse_width < 0.0009) {
	    // Send Control for 0 degrees
	    //print_console(emu, "0 degrres\n");
	    
	    uint8_t byte = 0;
	    send_control(emu, SERVO_MOTOR, (void *)&byte, 1);
	  }
	  else if (pulse_width < 0.0012) {
	    // Send Control for 30 Degrees
	    //print_console(emu, "30 degrres\n");
	    
	    uint8_t byte = 30;
	    send_control(emu, SERVO_MOTOR, (void *)&byte, 1);
	  }
	  else if (pulse_width < 0.0015) {
	    // Send Control for 60 degrees
	    uint8_t byte = 60;
	    send_control(emu, SERVO_MOTOR, (void *)&byte, 1);
	  }
	  else if (pulse_width < 0.0018) {
	    // Send Control for 90 degrees
	    //print_console(emu, "90 degrres\n");
	    
	    uint8_t byte = 90;
	    send_control(emu, SERVO_MOTOR, (void *)&byte, 1);
	  }
	  else if (pulse_width < 0.0021) {
	    // Send Control for 120 degrees
	    uint8_t byte = 120;
	    send_control(emu, SERVO_MOTOR, (void *)&byte, 1);
	  }
	  else if (pulse_width >= 0.0021) {
	    // Send Control for 150 degrees
	    uint8_t byte = 150;
	    send_control(emu, SERVO_MOTOR, (void *)&byte, 1);
	  }
	}
      }

      if (OUTMOD1 == 0b111) { // RESET/SET
	uint16_t pulse_width = *timer->TA0CCR1;	
      }

      last_period = period;
      last_pulse_width = pulse_width;
    }
  }

  if (!timer->timer_0_running && MC0 != 0) {
    //print_console(emu, "START TIMER\n");
    timer->timer_0_running = true;

    //pthread_t pp;
    //if(pthread_create(&pp, NULL, timer_A0_thread , (void*)emu)){
    //printf("ErrorcreatingDCOthread\n");
    //exit(1);
    //}
  }
}

void *timer_A0_thread (void *data)
{
  Emulator *emu = (Emulator *) data;
  Timer_a *timer = emu->cpu->timer_a;
  uint64_t counter;

  bool high = false;

  while (true) {
    high = true;
    counter = 0;

    while (true) {
      if ( counter == (*timer->TA0CCR1) ) {
	high = false;
      }
      else if ( counter == (*timer->TA0CCR0 + 1) ) {
	break;
      }
      
      mclk_wait_cycles(emu, 1);
      counter++;

      high ? printf("-") : printf("_");
      fflush(stdout);
    }
  }

  return NULL;
}

void setup_timer_a (Emulator *emu)
{
  Cpu *cpu = emu->cpu;
  Timer_a *timer = cpu->timer_a;

  // Configure Timer_A0 Registers
  const uint16_t TA0CTL_VLOC  = 0x160;
  const uint16_t TA0R_VLOC  = 0x170;
  const uint16_t TA0CCTL0_VLOC = 0x162;
  const uint16_t TA0CCR0_VLOC  = 0x172;
  const uint16_t TA0CCTL1_VLOC  = 0x164;
  const uint16_t TA0CCR1_VLOC  = 0x174;
  const uint16_t TA0CCTL2_VLOC  = 0x166;
  const uint16_t TA0CCR2_VLOC  = 0x176;
  const uint16_t TA0IV_VLOC  = 0x12E;
  
  *(timer->TA0CTL  = (uint16_t *) get_addr_ptr(TA0CTL_VLOC)) = 0;
  *(timer->TA0R  = (uint16_t *) get_addr_ptr(TA0R_VLOC)) = 0;
  *(timer->TA0CCTL0  = (uint16_t *) get_addr_ptr(TA0CCTL0_VLOC)) = 0;
  *(timer->TA0CCR0  = (uint16_t *) get_addr_ptr(TA0CCR0_VLOC)) = 0;
  *(timer->TA0CCTL1  = (uint16_t *) get_addr_ptr(TA0CCTL1_VLOC)) = 0;
  *(timer->TA0CCR1  = (uint16_t *) get_addr_ptr(TA0CCR1_VLOC)) = 0;
  *(timer->TA0CCTL2  = (uint16_t *) get_addr_ptr(TA0CCTL2_VLOC)) = 0;
  *(timer->TA0CCR2  = (uint16_t *) get_addr_ptr(TA0CCR2_VLOC)) = 0;
  *(timer->TA0IV  = (uint16_t *) get_addr_ptr(TA0IV_VLOC)) = 0;

  // Configure Timer_A1 Registers
  const uint16_t TA1CTL_VLOC  = 0x180;
  const uint16_t TA1R_VLOC  = 0x190;
  const uint16_t TA1CCTL0_VLOC = 0x182;
  const uint16_t TA1CCR0_VLOC  = 0x192;
  const uint16_t TA1CCTL1_VLOC  = 0x184;
  const uint16_t TA1CCR1_VLOC  = 0x194;
  const uint16_t TA1CCTL2_VLOC  = 0x186;
  const uint16_t TA1CCR2_VLOC  = 0x196;
  const uint16_t TA1IV_VLOC  = 0x11E;
  
  *(timer->TA1CTL  = (uint16_t *) get_addr_ptr(TA1CTL_VLOC)) = 0;
  *(timer->TA1R  = (uint16_t *) get_addr_ptr(TA1R_VLOC)) = 0;
  *(timer->TA1CCTL0  = (uint16_t *) get_addr_ptr(TA1CCTL0_VLOC)) = 0;
  *(timer->TA1CCR0  = (uint16_t *) get_addr_ptr(TA1CCR0_VLOC)) = 0;
  *(timer->TA1CCTL1  = (uint16_t *) get_addr_ptr(TA1CCTL1_VLOC)) = 0;
  *(timer->TA1CCR1  = (uint16_t *) get_addr_ptr(TA1CCR1_VLOC)) = 0;
  *(timer->TA1CCTL2  = (uint16_t *) get_addr_ptr(TA1CCTL2_VLOC)) = 0;
  *(timer->TA1CCR2  = (uint16_t *) get_addr_ptr(TA1CCR2_VLOC)) = 0;
  *(timer->TA1IV  = (uint16_t *) get_addr_ptr(TA1IV_VLOC)) = 0;

  // Configure other
  timer->source_0 = 0b10;
  timer->timer_0_running = false;

  timer->source_1 = 0b10;
  timer->timer_1_running = false;
}

/* POWER UP CLEAR (PUC)      
 *
 * A PUC is always generated when a POR is generated, but a POR is not
 * generated by a PUC. The following events trigger a PUC:  
 *                                                
 * A POR signal                             
 * Watchdog timer expiration when in watchdog mode only
 * Watchdog timer security key violation          
 * A Flash memory security key violation        
 * A CPU instruct fetch from the peripheral address range 0h to 01FFh

void power_up_clear () {
  *P1OUT = *P1DIR = *P1IFG = *P1IE = *P1SEL = *P1SEL2 = *P1REN = 0;
}
 */
