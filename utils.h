/*
  This file is part of MSP430 Emulator
  
  Foobar is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  Foobar is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

void display_help();
void reg_num_to_name(uint8_t source_reg, char *reg_name);
int16_t *get_reg_ptr(uint8_t reg);
void load_program(char *program_name, uint8_t *MEM);
uint8_t reg_name_to_num(char *name);
uint16_t *get_stack_ptr();

#include "utils.c"
