void initialize_msp_memspace();
void uninitialize_msp_memspace();

uint8_t *MEMSPACE;   /* Memory Space */
uint8_t *IVT;        /* Interrupt Vector Table {Within ROM} */
uint8_t *ROM;        /* Flash/Read-Only memory */
uint8_t *RAM;        /* Random Access Memory */
uint8_t *PER16;      /* 16-bit peripherals */
uint8_t *PER8;       /* 8-bit peripherals */
uint8_t *SFRS;       /* Special Function Registers */

#include "memspace.c"
