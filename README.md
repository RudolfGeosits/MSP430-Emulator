MSP430-Emulator
===============

- Currently providing a complete software model of the MSP430 instruction set
- An interactive debugger for advanced exploit development and analysis
- Peripheral port 1 is currently being configured

  The project goal is to simulate all peripherals and devices on the TIMSP430 Launchpad starter kit, 
  to be able to test run software and fuzz hardware inputs like UART or some of the other 
  digital ports via programmable means. Then moving on to a more device agnostic end game. We would 
  like to develop a web interface for remote debugging as well.
  
  Ultimately, we would like to develop a simple and configurable framework that can handle many architectures.
  If you think this sounds fun, please feel free to contribute! (TODO list below!)
  
  <img>http://s2.postimg.org/hti7fnwbd/emulator_screengrab.png</img>

- TODO

  - Disassembly Coloring
  - Basic Clock Module Emulation

  - Instructions
    - SUB (Fix SUB.B overflow flag)
    - CMP (Fix CMP.B overflow flag)
    - DADD (BCD math)    
    - RETI (Return from Interrupt)
