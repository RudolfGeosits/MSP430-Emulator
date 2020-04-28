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

#ifndef IO_H
#define IO_H

#include "../main.h"

void print_console (Emulator *emu, const char *buf);
void print_serial (Emulator *emu, char *buf);

void put_serial(Emulator* const emu, const uint8_t x);
bool get_serial(Emulator* const emu, uint8_t* const x);

#endif