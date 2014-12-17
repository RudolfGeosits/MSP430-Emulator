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
  load_program(argv[1], ROM);

  while (1) { /* CPU Fetch-Decode-Execute */
    decode( fetch() );
    command_loop();
  }

  uninitialize_msp_memspace();
  return 0;
}
