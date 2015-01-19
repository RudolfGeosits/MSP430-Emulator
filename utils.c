/*                                                                
MSP430 Emulator                                                   
Copyright (C) 2014, 2015 Rudolf Geosits (rgeosits@live.esu.edu)                
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

void load_program(char *program_name, uint8_t *MEM)
{
  uint32_t size, result;
  printf("Loading Program: ( %s )\n", program_name);

  FILE *fd = fopen(program_name, "rb+");
  
  if (fd == NULL) {
    printf("Could not open %s, exiting.\n", program_name);
    exit(1);
  }

  /* obtain file size */
  fseek(fd, 0, SEEK_END);
  size = ftell(fd);
  rewind(fd);

  result = fread(MEM, 1, size, fd);
  printf("Placed %d bytes into flash\n\n\n", result);

  fclose(fd);
}

uint16_t *get_stack_ptr() {
  return (uint16_t *) ( ((void *)MEMSPACE + SP) );
}

uint16_t *get_addr_ptr(uint16_t addr) {
  return (uint16_t *) ( (void *)MEMSPACE + addr );
}

//###########+++ Register number to name +++#########
int16_t *get_reg_ptr(uint8_t reg)
{  
  static int16_t r2 = 0;

  switch (reg) {  
    case 0x0: return &PC;
    case 0x1: return &SP;

    case 0x2:{
      r2 = sr_to_value();
      return &r2;
    }

    case 0x3: return &CG2;
    case 0x4: return &r4;
    case 0x5: return &r5;
    case 0x6: return &r6;
    case 0x7: return &r7;
    case 0x8: return &r8;
    case 0x9: return &r9;
    case 0xA: return &r10;
    case 0xB: return &r11;
    case 0xC: return &r12;
    case 0xD: return &r13;
    case 0xE: return &r14;
    case 0xF: return &r15;
      
    default:{
      printf("Invalid Register Number");
      return 0;
    }
  }
}

