/*
  MSP430 Emulator
  Copyright (C) 2014, 2015 Rudolf Geosits (rgeosits@live.esu.edu)
  
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "devices/memory/memspace.h"
#include "devices/cpu/registers.h"
#include "utils.h"
#include "debugger/debugger.h"
#include "devices/cpu/decoder.h"

int main(int argc, char **argv)
{
  if (argv[1] == NULL) {
    display_help();
    exit(1);
  }

  initialize_msp_memspace();
  initialize_msp_registers();
  load_program(argv[1], LOAD_POS);

  while (1) { /* CPU Fetch-Decode-Execute */
    decode( fetch() );
    command_loop();
  }

  uninitialize_msp_memspace();
  return 0;
}
