/*   Display all 16 registers                                                  
 *   - Toggle between Common mode and Specific mode                         
 *     (Common: Display all as R0 - R15)                                   
 *     (Specific: Display Register usages like PC, SR, etc.)               
 */

enum {COMMON_MODE, SPECIFIC_MODE};
uint8_t register_display_mode = SPECIFIC_MODE;

void display_registers()
{
  typedef enum {UNDEF, LINUX, WINDOWS} System_t;
  System_t this_system;
  uint16_t r2 = sr_to_value();

#ifdef __linux
  this_system = LINUX;
#else
  this_system = UNDEF;
#endif
  
  char *r0_name, *r1_name, *r2_name, *r3_name,
    *r4_name, *r5_name, *r6_name, *r7_name,
    *r8_name, *r9_name, *r10_name, *r11_name,
    *r12_name, *r13_name, *r14_name, *r15_name;
  
  char *v_flag, *n_flag, *z_flag, *c_flag;
  char *red, *green, *cyan;

  if (this_system == LINUX) {
    r0_name = "%R0";
    r1_name = "%R1";
    r2_name = "%R2";
    r3_name = "%R3";

    red = "\x1b[31;1m";
    green = "\x1b[32;1m";
    cyan = "\x1b[36;1m";

    r4_name = "%R4";
    r5_name = "%R5";
    r6_name = "%R6";
    r7_name = "%R7";
    r8_name = "%R8";
    r9_name = "%R9";
    r10_name = "%R10";
    r11_name = "%R11";
    r12_name = "%R12";
    r13_name = "%R13";
    r14_name = "%R14";
    r15_name = "%R15";

    if (register_display_mode == SPECIFIC_MODE) {
      r0_name = "PC";
      r1_name = "SP";
      r2_name = "SR";
      r3_name = "CG2";
    }

    v_flag = "V\x1b[0m";
    n_flag = "\x1b[36;1mN\x1b[0m";
    z_flag = "\x1b[36;1mZ\x1b[0m";
    c_flag = "\x1b[36;1mC\x1b[0m";
  }
  else {
    r0_name = "%R0";
    r1_name = "%R1";
    r2_name = "%R2";
    r3_name = "%R3";

    green = "";
    red = "";
    cyan = "";
    r4_name = "%R4";
    r5_name = "%R5";
    r6_name = "%R6";
    r7_name = "%R7";
    r8_name = "%R8";
    r9_name = "%R9";
    r10_name = "%R10";
    r11_name = "%R11";
    r12_name = "%R12";
    r13_name = "%R13";
    r14_name = "%R14";
    r15_name = "%R15";
    
    if (register_display_mode == SPECIFIC_MODE) {
      r0_name = "PC";
      r1_name = "SP";
      r2_name = "SR";
      r3_name = "CG2";
    }

    v_flag = "V";
    n_flag = "N";
    z_flag = "Z";
    c_flag = "C";
  }
  
  printf("\n%s%s:   %s%04X\t%s%s:   %s%04X\t%s%s:   %s%04X\t%s%s:  %s%04X\n"\
	 "%s%s:  %s%04X\t%s%s:  %s%04X\t%s%s:  %s%04X\t%s%s:  %s%04X\n"\
	 "%s%s:  %s%04X\t%s%s:  %s%04X\t%s%s: %s%04X\t%s%s: %s%04X\n"\
	 "%s%s: %s%04X\t%s%s: %s%04X\t%s%s: %s%04X\t%s%s: %s%04X\n"\
	 "%s%s:%d   %s%s:%d   %s%s:%d   %s%s:%d\n\n",
	 
	 red, r0_name, green, (uint16_t)PC, 
	 red, r1_name, green, (uint16_t)SP, 
	 red, r2_name, green, (uint16_t)r2, 
	 red, r3_name, green, (uint16_t)CG2, 
	 red, r4_name, green, (uint16_t)r4, 
	 red, r5_name, green, (uint16_t)r5, 
	 red, r6_name, green, (uint16_t)r6,
	 red, r7_name, green, (uint16_t)r7, 
	 red, r8_name, green, (uint16_t)r8, 
	 red, r9_name, green, (uint16_t)r9, 
	 red, r10_name, green, (uint16_t)r10,
	 red, r11_name, green, (uint16_t)r11, 
	 red, r12_name, green, (uint16_t)r12, 
	 red, r13_name, green, (uint16_t)r13, 
	 red, r14_name, green, (uint16_t)r14,
	 red, r15_name, green, (uint16_t)r15,

	 cyan, v_flag, SR.overflow, 
	 cyan, n_flag, SR.negative, 
	 cyan, z_flag, SR.zero, 
	 cyan, c_flag, SR.carry);
}
