
/* Initialize Address Space Locations
** 
** Allocate and set MSP430 Memory space
** Some of these locations vary by model
*/
void initialize_msp_memspace()
{
  /* 64 KB Address Space */
  MEMSPACE = (uint8_t *) malloc(0xFFFF);
  
  IVT = MEMSPACE +   0xFFE0;   /* 0xFFE0 - 0xFFFF */
  ROM = MEMSPACE +   0x0400;   /* 0x400 - 0xFFDF {Varies with device} */
  RAM = MEMSPACE +   0x0200;   /* 0x200 - 0x3FF {Varies with device} */
  PER16 = MEMSPACE + 0x0100;   /* 0x0100 - 0x01FF */
  PER8 = MEMSPACE +  0x0010;   /* 0x0010 - 0x00FF */
  SFRS = MEMSPACE +  0x0;      /* 0x0 - 0x0F */

  LOAD_POS = MEMSPACE + 0xC000;
}


/* 
** Free MSP430 virtual memory
*/
void uninitialize_msp_memspace()
{
  free(MEMSPACE);
}
