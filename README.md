MSP430-Emulator
===============

- Currently providing a complete software model of the MSP430 instruction set
- An interactive debugger for advanced development and software/hardware analysis
- Peripheral port 1 is currently being configured

  The project goal is to emulate all peripherals and devices on the TIMSP430 Launchpad starter kit, 
  to be able to run all firmware that would run on the physical device and test hardware inputs like UART or some of the other digital ports via programmable means. 
  Then moving on to a more device agnostic end game. We would like to develop a web interface for remote debugging as well.
  
  Ultimately, we would like to develop a simple and configurable framework that can handle many architectures.
  If you think this sounds fun, please feel free to contribute! (TODO list below!) Also, we would like to hear from you! 
  Please contact me at rgeosits@live.esu.edu if you are using this for something or would like more information on how to contribute.
  
  ![Screen Grab](http://s14.postimg.org/6wzj6wydd/MSP3.png "Screen Grab")

- TODO

  - Support for ELF binary format
  - Disassembly Coloring and structure
  - Basic Clock Module
  - GTK GUI needs to be improved  

  - Instructions
    - DADD (BCD math)    
    - RETI (Return from Interrupt)

- Build Instructions
  - You will need the following packages:

  - readline (apt-get install libreadline-dev)
  - gtk2.0 (apt-get install gtk2.0-examples)