/* Convert register name to respective opcode number
** Ex:
**    uint8_t n = reg_name_to_num("%r1");
** Returns number on success, -1 on invalid name
*/
uint8_t reg_name_to_num(char *name)
{
  if ( strncmp("%R0", name, sizeof "%R0") == 0 ||
       strncmp("%r0", name, sizeof "%r0") == 0 ||
       strncmp("R0", name, sizeof "R0") == 0   ||
       strncmp("r0", name, sizeof "r0") == 0   ||
       strncmp("PC", name, sizeof "PC") == 0   || 
       strncmp("pc", name, sizeof "pc") == 0 ) {
  
    return 0;
  }
  else if ( strncmp("%R1", name, sizeof "%R1") == 0 ||
	    strncmp("%r1", name, sizeof "%r1") == 0 ||
	    strncmp("R1", name, sizeof "R1") == 0   ||
	    strncmp("r1", name, sizeof "r1") == 0   ||
	    strncmp("SP", name, sizeof "SP") == 0   ||
	    strncmp("sp", name, sizeof "sp") == 0 ) {
  
    return 1;
  }
  else if ( strncmp("%R2", name, sizeof "%R2") == 0 ||
	    strncmp("%r2", name, sizeof "%r2") == 0 ||
	    strncmp("R2", name, sizeof "R2") == 0   ||
	    strncmp("r2", name, sizeof "r2") == 0   ||
	    strncmp("SR", name, sizeof "SR") == 0   ||
	    strncmp("sr", name, sizeof "sr") == 0 ) {
  
    return 2;
  }
  else if ( strncmp("%R3", name, sizeof "%R3") == 0 ||
	    strncmp("%r3", name, sizeof "%r3") == 0 ||
	    strncmp("R3", name, sizeof "R3") == 0   ||
	    strncmp("r3", name, sizeof "r3") == 0 ) {
  
    return 3;
  }
  else if ( strncmp("%R4", name, sizeof "%R4") == 0 ||
	    strncmp("%r4", name, sizeof "%r4") == 0 ||
	    strncmp("R4", name, sizeof "R4") == 0   ||
	    strncmp("r4", name, sizeof "r4") == 0 ) {
  
    return 4;
  }
  else if ( strncmp("%R5", name, sizeof "%R5") == 0 ||
	    strncmp("%r5", name, sizeof "%r5") == 0 ||
	    strncmp("R5", name, sizeof "R5") == 0   ||
	    strncmp("r5", name, sizeof "r5") == 0 ) {
  
    return 5;
  }
  else if ( strncmp("%R6", name, sizeof "%R6") == 0 ||
	    strncmp("%r6", name, sizeof "%r6") == 0 ||
	    strncmp("R6", name, sizeof "R6") == 0   ||
	    strncmp("r6", name, sizeof "r6") == 0 ) {
  
    return 6;
  }
  else if ( strncmp("%R7", name, sizeof "%R7") == 0 ||
	    strncmp("%r7", name, sizeof "%r7") == 0 ||
	    strncmp("R7", name, sizeof "R7") == 0   ||
	    strncmp("r7", name, sizeof "r7") == 0 ) {
  
    return 7;
  }
  else if ( strncmp("%R8", name, sizeof "%R8") == 0 ||
	    strncmp("%r8", name, sizeof "%r8") == 0 ||
	    strncmp("R8", name, sizeof "R8") == 0   ||
	    strncmp("r8", name, sizeof "r8") == 0 ) {
  
    return 8;
  }
  else if ( strncmp("%R9", name, sizeof "%R9") == 0 ||
	    strncmp("%r9", name, sizeof "%r9") == 0 ||
	    strncmp("R9", name, sizeof "R9") == 0   ||
	    strncmp("r9", name, sizeof "r9") == 0 ) {
  
    return 9;
  }
  else if ( strncmp("%R10", name, sizeof "%R10") == 0 ||
	    strncmp("%r10", name, sizeof "%r10") == 0 ||
	    strncmp("R10", name, sizeof "R10") == 0   ||
	    strncmp("r10", name, sizeof "r10") == 0 ) {
  
    return 10;
  }
  else if ( strncmp("%R11", name, sizeof "%R11") == 0 ||
	    strncmp("%r11", name, sizeof "%r11") == 0 ||
	    strncmp("R11", name, sizeof "R11") == 0   ||
	    strncmp("r11", name, sizeof "r11") == 0 ) {
  
    return 11;
  }
  else if ( strncmp("%R12", name, sizeof "%R12") == 0 ||
	    strncmp("%r12", name, sizeof "%r12") == 0 ||
	    strncmp("R12", name, sizeof "R12") == 0   ||
	    strncmp("r12", name, sizeof "r12") == 0 ) {
  
    return 12;
  }
  else if ( strncmp("%R13", name, sizeof "%R13") == 0 ||
	    strncmp("%r13", name, sizeof "%r13") == 0 ||
	    strncmp("R13", name, sizeof "R13") == 0   ||
	    strncmp("r13", name, sizeof "r13") == 0 ) {
  
    return 13;
  }
  else if ( strncmp("%R14", name, sizeof "%R14") == 0 ||
	    strncmp("%r14", name, sizeof "%r14") == 0 ||
	    strncmp("R14", name, sizeof "R14") == 0   ||
	    strncmp("r14", name, sizeof "r14") == 0 ) {
  
    return 14;
  }
  else if ( strncmp("%R15", name, sizeof "%R15") == 0 ||
	    strncmp("%r15", name, sizeof "%r15") == 0 ||
	    strncmp("R15", name, sizeof "R15") == 0   ||
	    strncmp("r15", name, sizeof "r15") == 0 ) {
  
    return 15;
  }

  else {
    return -1;
  }

}

//###########+++ Register number to name +++#########
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

//##########+++ Display the menu for this program +++##########
void display_help()
{
  printf("****************************************"\
         "****************************************\n");

  printf("*\t\tMSP430-Emulator\n*\n*\tUsage: ./msp430 [BINARY FILE]\n*\n");

  printf("****************************************"\
         "****************************************\n");
}
