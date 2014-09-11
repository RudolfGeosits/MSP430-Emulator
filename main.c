#include "stdio.h"
#include "stdlib.h"
#include "string.h"

char program_name[50];
unsigned char *FLASH, *RAM;

void load_program(char*, unsigned char*);
void reg_num_to_name(unsigned int, char*);

#include "cpu/msp_registers.c"
#include "utils.c"
#include "debugger.c"
#include "cpu/decoder.c"

int main(char** argv, int argc){
  initialize_msp_registers();

  printf("Program Name: ");
  scanf("%s", program_name);

  FLASH = (char*)malloc( 32000 ); //# Really 16000
  memset(FLASH, 0, 32000);
  RAM = (char*)malloc( 512 ); //# Really 512
  memset(RAM, 0, 512);

  load_program(program_name, FLASH);

  decoder(FLASH);

  free(FLASH);
  free(RAM);
  return 0;
}

void load_program(char* program_name, unsigned char* MEM){
  int size, result;
  unsigned char byte;
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
