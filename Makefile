CFLAGS=-g 

all: MSP430 SERVER

MSP430 : main.o emu_server.o utilities.o registers.o memspace.o debugger.o disassembler.o \
	register_display.o decoder.o flag_handler.o formatI.o formatII.o formatIII.o usci.o port1.o bcm.o \
	timer_a.o packet_queue.o io.o

	g++ ${CFLAGS} -o MSP430 $^ -lreadline -lwebsockets -lpthread -lrt -lssl -lcrypto;

main.o : main.cpp
	g++ ${CFLAGS} -c main.cpp

utilities.o : devices/utilities.c
	g++ ${CFLAGS} -c devices/utilities.c

registers.o : devices/cpu/registers.c
	g++ ${CFLAGS} -c devices/cpu/registers.c

memspace.o : devices/memory/memspace.c
	g++ ${CFLAGS} -c devices/memory/memspace.c

debugger.o : debugger/debugger.c
	g++ ${CFLAGS} -c debugger/debugger.c

disassembler.o : debugger/disassembler.c
	g++ ${CFLAGS} -c debugger/disassembler.c

register_display.o : debugger/register_display.c
	g++ ${CFLAGS} -c debugger/register_display.c

decoder.o : devices/cpu/decoder.c
	g++ ${CFLAGS} -c devices/cpu/decoder.c

flag_handler.o : devices/cpu/flag_handler.c
	g++ ${CFLAGS} -c devices/cpu/flag_handler.c

formatI.o : devices/cpu/formatI.c
	g++ ${CFLAGS} -c devices/cpu/formatI.c

formatII.o : devices/cpu/formatII.c
	g++ ${CFLAGS} -c devices/cpu/formatII.c

formatIII.o : devices/cpu/formatIII.c
	g++ ${CFLAGS} -c devices/cpu/formatIII.c

bcm.o : devices/peripherals/bcm.c
	g++ ${CFLAGS} -c devices/peripherals/bcm.c

timer_a.o : devices/peripherals/timer_a.c
	g++ ${CFLAGS} -c devices/peripherals/timer_a.c

usci.o : devices/peripherals/usci.c
	g++ ${CFLAGS} -c devices/peripherals/usci.c

port1.o : devices/peripherals/port1.c
	g++ ${CFLAGS} -c devices/peripherals/port1.c

io.o: debugger/io.c
	g++ ${CFLAGS} -c $<

emu_server.o : debugger/websockets/emu_server.cpp
	g++ ${CFLAGS} -c debugger/websockets/emu_server.cpp

packet_queue.o : debugger/websockets/packet_queue.c
	g++ ${CFLAGS} -c debugger/websockets/packet_queue.c

# Server Program

SERVER : server.o
	g++ ${CFLAGS} -o server server.o -lrt -lpthread -lwebsockets -lssl -lcrypto;

server.o : debugger/server/server.c
	g++ ${CFLAGS} -c debugger/server/server.c


clean :
	rm server.o main.o utilities.o emu_server.o registers.o \
	memspace.o debugger.o disassembler.o \
	register_display.o decoder.o flag_handler.o formatI.o \
	formatII.o formatIII.o \
	usci.o port1.o packet_queue.o bcm.o timer_a.o \
	*.bin *.tmp *.elf \
	MSP430 server;
