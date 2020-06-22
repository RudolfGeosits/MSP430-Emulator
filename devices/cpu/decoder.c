/*
  MSP430 Emulator
  Copyright (C) 2020 Rudolf Geosits (rgeosits@live.esu.edu)

  "MSP430 Emulator" is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  "MSP430 Emulator" is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "decoder.h"
#include "../../debugger/io.h"

// ##########+++ CPU Fetch Cycle  +++##########
uint16_t fetch(Emulator *emu, bool report)
{
    Cpu *cpu = emu->cpu;
    uint16_t word, *p;
    
    p = (get_addr_ptr(cpu->pc));
    word = *p;
    if (emu->do_trace && report)
    {
        char buffer[128];    
        sprintf(buffer, "Fetching %x - %x\n", cpu->pc, word);
        print_console(emu, buffer);
    }

    cpu->pc += 2;

    return word;
}

// ##########+++ CPU Decode Cycle +++##########
void decode(Emulator *emu, uint16_t instruction, bool disassemble)
{
    Cpu *cpu = emu->cpu;
    Debugger *debugger = emu->debugger;

    int done = 0;
    uint8_t FormatId;
    memset(debugger->mnemonic, 0, sizeof debugger->mnemonic);

    FormatId = (uint8_t)(instruction >> 12);

    if (FormatId == 0x1)
    {
        // format II (single operand) instruction
        decode_formatII(emu, instruction, disassemble);
    }
    else if (FormatId >= 0x2 && FormatId <= 3)
    {
        // format III (jump) instruction
        decode_formatIII(emu, instruction, disassemble);
    }
    else if (FormatId >= 0x4)
    {
        // format I (two operand) instruction
        decode_formatI(emu, instruction, disassemble);
    }
    else
    {
        char inv[100] = {0};
        debugger->error = ERROR_ILLEGAL_INSTRUCTION;
        sprintf(inv, "%04X\t[INVALID INSTRUCTION]\n", instruction);
        print_console(emu, inv);

        //cpu->pc -= 2;
        cpu->running = false;
        debugger->debug_mode = true;
    }
    update_cpu_stats(emu);
}

// Constant Generator
int16_t run_constant_generator(uint8_t source, uint8_t as_flag)
{
    int16_t generated_constant = 0;

    switch (source)
    {
        case 2:
        {     /* Register R2/SR/CG1 */
            switch (as_flag)
            {
                case 0b10:
                {     /* +4, bit processing */
	                generated_constant = 4;
	                break;
                }
                case 0b11:
                {     /* +8, bit processing */
	                generated_constant = 8;
	                break;
	            }
                default:
                {
	                printf("Invalid as_flag for CG1\n");
                }
            }

            break;
        }

        // Register R3/CG2
        case 3:
        {

            switch (as_flag)
            {
                case 0b00:
                {     /* 0, word processing */
	                generated_constant = 0;
	                break;
                }
                case 0b01:
                {     /* +1 */
	                generated_constant = 1;
	                break;
                }
                case 0b10:
                {     /* +2, bit processing */
	                generated_constant = 2;
	                break;
                }
                case 0b11:
                {     /* -1, word processing */
	                generated_constant = -1;
	                break;
                }
                default:
                {
	                printf("Invalid as_flag for CG2\n");
                }
            }

            break;
        }

        default:
        {
            printf("Invalid source register for constant generation.\n");
        }
    }

    return generated_constant;
}
