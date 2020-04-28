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

#include "io.h"
#include "websockets/packet_queue.h"

void print_serial (Emulator *emu, char *buf)
{
    switch (emu->mode)
    {
        case Emulator_Mode_Web:
            packet_enqueue(emu, buf, strlen(buf) + 1, SERIAL_PACKET_OPCODE);
            break;
        case Emulator_Mode_Cli:
            // This is just for fallback or compatibility, if another UART like peripheral
            // was implemented independently
            printf("Serial %s\n", buf);
            break;
    }
}

void put_serial(Emulator* const emu, const uint8_t x)
{
    if (emu->usci_output_pipe == NULL)
    {
        char str[2];
        str[0] = (char)x;
        str[1] = 0;
        print_serial(emu, str);
    }
    else
    {
        write(emu->usci_output_pipe_fd, &x, sizeof(uint8_t));
    }
}

bool get_serial(Emulator* const emu, uint8_t* const x)
{
    if (emu->usci_input_pipe == NULL)
        return false;
    const ssize_t count = read(emu->usci_input_pipe_fd, x, sizeof(uint8_t));
    return count > 0;
}

void print_console (Emulator *emu, const char *buf)
{
    switch (emu->mode)
    {
    case Emulator_Mode_Web:
        packet_enqueue(emu, (void*)buf, strlen(buf) + 1, CONSOLE_PACKET_OPCODE);
        break;
    case Emulator_Mode_Cli:
        printf("%s", buf);
        break;
    }
}