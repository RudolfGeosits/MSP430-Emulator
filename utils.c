#include "utils.h"

void reg_num_to_name(unsigned int source_reg, char* reg_name){
  
  switch(source_reg){
  
  case 0x0:{
    strncpy(reg_name, "R0\0", 3);
    return;
  }
  case 0x1:{
    strncpy(reg_name, "R1\0", 3);
    return;
  }
  case 0x2:{
    strncpy(reg_name, "R2\0", 3);
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
