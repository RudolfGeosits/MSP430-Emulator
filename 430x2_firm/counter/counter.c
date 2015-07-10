#include <msp430g2452.h>

volatile unsigned char x = 0;
volatile unsigned char left_digit = 'H';
volatile unsigned char right_digit = 'I';
volatile int i;
unsigned char count_started = 0;

void display_digit(unsigned char num);
void left_context();
void right_context();
void mirror_context();

void main()
{
  WDTCTL = WDTPW + WDTHOLD; // Stopwatchdogtimer

  P2SEL &= 0x0;   // Set all P2 as GPIO
  P2DIR &= ~BIT1, P2DIR &= ~BIT3, P2DIR &= ~BIT4; // Set P2.1 and P2.3 as Input
  P2IES |= BIT1 + BIT3 + BIT4;  // Falling Edge 1 -> 0
  P2IFG &= 0x0; // Clear interrupt flag for P2.x
  P2REN |= BIT1 + BIT3 + BIT4;  // Turn on Pullup-Pulldown capability
  P2OUT |= BIT1 + BIT3 + BIT4;  // Pull Up

  P2IE |= BIT1 + BIT3 + BIT4; // Enable interrupt for P2.1 and P2.3
  __enable_interrupt(); // Enable Global Interrupts

  P2DIR |= 0b101; // P2.0 and P2.2 are Output
  P2OUT &= ~BIT0; // P2.0 = OFF left
  P2OUT &= ~BIT2 ; // P2.2 = OFF  right
 
  P1SEL &= 0x0; // Set P1.0 - P1.7 as GPIO
  P1DIR |= 0xFF; // Set P1.0 - P1.7 as OUTPUT
  P1OUT &= 0x0; // Set P1.0 - P1.7 as OFF

  while (1) {
    if (count_started) {
      right_digit = x % 10;
      left_digit = (x - (x % 10)) / 10;
    }

    right_context(); /* Draw Right first (dont ask) */
    display_digit(right_digit);
    for (i = 0;i < 333;i++);

    left_context();
    display_digit(left_digit);
    for (i = 0;i < 333;i++);
  } 
}

void left_context() 
{
  P2OUT &= ~BIT2; 
  P2OUT |= BIT0;  
}

void right_context() 
{
  P2OUT &= ~BIT0;
  P2OUT |= BIT2;
}

void mirror_context() {
  P2OUT |= BIT2 | BIT0;
}

void display_digit(unsigned char num)
{
  switch (num) {
    case 'O':
    case 0: {
      P1OUT = 0b01110111;
      break;
    }
    case 'I':
    case 1: {
      P1OUT = 0b00100100;
      break;
    }
    case 2: {
      P1OUT = 0b01011101;
      break;
    }
    case 3: {
      P1OUT = 0b01101101;
      break;
    }
    case 4: {
      P1OUT = 0b00101110;
      break;
    }
    case 5: {
      P1OUT = 0b01101011;
      break;
    }
    case 6: {
      P1OUT = 0b01111011;
      break;
    }
    case 7: {
      P1OUT = 0b00100101;
      break;
    }
    case 8: {
      P1OUT = 0b01111111;
      break;
    }
    case 9: {
      P1OUT = 0b00101111;
      break;
    }
    case 'E': {
      P1OUT = 0b01011011;
      break;
    }
    case 'H': {
      P1OUT = 0b00111110;
      break;
    }
    case 'L': {
      P1OUT = 0b01010010;
      break;
    }
    case 'A': {
      P1OUT = 0b00111111;
      break;
    }

  }
}


//Port1 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2()
{
  if (P2IFG & BIT1) {  // Up
    if (++x == 100) {
      x = 0;
    }

    count_started = 1;
  }
  else if (P2IFG & BIT3) {  // Down
    if (count_started && x > 0) {
      x--;
    }
  }

  P2IFG &= 0x0; // Clear All P2 Interrupts
  for (i = 0;i < 333;i++);
}
