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

//##########+++ Main Command Loop +++##########
void command_loop()
{
  char command[32];                  

  while (1) {
    memset(command, 0, sizeof(command));
    scanf("%s", command);

    //# st X, step X instructions forward, defaults to 1
    if ( strncmp("st", command, sizeof("st")) == 0 ) {
      break;
    }                                 
    else if ( strncmp("r", command, sizeof("r")) == 0 ) {
      display_registers();
      continue;
    }

    //# db ADDRESS, dump bytes starting from ADDRESS 
    else if ( strncmp("db", command, sizeof("db")) == 0 ) {      
      uint32_t start_addr;     
      
      scanf("%X", &start_addr);               
      dump_memory(MEMSPACE, 0xFFFF, start_addr, "b");
      continue;
    }                                               

    //# dw ADDRESS, dump words starting from ADDRESS
    else if ( strncmp("dw", command, sizeof("dw")) == 0 ) {               
      uint32_t start_addr;             

      scanf("%X", &start_addr);                         
      dump_memory(MEMSPACE, 0xFFFF, start_addr, "w");
    }                                                                    

    //# setr REG VAL, set a register to some value
    else if ( strncmp("setr", command, sizeof("setr")) == 0 ) {
      int value = 0;
      char reg_name[10];
      
      scanf("%s %X", reg_name, &value);

      uint16_t *reg = get_reg_ptr( reg_name_to_num(reg_name) );
      *reg = value;
    }
    //# setr MEMLOC VAL, set a memory location to some value
    else if ( strncmp("setr", command, sizeof("setr")) == 0 ) {
      int value = 0;
      char memory_loc[10];
      
      scanf("%s %X", memory_loc, &value);
    }
    //# End the command loop, next instruction
    else {
      break;
    }
  }
}

//##########+++ Dump Memory Function +++##########
void display_registers()
{
  uint16_t r2 = sr_to_value();

  printf("\n%%R0:%04X %%r1:%04X %%r2:%04X %%r3:%04X %%r4:%04X\n" \
         "%%r5:%04X %%r6:%04X %%r7:%04X %%r8:%04X %%r9:%04X\n" \
         "%%r10:%04X   %%r11:%04X  %%r12:%04X   %%r13:%04X\n" \
         "%%r14:%04X   %%r15:%04X  V:%d  N:%d  Z:%d  C:%d\n",
         
         (uint16_t)PC, (uint16_t)SP, (uint16_t)r2, (uint16_t)CG2, (uint16_t)r4,
	 (uint16_t)r5, (uint16_t)r6, (uint16_t)r7, (uint16_t)r8, (uint16_t)r9,
	 (uint16_t)r10, (uint16_t)r11, (uint16_t)r12, (uint16_t)r13, 
	 (uint16_t)r14, (uint16_t)r15, 
	
	 SR.overflow, SR.negative, SR.zero, SR.carry);
}

//##########+++ Dump Memory Function +++##########
void dump_memory(uint8_t *MEM, int size, uint32_t start_addr, char *chunks)
{
  uint32_t i, msp_addr = start_addr;
  MEM += start_addr;

  puts("");

  for (i = 0; i < 32; i += 8) {
    printf("0x%04X:\t", msp_addr);

    if ( strncmp(chunks, "b", sizeof("b")) == 0 ) {
      printf("0x%02X  0x%02X  0x%02X  0x%02X  " \
	     "0x%02X  0x%02X  0x%02X  0x%02X\n",
             *(MEM+0),*(MEM+1),*(MEM+2),*(MEM+3),
	     *(MEM+4),*(MEM+5),*(MEM+6),*(MEM+7));
    }
    else if ( strncmp(chunks, "w", sizeof("w")) == 0 ) {
      printf("0x%02X%02X  0x%02X%02X  0x%02X%02X  0x%02X%02X\n",
             *(MEM+0),*(MEM+1),*(MEM+2),*(MEM+3),*(MEM+4),
	     *(MEM+5),*(MEM+6),*(MEM+7));
    }
    else if ( strncmp(chunks, "d", sizeof("d")) == 0 ) {
      printf("0x%02X%02X%02X%02X  0x%02X%02X%02X%02X\n",
             *(MEM+0),*(MEM+1),*(MEM+2),*(MEM+3),*(MEM+4),
	     *(MEM+5),*(MEM+6),*(MEM+7));
    }
    
    MEM += 8; //# Increase character by 4
    msp_addr += 8; //# Increase msp_addr by 4
  }

  puts("");
}
