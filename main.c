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

#include "main.h"

int main(int argc, char *argv[])
{
  Emulator *emu = (Emulator *) calloc( 1, sizeof(Emulator) );
  Cpu *cpu = NULL;
  Debugger *deb = NULL;

  emu->cpu = (Cpu *) calloc( 1, sizeof(Cpu) );
  emu->cpu->p1 = (Port *) calloc ( 1, sizeof(Port) );
  emu->cpu->usci = (Usci *) calloc ( 1, sizeof(Usci) );  

  emu->debugger = (Debugger *) calloc (1, sizeof(Debugger) );
  setup_debugger(emu);
  
  cpu = emu->cpu;
  deb = emu->debugger;

  if (deb->web_interface == true) {
    pthread_t *t = &deb->web_server_thread;

    if(pthread_create( t, NULL, web_server, (void *)emu )) {
      fprintf(stderr, "Error creating web server thread\n");
      return 1;
    }
    
    printf("SERVER: Waiting for web client to connect...\n");
    while (!deb->web_server_ready) usleep(100);
    
    printf("SERVER: Waiting for web client to upload firmware...\n");
    web_send("MSP430 Emulator\nCopyright (C) 2016 Rudolf Geosits (rgeosits@live.esu.edu)\n\n");
    web_send("[!] Please upload your firmware (waiting)\n");
    while (!deb->web_firmware_uploaded) usleep(100);
  }
  else if (deb->console_interface == true) {
    puts("console mode not implemented.");

    if (argv[1] == NULL) {
      display_help(emu);
      exit(1);
    }

    register_signal(SIGINT); // Register Callback for CONTROL-c
  }  
  
  initialize_msp_memspace();
  initialize_msp_registers(emu);  
  setup_port_1(emu);
  setup_usci(emu);
  //open_pty(emu);

  load_bootloader(0x0C00);
  //load_firmware(argv[1], 0xC000);
  load_firmware("tmp.bin", 0xC000);

  // display first round of registers
  display_registers(emu);
  disassemble(emu, cpu->pc, 1);

  // Fetch-Decode-Execute Cycle (run machine)
  while (!deb->quit) {
    if (!deb->run) {usleep(100);continue;}
    
    decode(emu, fetch(emu), EXECUTE); // Instruction Decoder
    handle_port_1(emu);
    handle_usci(emu);
  }
  
  uninitialize_msp_memspace(emu->cpu);
  free(cpu->p1);
  free(cpu->usci);
  free(cpu);
  free(deb);
  free(emu);

  return 0;
}
