#include "stdio.h"
#include "stdlib.h"
#include "string.h"

char program_name[50];

void load_program(char*, unsigned char*);
void reg_num_to_name(unsigned int, char*);

#include "utils.c"
#include "devices/memory/memspace.c"
#include "devices/cpu/registers.c"
#include "debugger/debugger.c"
#include "devices/cpu/decoder.c"

int main(char** argv, int argc){

  initialize_msp_memspace();
  initialize_msp_registers();

  printf("Program Name: ");
  scanf("%s", program_name);

  load_program(program_name, ROM);

  //# Main CPU fetch/decode/execute
  while(1){
    decode( fetch() );
    command_loop();
  }

  uninitialize_msp_memspace();

  return 0;
}
