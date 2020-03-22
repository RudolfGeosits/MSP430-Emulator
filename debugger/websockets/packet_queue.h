#pragma once

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

#include "../../main.h"

struct Packet {
  void *message;
  size_t length;
  uint8_t opcode;

  Packet *next;
};

void init_packet_queue (Emulator *emu);
bool packet_queue_empty (Emulator *emu);

void packet_enqueue (Emulator *emu, void *info, size_t len, uint8_t opcode);
Packet packet_dequeue (Emulator *emu);
void print_packet_queue (Emulator *emu);
