#include "memspace.h"

unsigned char* MEMSPACE;
unsigned char* IVT;
unsigned char* ROM;
unsigned char* RAM;
unsigned char* PER16;
unsigned char* PER8;
unsigned char* SFRS;

void initialize_msp_memspace()
{
  MEMSPACE = (unsigned char*) malloc(0xFFFF);
  
  IVT = MEMSPACE + 0xFFE0;
  ROM = MEMSPACE + 0x0400;
  RAM = MEMSPACE + 0x0200;
  PER16 = MEMSPACE + 0x0100;
  PER8 = MEMSPACE + 0x0010;
  SFRS = MEMSPACE + 0x0;
}

void uninitialize_msp_memspace()
{
  free(MEMSPACE);
}
