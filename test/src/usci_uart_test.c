#include <stdio.h>
#include "bsp.h"

#define BUFFER_SIZE 128

void main()
{
    char buffer[BUFFER_SIZE];
    uart_init();
    uart_write_string("Hello there!");

    uart_read_string(buffer, BUFFER_SIZE, ';');
    uart_write_string("Echo - ");
    uart_write_string(buffer);
    while (true)
        asm volatile ("nop");
}