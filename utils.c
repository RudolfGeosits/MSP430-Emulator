#include "utils.h"

//##########+++ Load binary into memory +++##########       
void load_program(char* program_name, unsigned char* MEM){
  int size, result;
  printf("Executing Program name: %s\n", program_name);
  FILE* fd = fopen(program_name, "rb+");

  // obtain file size
  fseek(fd, 0, SEEK_END);
  size = ftell(fd);
  rewind(fd);

  result = fread(MEM, 1, size, fd);
  printf("Placed %d bytes into flash\n\n\n", result);

  fclose(fd);
}

//###########+++ Register number to name +++#########
short* get_reg_ptr(unsigned char reg){
  
  static short r2 = 0;

  switch( reg ){
 
  case 0x0:{
    return &PC;
  }
  case 0x1:{
    return &SP;
  }
  case 0x2:{
    r2 = sr_to_value();
    return &r2;
  }
  case 0x3:{
    return 0;
  }
  case 0x4:{
    return &r4;
  }
  case 0x5:{
    return &r5;
  }
  case 0x6:{
    return &r6;
  }
  case 0x7:{
    return &r7;
  }
  case 0x8:{
    return &r8;
  }
  case 0x9:{
    return &r9;
  }
  case 0xA:{
    return &r10;
  }
  case 0xB:{
    return &r11;
  }
  case 0xC:{
    return &r12;
  }
  case 0xD:{
    return &r13;
  }
  case 0xE:{
    return &r14;
  }
  case 0xF:{
    return &r15;
  }
  default:{
    return 0;
  } 

 }

}

//###########+++ Register number to name +++#########
void reg_num_to_name(unsigned int source_reg, char* reg_name){
  
  switch(source_reg){
  
  case 0x0:{
    strncpy(reg_name, "PC\0", 3);
    return;
  }
  case 0x1:{
    strncpy(reg_name, "SP\0", 3);
    return;
  }
  case 0x2:{
    strncpy(reg_name, "SR\0", 3);
    return;
  }
  case 0x3:{
    strncpy(reg_name, "R3\0", 3);
    return;
  }
  case 0x4:{
    strncpy(reg_name, "R4\0", 3);
    return;
  }
  case 0x5:{
    strncpy(reg_name, "R5\0", 3);
    return;
  }
  case 0x6:{
    strncpy(reg_name, "R6\0", 3);
    return;
  }
  case 0x7:{
    strncpy(reg_name, "R7\0", 3);
    return;
  }
  case 0x8:{
    strncpy(reg_name, "R8\0", 3);
    return;
  }
  case 0x9:{
    strncpy(reg_name, "R9\0", 3);
    return;
  }
  case 0xA:{
    strncpy(reg_name, "R10\0", 4);
    return;
  }
  case 0xB:{
    strncpy(reg_name, "R11\0", 4);
    return;
  }
  case 0xC:{
    strncpy(reg_name, "R12\0", 4);
    return;
  }
  case 0xD:{
    strncpy(reg_name, "R13\0", 4);
    return;
  }
  case 0xE:{
    strncpy(reg_name, "R14\0", 4);
    return;
  }
  case 0xF:{
    strncpy(reg_name, "R15\0", 4);
    return;
  }
  default:{
    strncpy(reg_name, "???\0", 4);
    return;
  }

  } //# End of switch
}

//##########+++ Display the menu for this program +++##########
void display_help(){

  printf("****************************************"\
         "****************************************\n");

  printf("*\t\tMSP430-Emulator\n*\n*\tUsage: ./msp430 [BINARY FILE]\n*\n");

  printf("****************************************"\
         "****************************************\n");

}
