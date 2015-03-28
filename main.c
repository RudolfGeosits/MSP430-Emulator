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
#include <stdbool.h>
#include <pthread.h>
#include <gtk/gtk.h>

#include "devices/memory/memspace.h"
#include "devices/cpu/registers.h"
#include "utils.h"
#include "devices/peripherals/setup.h"
#include "devices/cpu/decoder.h"
#include "debugger/disassembler.c"
#include "debugger/debugger.h"
#include "debugger/gui/gui.c"

int main(int argc, char *argv[])
{
  pthread_t gui_thread;
  
  if (argv[1] == NULL) {
    display_help();
    exit(1);
  }

  if( pthread_create(&gui_thread, NULL, gui, (void *)NULL ) ) {
    fprintf(stderr, "Error creating thread\n");
    return 1;
  }

  initialize_msp_memspace();
  initialize_msp_registers();
  ports_setup();

  load_program(argv[1], LOAD_POS);

  while (1) {            /* Fetch-Decode-Execute Cycle */
    command_loop();      /* Debugger */

    handle_port1();
    decode( fetch() );   /* Instruction Decoder */
  }

  uninitialize_msp_memspace();
  return 0;
}
