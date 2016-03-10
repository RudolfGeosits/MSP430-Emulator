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

#include "main.h"

int main(int argc, char *argv[])
{
  Emulator *emu = (Emulator *) calloc( 1, sizeof(Emulator) );

  emu->cpu = (Cpu *) calloc( 1, sizeof(Cpu) );
  emu->cpu->p1 = (Port *) calloc ( 1, sizeof(Port) );
  emu->cpu->usci = (Usci *) calloc ( 1, sizeof(Usci) );
  
  emu->debugger = (Debugger *) calloc (1, sizeof(Debugger) );
  setup_debugger(emu);
  
  Cpu *cpu = emu->cpu;
  Debugger *debugger = emu->debugger;

  if (argv[1] == NULL) {
    display_help(emu);
    exit(1);
  }

  if (debugger->web_interface == true) {
    pthread_t *t = &debugger->web_server_thread;

    if(pthread_create( t, NULL, web_server, (void *)emu )) {
      fprintf(stderr, "Error creating web server thread\n");
      return 1;
    }

    printf("Waiting for web client to connect...\n");
    while (!debugger->web_server_ready) usleep(1000);
  }
  else if (debugger->console_interface == true) {
    puts("console mode not implemented.");
  }
  
  register_signal(SIGINT);
  initialize_msp_memspace();
  initialize_msp_registers(emu);  
  setup_port_1(emu);
  setup_usci(emu);
  //open_pty(emu);

  load_bootloader(0x0C00);
  load_firmware(argv[1], 0xC000);

  // display first round of registers
  display_registers(emu);
  disassemble(emu, cpu->pc, 1);

  // Fetch-Decode-Execute Cycle
  while (!debugger->quit) {
    if (!debugger->run) {usleep(1000);continue;}
    // debugger called here or in server callback
    //if (debugger->console_interface) command_loop(emu);
    //command_loop(emu);

    decode(emu, fetch(emu), EXECUTE); // Instruction Decoder
    handle_port_1(emu);
    handle_usci(emu);
  }

  uninitialize_msp_memspace(emu->cpu);
  free(cpu->p1);
  free(cpu->usci);
  free(cpu);
  free(debugger);
  free(emu);

  return 0;
}
