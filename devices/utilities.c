/*                                                                
MSP430 Emulator                                                   
Copyright (C) 2016 Rudolf Geosits (rgeosits@live.esu.edu)                
This program is free software: you can redistribute it and/or modify           
it under the terms of the GNU General Public License as published by           
the Free Software Foundation, either version 3 of the License, or              
(at your option) any later version.                                            
                                                                               
This program is distributed in the hope that it will be useful,                
but WITHOUT ANY WARRANTY; without even the implied warranty of    
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  
GNU General Public License for more details.                      
                                                                
You should have received a copy of the GNU General Public License 
along with this program. If not, see <http://www.gnu.org/licenses             
*/

#include "utilities.h"

/**
 * @brief This function loads the default TI bootloader code into virtual mem
 * @param virt_addr The location in virtual memory to load the bootloader
 */
void load_bootloader(uint16_t virt_addr)
{
  uint16_t *real_addr = get_addr_ptr(virt_addr);
  
  memmove(real_addr, blc, sizeof blc);
  printf("Loaded booloader code into address 0x%04X\n", virt_addr);
}

/**
 * @brief This function loads firmware from a binary file on disk into the
 * virtual memory of the emulated device at base virt_loc
 * @param file_name The file name of the binary to load into virtual memory
 * @param virt_loc The location in virtual memory to load the firmware
 */
void load_firmware(Emulator *emu, char *file_name, uint16_t virt_addr)
{
  uint32_t size, result;
  char str[100] = {0};

  sprintf(str, "Loading firmware: ( %s )\n", file_name);
  
  printf("%s", str);
  print_console(emu, str);

  FILE *fd = fopen(file_name, "rb+");
  
  if (fd == NULL) {
    printf("Could not open %s, exiting.\n", file_name);
    exit(1);
 }

  /* obtain file size */
  fseek(fd, 0, SEEK_END);
  size = ftell(fd);
  rewind(fd);

  uint16_t *real_addr = get_addr_ptr(virt_addr);

  result = fread(real_addr, 1, size, fd);

  sprintf(str, "Placed %d bytes into flash\n\n", result);
  printf("%s", str);
  print_console(emu, str);

  fclose(fd);
}

uint16_t *get_stack_ptr(Emulator *emu) {
  Cpu *cpu = emu->cpu;
  
  return (uint16_t *) ( ((void *)MEMSPACE + cpu->sp) );
}

/**
 * @brief Get the host's pointer to the virtual address of the guest
 * @param virt_addr The virtual address of the guest to translate to a useable
 * one in context of the host
 * @return Pointer to the host's location of the guest's memory address
 */
uint16_t *get_addr_ptr(uint16_t virt_addr) {
  return (uint16_t *) ( (void *)MEMSPACE + virt_addr );
}

/**
 * @brief Get a pointer to the register specified by the numeric register value
 * @param cpu A pointer to the CPU structure
 * @param reg The numeric value of the register
 * @return Pointer to the register in question, NULL if register doesn't exist
 */
int16_t *get_reg_ptr(Emulator *emu, uint8_t reg)
{
  Cpu *cpu = emu->cpu;

  static int16_t r2 = 0;
  
  switch (reg) {  
    case 0x0: return (int16_t *) &cpu->pc;
    case 0x1: return (int16_t *) &cpu->sp;

    case 0x2:{
      r2 = sr_to_value(emu);
      return &r2;
    }
      
    case 0x3: return &cpu->cg2;
    case 0x4: return &cpu->r4;
    case 0x5: return &cpu->r5;
    case 0x6: return &cpu->r6;
    case 0x7: return &cpu->r7;
    case 0x8: return &cpu->r8;
    case 0x9: return &cpu->r9;
    case 0xA: return &cpu->r10;
    case 0xB: return &cpu->r11;
    case 0xC: return &cpu->r12;
    case 0xD: return &cpu->r13;
    case 0xE: return &cpu->r14;
    case 0xF: return &cpu->r15;
      
    default:{
      puts("Invalid Register Number");
      return 0;
    }
  }
}

/**
 * @brief Convert register ASCII name to it's respective numeric value
 * @param name The register's ASCII name
 * @return The numeric equivalent for the register on success, -1 if an 
 * invalid name was supplied
 */
