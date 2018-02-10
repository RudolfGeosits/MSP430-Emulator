MSP430 Emulator
===============

<<<<<<< HEAD

You can use this emulator at: http://www.msp430emulator.com/emu.html
=======
You can use this emulator online now at: http://www.msp430emulator.com/emu.html
>>>>>>> 85b1fb4170098e5d5042ca1a1ee50fe60e506a46

- Providing a complete software model of the MSP430 16-bit instruction set
- An interactive debugger for advanced development and in depth firmware/hardware analysis
- Peripherals include:
  - GPIO Ports (LEDs and other pins)
  - UART Serial Communication (via USCI Module) 
  - Timer_A (in progress)
  - BCM+ (in progress)

  The project goal is to emulate all peripherals and devices on the TI MSP430 Launchpad starter kit, 
  to be able to run all firmware that would run on the physical device and test hardware inputs like UART or some of the other digital ports via programmable means. 
  The emulator/simulator written in C is server which can be ran as a daemon. The user interface is accessed through the browser. This server and interface is available online at the link above.
  
  Please feel free to contribute! (TODO list below!) Also, we would like to hear from you if you are using this software! 

  Please contact me at rgeosits@live.esu.edu if you are using this for something or would like more information on how to contribute. Let me know if you need something specific emulated!
  Thank you!
  
[![cap2.png](https://s9.postimg.org/he4a72qfz/cap2.png)]
  
  --------------------------------------------------------------------------------------------------------------

- TODO
  - Basic Clock Module / Timer
  
  - Instructions
    - DADD (BCD math)
    - RETI (Return from Interrupt)

- Build Instructions (You will need the following packages)
<<<<<<< HEAD
  - Install dependancies via ./install_deps.sh
=======
  - readline (apt-get install libreadline-dev)
  - msp430*  (apt-get install msp430*)
  - libwebsockets-dev (apt-get install libwebsockets-dev)

  - ( sudo apt-get install libreadline-dev msp430* libwebsockets-dev; )

>>>>>>> 85b1fb4170098e5d5042ca1a1ee50fe60e506a46
  - navigate to the root of the source tree
  - type 'make' (you can also make clean after)

- User Instructions
<<<<<<< HEAD
  - Log onto http://www.msp430emulator.com/emu.html to use the online emulation server.

  - How to use a local instance of this server (typically for time sensitive applications where latency would cause errors in accuracy)
    - Build this emulation server (instructions above)
    - Run the emulation server on your machine (./server)
    - Log onto http://www.msp430emulator.com/emu.html and select the checkbox for "Run Local" at the bottom left of the interface.
    - You will be connected! Enjoy.
=======
  - Log onto http://www.msp430emulator.com/emu.html to use the ONLINE Emulator

  - The following are instructions on how to use a LOCAL web interface
    - Click local debugging in the online emulator (Not yet available)
    - You will be connected! Enjoy.
>>>>>>> 85b1fb4170098e5d5042ca1a1ee50fe60e506a46
