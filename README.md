MSP430 Emulator
===============


You can use this emulator at: http://www.msp430emulator.com/emu.html

- Providing a complete software model of the MSP430 16-bit instruction set
- An interactive debugger for advanced development and in depth firmware/hardware analysis
- Peripherals include:
  - UART Serial Communication (via USCI Module) 
  - GPIO Ports (LEDs and other pins)

  The project goal is to emulate all peripherals and devices on the TI MSP430 Launchpad starter kit, 
  to be able to run all firmware that would run on the physical device and test hardware inputs like UART or some of the other digital ports via programmable means. 
  The C program is basically a server while the user interface is accessed through the browser. This server and interface is available online at the link above.
  
  Please feel free to contribute! (TODO list below!) Also, we would like to hear from you if you are using this software! 

  Please contact me at rgeosits@live.esu.edu if you are using this for something or would like more information on how to contribute. Let me know if you need something specific emulated!

  We appreciate all those who support this open sourced emulation project, for both educational
  and professional purposes. Your contributions will go directly to time worked on this project.

  Thank you!
  
  --------------------------------------------------------------------------------------------------------------

- TODO
  - Basic Clock Module / Timer
  
  - Instructions
    - DADD (BCD math)    
    - RETI (Return from Interrupt)

- Build Instructions (You will need the following packages)
  - Install dependancies via ./install_deps.sh
  - navigate to the root of the source tree
  - type 'make' (you can also make clean after)

- User Instructions
  - Log onto http://www.msp430emulator.com/emu.html to use the online emulation server.

  - How to use a local instance of this server (typically for time sensitive applications where latency would cause errors in accuracy)
    - Build this emulation server (instructions above)
    - Run the emulation server on your machine (./server)
    - Log onto http://www.msp430emulator.com/emu.html and select the checkbox for "Run Local" at the bottom left of the interface.
    - You will be connected! Enjoy.