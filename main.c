#include "stdio.h"
#include "stdlib.h"
#include "string.h"

char program_name[50];

void load_program(char*, unsigned char*);
void reg_num_to_name(unsigned int, char*);

#include "devices/memory/memspace.c"
#include "devices/cpu/registers.c"
#include "utils.c"
#include "debugger/debugger.c"
#include "devices/cpu/decoder.c"

int main(char** argv, int argc){
  initialize_msp_memspace();
  initialize_msp_registers();

  printf("Program Name: ");
  scanf("%s", program_name);

  load_program(program_name, ROM);
  decode(ROM);

  uninitialize_msp_memspace();
  return 0;
}

//##########+++ Load binary into memory +++##########
void load_program(char* program_name, unsigned char* MEM){
  int size, result;
  printf("Executing Program name: %s\n", program_name);
  FILE* fd = fopen(program_name, "rb+");
  
  // obtain file size
  fseek(fd, 0, SEEK_END);
  size = ftell(fd);
  rewind(fd);

  result = fread(MEM, 1, size, fd);
  printf("Placed %d bytes into flash\n\n\n", result);

  fclose(fd);
}
