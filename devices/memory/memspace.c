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

#include <stdio.h>
#include "memspace.h"

uint8_t* MEMSPACE;   /* Memory Space */
uint8_t* MEMSPACE_FLAGS;   /* Memory Space */
uint8_t* CODE;       // Code Memory
uint8_t* INFO;       // Info memory
uint8_t* IVT;        /* Interrupt Vector Table {Within ROM} */
uint8_t* ROM;        /* Flash/Read-Only memory */
uint8_t* RAM;        /* Random Access Memory */
uint8_t* PER16;      /* 16-bit peripherals */
uint8_t* PER8;       /* 8-bit peripherals */
uint8_t* SFRS;       /* Special Function Registers */

static int32_t getEffectiveAddressIndex(void* offset)
{
  intptr_t offsetIndex = (intptr_t)offset;
  intptr_t memoryIndex = (intptr_t)MEMSPACE;

  if (offsetIndex < memoryIndex)
    return -1;
  if (offsetIndex > memoryIndex + ADDRESS_SPACE_SIZE)
    return -1;
  return offsetIndex - memoryIndex;
}

/* Initialize Address Space Locations
**
** Allocate and set MSP430 Memory space
** Some of these locations vary by model
*/
void initialize_msp_memspace()
{
  // MSP430g2553 Device Specific ...
  // 16 KB / 64 KB Addressable Space
  MEMSPACE = (uint8_t *) calloc(1, ADDRESS_SPACE_SIZE);
  MEMSPACE_FLAGS = (uint8_t *) calloc(1, ADDRESS_SPACE_SIZE);
  memset(MEMSPACE_FLAGS, 0x00, ADDRESS_SPACE_SIZE);

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


uint8_t memory_read_byte(void* const address)
{
  int32_t index = getEffectiveAddressIndex(address);
  if (index >= 0)
    MEMSPACE_FLAGS[index] |= (uint8_t)MemoryCell_Flag_Read;
  return *(uint8_t*)address;
}

uint16_t memory_read_word(void* const address)
{
  int32_t index = getEffectiveAddressIndex(address);
  if (index >= 0)
   {
      MEMSPACE_FLAGS[index] |= (uint8_t)MemoryCell_Flag_Read;
      MEMSPACE_FLAGS[index + 1] |= (uint8_t)MemoryCell_Flag_Read;
   }
  return *(uint16_t*)address;
}

void memory_write_byte(void* const address, const uint8_t x)
{
  int32_t index = getEffectiveAddressIndex(address);
  if (index >= 0)
    MEMSPACE_FLAGS[index] |= (uint8_t)MemoryCell_Flag_Written;
  (*(uint8_t*)address) = x;
}

void memory_write_word(void* const address, const uint16_t x)
{
  int32_t index = getEffectiveAddressIndex(address);
  if (index >= 0)
  {
    MEMSPACE_FLAGS[index] |= (uint8_t)MemoryCell_Flag_Written;
    MEMSPACE_FLAGS[index + 1] |= (uint8_t)MemoryCell_Flag_Written;
    //printf("Write [%04X] <- %04X\n", index, x);
  }  
  (*(uint16_t*)address) = x;
}

uint8_t memory_get_flags(void* const address)
{
  int32_t index = getEffectiveAddressIndex(address);
  if (index >= 0)
    return MEMSPACE_FLAGS[index];
  return 0;
}

uint8_t memory_get_flags_of_virtual_address(void* const address)
{
  uintptr_t index = (uintptr_t)address;
  if (index >= ADDRESS_SPACE_SIZE)
    return 0;
  return MEMSPACE_FLAGS[index];
}

void memory_clear_flags(void* const address)
{
  int32_t index = getEffectiveAddressIndex(address);
  if (index >= 0)
    MEMSPACE_FLAGS[index] = 0;
}

/*
** Free MSP430 virtual memory
*/
void uninitialize_msp_memspace()
{
  free(MEMSPACE);
  free(MEMSPACE_FLAGS);
}
