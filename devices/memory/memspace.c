
#include "memspace.h"

/* Initialize Address Space Locations
** 
** Allocate and set MSP430 Memory space
** Some of these locations vary by model
*/
void initialize_msp_memspace()
{
  // 64 KB Addressable Space
  MEMSPACE = (uint8_t *) malloc(0x1FFFF);
  //MEMSPACE = (uint8_t *) malloc(0x4000);
  
  // Info memory from 0x1000 - 0x10FF
  IVT = MEMSPACE +   0xFFE0;   // 0xFFC0 - 0xFFFF
  ROM = MEMSPACE +   0x0400;   // 0x400 - 0x1FFFF
  RAM = MEMSPACE +   0x0200;   // 0x200 - 0x3FF
  PER16 = MEMSPACE + 0x0100;   // 0x0100 - 0x01FF
  PER8 = MEMSPACE +  0x0010;   // 0x0010 - 0x00FF
  SFRS = MEMSPACE +  0x0;      // 0x0 - 0x0F
}

/* 
** Free MSP430 virtual memory
*/
void uninitialize_msp_memspace()
{
  free(MEMSPACE);
}
