/*
  MSP430 Emulator
  Copyright (C) 2020 Rudolf Geosits (rgeosits@live.esu.edu)

  "MSP430 Emulator" is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  "MSP430 Emulator" is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "memspace.h"

uint8_t* MEMSPACE;   /* Memory Space */
uint8_t* CODE;       // Code Memory
uint8_t* INFO;       // Info memory
uint8_t* IVT;        /* Interrupt Vector Table {Within ROM} */
uint8_t* ROM;        /* Flash/Read-Only memory */
uint8_t* RAM;        /* Random Access Memory */
uint8_t* PER16;      /* 16-bit peripherals */
uint8_t* PER8;       /* 8-bit peripherals */
uint8_t* SFRS;       /* Special Function Registers */

/* Initialize Address Space Locations
** 
** Allocate and set MSP430 Memory space
** Some of these locations vary by model
*/
void initialize_msp_memspace()
{
  // MSP430g2553 Device Specific ...
  // 16 KB / 64 KB Addressable Space
  MEMSPACE = (uint8_t *) calloc(1, 0x10000);
  //MEMSPACE = (uint8_t*)calloc(1, 0x100000);

  // (lower bounds, so increment upwards)

  // Info memory from 0x10FF - 0x1000 (256 Bytes)
  INFO = MEMSPACE + 0x1000;
  // Set it all to 0xFF by default...
  memset(INFO, 0xFF, 256);

  // Code Memory 0xFFFF - 0xC000;
  CODE = MEMSPACE + 0xC000;
  // Set it all to 0xFF by default...
  memset(CODE, 0xFF, 16384);
  
  // Interrupt Vector Table 0xFFFF - 0xFFC0
  IVT = MEMSPACE + 0xFFC0;
  
  // ROM // 0x400 - 0x1FFFF
  ROM = MEMSPACE + 0x0400;   

  // RAM from 0x3FF - 0x200
  RAM = MEMSPACE + 0x0200;   // 0x200 - 0x3FF
  PER16 = MEMSPACE + 0x0100;   // 0x0100 - 0x01FF
  PER8 = MEMSPACE + 0x0010;   // 0x0010 - 0x00FF
  SFRS = MEMSPACE + 0x0;      // 0x0 - 0x0F
  
  // Setup the calibration data in info memory

  const uint16_t CALDCO_16MHZ_VLOC = 0x10F8;
  *((uint8_t *) (MEMSPACE + CALDCO_16MHZ_VLOC)) = 0x95;

  const uint16_t CALBC1_16MHZ_VLOC = 0x10F9;
  *((uint8_t *) (MEMSPACE + CALBC1_16MHZ_VLOC)) = 0x8F;
  
  const uint16_t CALDCO_12MHZ_VLOC = 0x10FA;
  *((uint8_t *) (MEMSPACE + CALDCO_12MHZ_VLOC)) = 0x9E;
  
  const uint16_t CALBC1_12MHZ_VLOC = 0x10FB;
  *((uint8_t *) (MEMSPACE + CALBC1_12MHZ_VLOC)) = 0x8E;
  
  const uint16_t CALDCO_8MHZ_VLOC = 0x10FC;
  *((uint8_t *) (MEMSPACE + CALDCO_8MHZ_VLOC)) = 0x92;
  
  const uint16_t CALBC1_8MHZ_VLOC = 0x10FD;
  *((uint8_t *) (MEMSPACE + CALBC1_8MHZ_VLOC)) = 0x8D;
  
  const uint16_t CALDCO_1MHZ_VLOC = 0x10FE;
  *((uint8_t *) (MEMSPACE + CALDCO_1MHZ_VLOC)) = 0xD1;
  
  const uint16_t CALBC1_1MHZ_VLOC = 0x10FF;
  *((uint8_t *) (MEMSPACE + CALBC1_1MHZ_VLOC)) = 0x86;  
}

/* 
** Free MSP430 virtual memory
*/
void uninitialize_msp_memspace()
{
  free(MEMSPACE);
}