int8_t reg_name_to_num(char *name)
{
  if ( !strncasecmp("%r0", name, sizeof "%r0") ||
       !strncasecmp("r0", name, sizeof "r0")   ||
       !strncasecmp("%pc", name, sizeof "%pc") ||
       !strncasecmp("pc", name, sizeof "pc") ) {
    
    return 0;
  }
  else if ( !strncasecmp("%r1", name, sizeof "%r1") ||
	    !strncasecmp("r1", name, sizeof "r1")   ||
	    !strncasecmp("%sp", name, sizeof "%sp")   ||
	    !strncasecmp("sp", name, sizeof "sp") ) {
    
    return 1;
  }
  else if ( !strncasecmp("%r2", name, sizeof "%r2") ||
	    !strncasecmp("r2", name, sizeof "r2")   ||
	    !strncasecmp("%sr", name, sizeof "%sr")   ||
	    !strncasecmp("sr", name, sizeof "sr") ) {
  
    return 2;
  }
  else if ( !strncasecmp("%r3", name, sizeof "%r3") ||
	    !strncasecmp("r3", name, sizeof "r3")   ||
	    !strncasecmp("%cg2", name, sizeof "%cg2") ||
	    !strncasecmp("cg2", name, sizeof "cg2") ) {            
    
    return 3;
  }
  else if ( !strncasecmp("%r4", name, sizeof "%r4") ||
	    !strncasecmp("r4", name, sizeof "r4") ) {
  
    return 4;
  }
  else if ( !strncasecmp("%r5", name, sizeof "%r5") ||
	    !strncasecmp("r5", name, sizeof "r5") ) {
  
    return 5;
  }
  else if ( !strncasecmp("%r6", name, sizeof "%r6") ||
	    !strncasecmp("r6", name, sizeof "r6") ) {
  
    return 6;
  }
  else if ( !strncasecmp("%r7", name, sizeof "%r7") ||
	    !strncasecmp("r7", name, sizeof "r7") ) {
  
    return 7;
  }
  else if ( !strncasecmp("%r8", name, sizeof "%r8") ||
	    !strncasecmp("r8", name, sizeof "r8") ) {
  
    return 8;
  }
  else if ( !strncasecmp("%r9", name, sizeof "%r9") ||
	    !strncasecmp("r9", name, sizeof "r9") ) {
  
    return 9;
  }
  else if ( !strncasecmp("%r10", name, sizeof "%r10") ||
	    !strncasecmp("r10", name, sizeof "r10") ) {
  
    return 10;
  }
  else if ( !strncasecmp("%r11", name, sizeof "%r11") ||
	    !strncasecmp("r11", name, sizeof "r11") ) {
  
    return 11;
  }
  else if ( !strncasecmp("%r12", name, sizeof "%r12") ||
	    !strncasecmp("r12", name, sizeof "r12") ) {
  
    return 12;
  }
  else if ( !strncasecmp("%r13", name, sizeof "%r13") ||
	    !strncasecmp("r13", name, sizeof "r13") ) {
  
    return 13;
  }
  else if ( !strncasecmp("%r14", name, sizeof "%r14") ||
	    !strncasecmp("r14", name, sizeof "r14") ) {
  
    return 14;
  }
  else if ( !strncasecmp("%r15", name, sizeof "%r15") ||
	    !strncasecmp("r15", name, sizeof "r15") ) {
  
    return 15;
  }
  
  return -1;
}

/**
 * @brief Convert register number into its ASCII name
 * @param number The register number (0, 1, 2, ...) associated with a 
 * register's name like (R0, R1, R2, ...)
 * @param name A pointer to an allocated character array to fill up with
 * the register's ASCII name
 */
void reg_num_to_name(uint8_t number, char *name)
{  
  switch (number) { 
    case 0x0:{
      strncpy(name, "PC\0", 3);
      return;
    }
    case 0x1:{
      strncpy(name, "SP\0", 3);
      return;
    }
    case 0x2:{
      strncpy(name, "SR\0", 3);
      return;
    }
    case 0x3:{
      strncpy(name, "R3\0", 3);
      return;
    }
    case 0x4:{
      strncpy(name, "R4\0", 3);
      return;
    }
    case 0x5:{
      strncpy(name, "R5\0", 3);
      return;
    }
    case 0x6:{
      strncpy(name, "R6\0", 3);
      return;
    }
    case 0x7:{
      strncpy(name, "R7\0", 3);
      return;
    }
    case 0x8:{
      strncpy(name, "R8\0", 3);
      return;
    }
    case 0x9:{
      strncpy(name, "R9\0", 3);
      return;
    }
    case 0xA:{
      strncpy(name, "R10\0", 4);
      return;
    }
    case 0xB:{
      strncpy(name, "R11\0", 4);
      return;
    }
    case 0xC:{
      strncpy(name, "R12\0", 4);
      return;
    }
    case 0xD:{
      strncpy(name, "R13\0", 4);
      return;
    }
    case 0xE:{
      strncpy(name, "R14\0", 4);
      return;
    }
    case 0xF:{
      strncpy(name, "R15\0", 4);
      return;
    }
    default:{
      strncpy(name, "???\0", 4);
      return;
    }
  }
}

/**
 * @brief This function displays the help menu to the user if he (or - but in
 * practice all too seldom - she) enters incorrect parameters or prompts the 
 * help menu with "help" or "h"
 */
void display_help(Emulator *emu)
{
  Debugger *deb = emu->debugger;
  char *help_str = (char *) malloc(3000);

  sprintf(help_str,
	  "**************************************************\n"\
	  "*\t\tMSP430-Emulator\n*\n*\tUsage: ./msp430 BINARY_FIRMWARE\n*\n"\
	  "* run\t\t\t[Run Program Until Breakpoint is Hit]\n"\
	  "* step [N]\t\t[Step Into Instruction]\n"\
	  "* dump [HEX_ADDR|Rn]\t[Dump Memory direct or at register value]\n"\
	  "* set [HEX_ADDR|Rn]\t[Set Memory or Register Location]\n"\
	  "* dis [N][HEX_ADDR]\t[Disassemble Instructions]\n"\
	  "* break ADDR\t\t[Set a Breakpoint]\n"\
	  "* bps\t\t\t[Display Breakpoints]\n"\
	  "* regs\t\t\t[Display Registers]\n"\
	  "* CTRL+C\t\t[Pause Execution]\n"\
	  "* reset\t\t\t[Reset Machine]\n"\
	  "* quit\t\t\t[Exit program]\n"\
	  "**************************************************\n");

  if (deb->web_interface) {
    print_console(emu, help_str);
  }
  else {
    printf("%s", help_str);
  }

  free(help_str);
}
