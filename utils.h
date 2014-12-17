void display_help();
void reg_num_to_name(uint8_t source_reg, char *reg_name);
int16_t *get_reg_ptr(uint8_t reg);
void load_program(char *program_name, uint8_t *MEM);

#include "utils.c"
