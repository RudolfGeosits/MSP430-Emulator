MSP430 Emulator
===============

You can use this emulator online now at: https://msp430.online

- Providing a complete software model of the MSP430 16-bit instruction set
- An interactive debugger for advanced development and in depth firmware/hardware analysis
- Peripherals include:
  - GPIO Ports (LEDs and other pins)
  - UART Serial Communication (via USCI Module)
  - PWM Servo Motor Control
  - Timer_A (in progress)
  - BCM+ (in progress)

  The project goal is to emulate all peripherals and devices on the TI MSP430 Launchpad starter kit, 
  to be able to run all firmware that would run on the physical device and programmatically test hardware inputs like    UART / other digital ports. The emulator is written in C/C++ and acts as an API to the MCU.
  
  Please contact rgeosits@live.esu.edu if you are interested in using this tool for educational or industrial purposes.
  Thank you!
  
![Interface.PNG](https://msp430.online/images/Interface.PNG)
  
--------------------------------------------------------------------------------------------------------------------------

- Build Instructions for API
  - Install dependencies via ./install_deps.sh
  - navigate to the root of the source tree
  - type 'make'

- User Instructions for online interface
  - Log onto https://msp430.online to use the online simulator!

- Documentation & Sample Programs:
  - Sample Programs: https://msp430.online/examples.html
  - User Guide: http://msp430.online/help.html

- TODO
  - Basic Clock Module / Timer  
  - Instructions
    - DADD (BCD math)
    - RETI (Return from Interrupt)
