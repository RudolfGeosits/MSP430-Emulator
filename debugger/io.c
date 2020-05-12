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

#define BIT_COUNT 8

static void digitalIoToString(char* const output, const uint8_t mask, const uint8_t x)
{
    for (size_t i = 0; i < BIT_COUNT; i++)
    {
        const uint8_t bit = 1 << i;
        const bool valid = (mask & bit) != 0;
        const bool one = (x & bit) != 0;
        output[i] = valid ? (one ? '1' : '0') : 'X';
    }
    output[BIT_COUNT + 1] = 0;
}

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

void put_port1(Emulator* const emu, const uint8_t mask, const uint8_t x)
{

    char str[BIT_COUNT + 1];
    digitalIoToString(str, mask, x);
    if (emu->port1_output_pipe == NULL)
    {
        print_console(emu, "PORT1:");
        print_console(emu, str);
        print_console(emu, "\n");
    }
    else
    {
        str[BIT_COUNT] = '\n';
        const ssize_t count = write(emu->port1_output_pipe_fd, str, BIT_COUNT + 1);
        fflush(emu->port1_output_pipe);
        if (count != BIT_COUNT + 1)
            print_console(emu, "Cannot write to port 1 output\n");
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
        const ssize_t count = write(emu->usci_output_pipe_fd, &x, sizeof(uint8_t));
        fflush(emu->usci_output_pipe);
        if (count != 1)
            print_console(emu, "Cannot write to serial output\n");
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