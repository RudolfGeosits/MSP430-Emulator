MSP430 Emulator
===============

You can use this emulator at: http://www.poorhackers.com/emu.html

NOTE: We are in the process of moving to a web interface!

- Providing a complete software model of the MSP430 16-bit instruction set
- An interactive debugger for advanced development and in depth firmware/hardware analysis
- Peripherals include:
  - UART Serial Communication (via USCI Module) 
  - GPIO Ports (LEDs and other pins)

  The project goal is to emulate all peripherals and devices on the TI MSP430 Launchpad starter kit, 
  to be able to run all firmware that would run on the physical device and test hardware inputs like UART or some of the other digital ports via programmable means. 
  Then moving on to a more device agnostic end game. The C program is basically a server while the user interface is accessed through the browser. This will be put online soon.
  
  Ultimately, we would like to develop a simple and configurable framework that can handle many architectures.
  If you think this sounds fun, please feel free to contribute! (TODO list below!) Also, we would like to hear from you! 
  Please contact me at rgeosits@live.esu.edu if you are using this for something or would like more information on how to contribute.
  
  ![Screen Grab](http://s14.postimg.org/oc29wu3e9/Screenshot_from_2015_08_22_20_08_24.png "Screen Grab")

- TODO
  - Support for ELF binary format
  - Basic Clock Module
  
  - Instructions
    - DADD (BCD math)    
    - RETI (Return from Interrupt)

- Build Instructions (You will need the following packages)
  - readline (apt-get install libreadline-dev)
  - msp430*  (apt-get install msp430*)
  - libwebsockets-dev (apt-get install libwebsockets-dev)

  - navigate to the root of the source tree
  - type 'make' (you can also make clean after)

- User Instructions
  - Log onto www.poorhackers.com/emu.html to use the ONLINE web interface

  - The following are instructions on how to use a LOCAL web interface
    - run the server via ./SERVER
    - load index.html from debugger/webclient/index.html to your browser
    - You will be connected