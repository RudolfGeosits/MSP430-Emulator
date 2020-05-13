#include "bsp.h"

void main()
{
    // Set output direction
    *P1DIR = 0x01;
    // Test sequence
    *P1OUT = 0x00;
    *P1OUT = 0x01;
    *P1OUT = 0x00;
    while (true)
        asm volatile ("nop");
}