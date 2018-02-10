/*
  MSP430 Emulator
  Copyright (C) 2018 Rudolf Geosits (rgeosits@live.esu.edu)
  
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
  Cpu *cpu = NULL; Debugger *deb = NULL;

  emu->cpu       = (Cpu *) calloc(1, sizeof(Cpu));
  emu->cpu->bcm  = (Bcm *) calloc(1, sizeof(Bcm));
  emu->cpu->timer_a  = (Timer_a *) calloc(1, sizeof(Timer_a));
  emu->cpu->p1   = (Port_1 *) calloc(1, sizeof(Port_1));
  emu->cpu->usci = (Usci *) calloc(1, sizeof(Usci));  

  emu->debugger  = (Debugger *) calloc(1, sizeof(Debugger));
  setup_debugger(emu);    

  cpu = emu->cpu;
  deb = emu->debugger;

  deb->server    = (Server *) calloc(1, sizeof(Server));

  if (deb->web_interface == true) {
    if (argv[1] == NULL) {
      puts("Need port argument");
      return(1);
    }
    
    deb->ws_port = strtoul(argv[1], NULL, 10);

    pthread_t *t = &deb->web_server_thread;

    if ( pthread_create(t, NULL, web_server, (void *)emu ) ) {
      fprintf(stderr, "Error creating web server thread\n");
      return 1;
    }
    
    while (!deb->web_server_ready) usleep(10000);

    print_console(emu, "MSP430 Emulator\nCopyright (C) 2018 Rudolf"\
		  " Geosits (rgeosits@live.esu.edu)\n\n");

    print_console(emu, "[!] Upload your firmware (ELF format only)\n");
    while (!deb->web_firmware_uploaded) usleep(10000);
  }

  //register_signal(SIGINT); // Register Callback for CONTROL-c
  
  initialize_msp_memspace();
  initialize_msp_registers(emu);  

  setup_bcm(emu);
  setup_timer_a(emu);
  setup_port_1(emu);
  setup_usci(emu);
  
  load_bootloader(0x0C00);

  if (deb->web_interface)
    load_firmware(emu, "tmp.bin", 0xC000);
  
  // display first round of registers
  display_registers(emu);
  disassemble(emu, cpu->pc, 1);
  update_register_display(emu);

  // Fetch-Decode-Execute Cycle (run machine)
  while (!deb->quit) {
    // Handle debugger when CPU is not running
    if (!cpu->running) { usleep(10000); continue; }

    // Handle Breakpoints
    handle_breakpoints(emu);
    
    // Instruction Decoder
    decode(emu, fetch(emu), EXECUTE); 
    
    // Handle Peripherals
    handle_bcm(emu);
    handle_timer_a(emu);
    handle_port_1(emu);
    handle_usci(emu);

    // Average of 4 cycles per instruction
    mclk_wait_cycles(emu, 4);
  }

  uninitialize_msp_memspace(emu->cpu);
  free(cpu->timer_a);
  free(cpu->bcm);
  free(cpu->p1);
  free(cpu->usci);
  free(cpu);
  free(deb->server);
  free(deb);
  free(emu);

  return 0;
}
