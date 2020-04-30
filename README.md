MSP430 Emulator
===============

This is a fork of the MSP430 Emulator created by Rudolf Geosits and available at: https://github.com/RudolfGeosits/MSP430-Emulator. Please visit that site for more information.

The main purpose of the fork is to re-enable a pure command-line usage, for deployment in Continuous Integration testing environments.

The emulator's feature-set is based on MSP430G2553 MCU.

This fork contains the following changes with respect to the parent project:
- Main function rewrite to support more command line arguments and command-line usage
- USCI module rewrite so that is supports both transmission and reception to/from pipes/files
- Memory access flags to enable implementation of peripherals that act upon a register read/write
- Automated integration tests for validating basic usage scenarios
- Dockerfile for reproducible, host-independend builds
- Some bugfixes (see futher below) and code clean-up to remove basic warnings

The following things have been tested to work (please consult the test folder):
- Function calls
- Basic arithmetic (including libc emulation of addition and division)
- Basic string operations (sprintf)
- USCI I/O in UART mode (no interrupts, no baud rate control, only 8-bit)

Major limitations:
- No proper interrupt support (RTOS will not work)
- No memory layout/size configuration (MSP430G2553 layout is hardcoded)
- only MSP430 instruction set (no MSP430X support, so proper simulation of MSP430FR5969 is not possible)

The following bugfixes have been implemented:
- Pointer to SR register now points to the actual register, not a proxy (resolves issue with CLCR instruction emulated via BIC with SR as an argument)
- Reset starts execution from the address pointed to by the reset vector instead of 0xC000 (programs compiled with TI GCC usually do not start at 0xC000)

--------------------------------------------------------------------------------------------------------------------------

- Build Instructions (via make):
  - Install dependancies via ./install_deps.sh
  - Navigate to the root of the source tree
  - Type 'make all' to build the emulator
  - Type 'make test' to build and run the integration tests

- Build Instructions (via docker):
  - Navigate to the root of the source tree
  - Type 'docker build --tag YOURTAG .'

- Sample usage (please consult the test folder):
  - Bin file can be prepared from elf file using 'msp430-objcopy -O binary SOURCE_ELF TARGET_BIN'; elf file is created by compiling a program using MSP430 GCC
  - Type './msp430-emu -m cli -b BIN_FILE_TO_LOAD -i UART_INPUT_PIPE -o UART_OUTPUT_PIPE' to start the emulator in command line mode, load the BIN_FILE_TO_LOAD, read USCI UART data from UART_INPUT_PIPE and write USCI UART data to UART_OUTPUT_PIPE
  - NOTE: if pipes are used, they must be opened by a sink/source program beforehand; fopen waits for the other end of communication to be available; this limitation does not apply if regular files are used
  - (inside emulator) type 'help' for available commands
  - (inside emulator) type 'reset' to invoke a reset, which sets up the correct PC value (not needed for programs with entry point @0xC000)
  - (inside emulator) type 'run' to run the program until a breakpoint hits, an error occurrs or the end of time; type 's 1000' to run 1000 instructions of the program
  - (inside emulator) type 'quit' to quit the emulator

