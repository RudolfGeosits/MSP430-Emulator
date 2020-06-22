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

#pragma once
#ifndef _MEMSPACE_H_
#define _MEMSPACE_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ADDRESS_SPACE_SIZE 0x10000

typedef enum {
  MemoryCell_Flag_Written = 1,
  MemoryCell_Flag_Read = 2
} MemoryCell_Flag;

void initialize_msp_memspace();
void uninitialize_msp_memspace();

uint8_t memory_read_byte(void* const address);
uint16_t memory_read_word(void* const address);

void memory_write_byte(void* const address, const uint8_t x);
void memory_write_word(void* const address, const uint16_t x);

uint8_t memory_get_flags(void* const address);
uint8_t memory_get_flags_of_virtual_address(void* const address);
void memory_clear_flags(void* const address);

#endif
