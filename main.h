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

#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <stdbool.h>
#include <pthread.h>
#include <libwebsockets.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "devices/peripherals/port1.h"
#include "devices/peripherals/usci.h"
#include "devices/cpu/registers.h"
#include "devices/utilities.h"
#include "devices/memory/memspace.h"
#include "debugger/server/server.h"
#include "devices/cpu/decoder.h"

typedef struct {
  Cpu *cpu;
  Debugger *debugger;
} Emulator;

#endif
