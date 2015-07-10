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
#include <signal.h>
#include <stdbool.h>
#include <pthread.h>
#include <gtk/gtk.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "devices/memory/memspace.h"
#include "devices/cpu/registers.h"
#include "devices/utilities.h"
#include "devices/peripherals/setup.h"
#include "devices/cpu/decoder.h"
#include "debugger/disassembler.h"
#include "debugger/debugger.h"
#include "debugger/gui/gui.c"

int main(int argc, char *argv[])
{
  Cpu *msp430 = (Cpu *) malloc( sizeof(Cpu) );
  pthread_t gui_thread;
  
  if (argv[1] == NULL) {
    display_help();
    exit(1);
  }
  
  if( pthread_create(&gui_thread, NULL, gui, (void *)NULL ) ) {
    fprintf(stderr, "Error creating thread\n");
    return 1;
  }

  register_signal(SIGINT);
  initialize_msp_memspace();
  initialize_msp_registers(msp430);
  ports_setup();
  
  load_firmware(argv[1], LOAD_POS);

  /* Fetch-Decode-Execute Cycle */
  while ( command_loop(msp430) ) {
    handle_port1();
    decode( msp430, fetch(msp430) );   /* Instruction Decoder */
    usleep(10);
  }
  
  uninitialize_msp_memspace(msp430);
  free(msp430);
  return 0;
}
