#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "bsp.h"

static uint16_t doCalculation(uint32_t a, uint32_t b)
{
    // 17 * (17 + 111) % 255 == (17 * 128) % 255 == 2176 % 255 == 136
    const uint32_t result = a * (a + b) % 255;
    // 136 / 3 == 45
    const uint32_t result2 = result / 3;
    // 45 - 1 == 44
    return (uint16_t)result2 - 1;
}

void main()
{
    uart_init();
    char buffer[8];
    const uint32_t a = 17;
    const uint32_t b = 111;
    const uint16_t result = doCalculation(a,b);
    uart_write_string("Test calculation from ");
    sprintf(buffer, "%d", a);
    uart_write_string(buffer);
    uart_write_string(" and ");
    sprintf(buffer, "%d", b);
    uart_write_string(buffer);
    uart_write_string(" yields ");
    sprintf(buffer, "%d", result);
    uart_write_string(buffer);
    while (true)
        asm volatile ("nop");
}