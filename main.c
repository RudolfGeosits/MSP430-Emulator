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

int64_t timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
    ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}

int main(int argc, char *argv[])
{
  Emulator *emu = (Emulator *) calloc( 1, sizeof(Emulator) );
  Cpu *cpu = NULL;
  Debugger *deb = NULL;

  emu->cpu = (Cpu *) calloc(1, sizeof(Cpu));
  emu->cpu->p1 = (Port_1 *) calloc(1, sizeof(Port_1));
  emu->cpu->usci = (Usci *) calloc(1, sizeof(Usci));  

  emu->debugger = (Debugger *) calloc(1, sizeof(Debugger));
  setup_debugger(emu);  
  
  cpu = emu->cpu;
  deb = emu->debugger;

  deb->server = (Server *) calloc(1, sizeof(Server));  


  if (deb->web_interface == true) {
    if (argv[1] == NULL) {
      puts("Need port argument");
      return(1);
    }
    
    deb->ws_port = strtoul(argv[1], NULL, 10);

    pthread_t *t = &deb->web_server_thread;

    if(pthread_create( t, NULL, web_server, (void *)emu )) {
      fprintf(stderr, "Error creating web server thread\n");
      return 1;
    }
    
    printf("SERVER: Waiting for web client to connect...\n");
    while (!deb->web_server_ready) usleep(100);

    printf("SERVER: Waiting for web client to upload firmware...\n");
    web_send("MSP430 Emulator\nCopyright (C) 2016 Rudolf Geosits (rgeosits@live.esu.edu)\n\n", 
	     STDOUT);

    web_send("[!] Please upload your firmware (waiting)\n", STDOUT);
    while (!deb->web_firmware_uploaded) usleep(1000);
  }
  else if (deb->console_interface) {
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

  // Open Pseudo terminal in console mode for Serial
  //if (deb->console_interface) open_pty(emu);
  
  load_bootloader(0x0C00);

  if (deb->console_interface)
    load_firmware(argv[1], 0xC000);
  else if (deb->web_interface)
    load_firmware("tmp.bin", 0xC000);

  // display first round of registers
  display_registers(emu);
  disassemble(emu, cpu->pc, 1);

  // Fetch-Decode-Execute Cycle (run machine)
  while (!deb->quit) {
    struct timespec start, end;

    // Handle debugger
    if (!deb->run) {usleep(1000);continue;}
    handle_breakpoints(emu);

    // Instruction Decoder
    decode(emu, fetch(emu), EXECUTE); 

    // Handle Peripherals
    handle_port_1(emu);
    handle_usci(emu);

    // Get close to 1.1 MHZ
    clock_gettime(CLOCK_MONOTONIC, &start);

    while (true) {
      clock_gettime(CLOCK_MONOTONIC, &end);
      uint64_t timeElapsed = timespecDiff(&end, &start);
      
      if (timeElapsed >= 900) break;
    }
  }
  
  uninitialize_msp_memspace(emu->cpu);
  free(cpu->p1);
  free(cpu->usci);
  free(cpu);
  free(deb->server);
  free(deb);
  free(emu);

  return 0;
}
