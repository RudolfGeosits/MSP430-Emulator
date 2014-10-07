#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "devices/memory/memspace.c"
#include "devices/cpu/registers.c"
#include "utils.c"
#include "debugger/debugger.c"
#include "devices/cpu/decoder.c"

int main(int argc, char** argv)
{
  if(argv[1] == NULL){
    display_help();
    exit(1);
  }

  initialize_msp_memspace();
  initialize_msp_registers();

  load_program( argv[1], ROM );

  //# Main CPU fetch/decode/execute
  while(1){
    decode( fetch() );
    command_loop();
  }

  uninitialize_msp_memspace();
  return 0;
}
