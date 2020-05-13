/*
  Simplistic bare-bones BSP for testing the emulated MSP430G2553 MCU
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static volatile uint8_t* IE1 = (uint8_t*)0x0000;
static volatile uint8_t* IFG1 = (uint8_t*)0x0002;
static volatile uint8_t* IE2 = (uint8_t*)0x0001;
static volatile uint8_t* IFG2 = (uint8_t*)0x0003;
static volatile uint8_t* ADC10DTC0 = (uint8_t*)0x0048;
static volatile uint8_t* ADC10DTC1 = (uint8_t*)0x0049;
static volatile uint8_t* ADC10AE0 = (uint8_t*)0x004A;
static volatile uint8_t* ADC10CTL0 = (uint8_t*)0x01B0;
static volatile uint8_t* ADC10CTL1 = (uint8_t*)0x01B2;
static volatile uint8_t* ADC10MEM = (uint8_t*)0x01B4;
static volatile uint8_t* ADC10SA = (uint8_t*)0x01BC;
static volatile uint8_t* DCOCTL = (uint8_t*)0x0056;
static volatile uint8_t* BCSCTL1 = (uint8_t*)0x0057;
static volatile uint8_t* BCSCTL2 = (uint8_t*)0x0058;
static volatile uint8_t* BCSCTL3 = (uint8_t*)0x0053;
static volatile uint8_t* CACTL1 = (uint8_t*)0x0059;
static volatile uint8_t* CACTL2 = (uint8_t*)0x005A;
static volatile uint8_t* CAPD = (uint8_t*)0x005B;
static volatile uint8_t* FCTL1 = (uint8_t*)0x0128;
static volatile uint8_t* FCTL2 = (uint8_t*)0x012A;
static volatile uint8_t* FCTL3 = (uint8_t*)0x012C;
static volatile uint8_t* P1IN = (uint8_t*)0x0020;
static volatile uint8_t* P1OUT = (uint8_t*)0x0021;
static volatile uint8_t* P1DIR = (uint8_t*)0x0022;
static volatile uint8_t* P1IFG = (uint8_t*)0x0023;
static volatile uint8_t* P1IES = (uint8_t*)0x0024;
static volatile uint8_t* P1IE = (uint8_t*)0x0025;
static volatile uint8_t* P1SEL = (uint8_t*)0x0026;
static volatile uint8_t* P1SEL2 = (uint8_t*)0x0041;
static volatile uint8_t* P1REN = (uint8_t*)0x0027;
static volatile uint8_t* P2IN = (uint8_t*)0x0028;
static volatile uint8_t* P2OUT = (uint8_t*)0x0029;
static volatile uint8_t* P2DIR = (uint8_t*)0x002A;
static volatile uint8_t* P2IFG = (uint8_t*)0x002B;
static volatile uint8_t* P2IES = (uint8_t*)0x002C;
static volatile uint8_t* P2IE = (uint8_t*)0x002D;
static volatile uint8_t* P2SEL = (uint8_t*)0x002E;
static volatile uint8_t* P2SEL2 = (uint8_t*)0x0042;
static volatile uint8_t* P2REN = (uint8_t*)0x002F;
static volatile uint8_t* P3IN = (uint8_t*)0x0018;
static volatile uint8_t* P3OUT = (uint8_t*)0x0019;
static volatile uint8_t* P3DIR = (uint8_t*)0x001A;
static volatile uint8_t* P3SEL = (uint8_t*)0x001B;
static volatile uint8_t* P3SEL2 = (uint8_t*)0x0043;
static volatile uint8_t* P3REN = (uint8_t*)0x0010;
static volatile uint8_t* TA0IV = (uint8_t*)0x012E;
static volatile uint8_t* TA0CTL = (uint8_t*)0x0160;
static volatile uint8_t* TA0CCTL0 = (uint8_t*)0x0162;
static volatile uint8_t* TA0CCTL1 = (uint8_t*)0x0164;
static volatile uint8_t* TA0CCTL2 = (uint8_t*)0x0166;
static volatile uint8_t* TA0R = (uint8_t*)0x0170;
static volatile uint8_t* TA0CCR0 = (uint8_t*)0x0172;
static volatile uint8_t* TA0CCR1 = (uint8_t*)0x0174;
static volatile uint8_t* TA0CCR2 = (uint8_t*)0x0176;
static volatile uint8_t* TA1IV = (uint8_t*)0x011E;
static volatile uint8_t* TA1CTL = (uint8_t*)0x0180;
static volatile uint8_t* TA1CCTL0 = (uint8_t*)0x0182;
static volatile uint8_t* TA1CCTL1 = (uint8_t*)0x0184;
static volatile uint8_t* TA1CCTL2 = (uint8_t*)0x0186;
static volatile uint8_t* TA1R = (uint8_t*)0x0190;
static volatile uint8_t* TA1CCR0 = (uint8_t*)0x0192;
static volatile uint8_t* TA1CCR1 = (uint8_t*)0x0194;
static volatile uint8_t* TA1CCR2 = (uint8_t*)0x0196;
static volatile uint8_t* UCA0CTL0 = (uint8_t*)0x0060;
static volatile uint8_t* UCA0CTL1 = (uint8_t*)0x0061;
static volatile uint8_t* UCA0BR0 = (uint8_t*)0x0062;
static volatile uint8_t* UCA0BR1 = (uint8_t*)0x0063;
static volatile uint8_t* UCA0MCTL = (uint8_t*)0x0064;
static volatile uint8_t* UCA0STAT = (uint8_t*)0x0065;
static volatile uint8_t* UCA0RXBUF = (uint8_t*)0x0066;
static volatile uint8_t* UCA0TXBUF = (uint8_t*)0x0067;
static volatile uint8_t* UCA0ABCTL = (uint8_t*)0x005D;
static volatile uint8_t* UCA0IRTCTL = (uint8_t*)0x005E;
static volatile uint8_t* UCA0IRRCTL = (uint8_t*)0x005F;
static volatile uint8_t* UCB0CTL0 = (uint8_t*)0x0068;
static volatile uint8_t* UCB0CTL1 = (uint8_t*)0x0069;
static volatile uint8_t* UCB0BR0 = (uint8_t*)0x006A;
static volatile uint8_t* UCB0BR1 = (uint8_t*)0x006B;
static volatile uint8_t* UCB0I2CIE = (uint8_t*)0x006C;
static volatile uint8_t* UCB0STAT = (uint8_t*)0x006D;
static volatile uint8_t* UCB0RXBUF = (uint8_t*)0x006E;
static volatile uint8_t* UCB0TXBUF = (uint8_t*)0x006F;
static volatile uint8_t* UCB0I2COA = (uint8_t*)0x0118;
static volatile uint8_t* UCB0I2CSA = (uint8_t*)0x011A;
static volatile uint8_t* WDTCTL = (uint8_t*)0x0120;
static volatile uint8_t* CALDCO_16MHZ = (uint8_t*)0x10F8;
static volatile uint8_t* CALBC1_16MHZ = (uint8_t*)0x10F9;
static volatile uint8_t* CALDCO_12MHZ = (uint8_t*)0x10FA;
static volatile uint8_t* CALBC1_12MHZ = (uint8_t*)0x10FB;
static volatile uint8_t* CALDCO_8MHZ = (uint8_t*)0x10FC;
static volatile uint8_t* CALBC1_8MHZ = (uint8_t*)0x10FD;
static volatile uint8_t* CALDCO_1MHZ = (uint8_t*)0x10FE;
static volatile uint8_t* CALBC1_1MHZ = (uint8_t*)0x10FF;

static inline void uart_init(void)
{
    // Reset start
    *UCA0CTL1 |= (uint8_t)0x1;
    *UCA0CTL0 = 0x00;
    *UCA0CTL1 = 0x01;
    // P1.1 is USCI UART IN
    // P1.2 is USCI UART OUT
    // Secondary peripheral function is selected
    // &= and |= is taken from TI tutorial
    *P1DIR = 0x04;
    *P1SEL |= 0x06;
    *P1SEL2 |= 0x06;
    *P1OUT &= 0x00;
    // Boud
    // Reset end
    *UCA0CTL1 &= ~(uint8_t)0x1;
}

static inline void uart_write(const uint8_t x)
{
    while ((*IFG2 & 0x02) == 0)
        asm volatile ("nop;");
    *UCA0TXBUF = x;
}

static inline void uart_write_string(char* x)
{
    while (*x != 0)
    {
        uart_write((uint8_t)*x);
        x++;
    }
}

static inline uint8_t uart_read(void)
{
    while ((*IFG2 & 0x01) == 0)
        asm volatile ("nop;");
    return *UCA0RXBUF;
}

static inline void uart_read_string(char* const x, const size_t maxSize, const char terminator)
{
    size_t count = 0;
    while (count < maxSize)
    {
        x[count] = uart_read();
        if (x[count] == terminator)
        {
            x[count] = 0;
            return;
        }
        count++;
    }
}
