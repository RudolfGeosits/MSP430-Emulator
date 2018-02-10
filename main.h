/*
  MSP430 Emulator
  Copyright (C) 2016 Rudolf Geosits (rgeosits@live.esu.edu)  
                                                                      
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

#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
#include "libs/libwebsockets-2.0.2/libwebsockets.h"
#include <readline/readline.h>
#include <readline/history.h>

typedef struct Emulator Emulator;

typedef struct Cpu Cpu;
typedef struct Port_1 Port_1;
typedef struct Usci Usci;
typedef struct Bcm Bcm;
typedef struct Timer_a Timer_a;
typedef struct Status_reg Status_reg;

typedef struct Debugger Debugger;
typedef struct Server Server;
typedef struct Packet Packet;

#include "devices/peripherals/bcm.h"
#include "devices/peripherals/timer_a.h"
#include "devices/peripherals/port1.h"
#include "devices/peripherals/usci.h"
#include "devices/cpu/registers.h"
#include "devices/utilities.h"
#include "devices/memory/memspace.h"
#include "debugger/websockets/emu_server.h"
#include "devices/cpu/decoder.h"
#include "debugger/debugger.h"
#include "debugger/register_display.h"
#include "debugger/disassembler.h"

struct Emulator {
  Cpu *cpu;
  Debugger *debugger;
};

#endif
