CC=g++
CFLAGS=-g
SERVER=server
EMULATOR=msp430-emu

.PHONY: all test

all: ${EMULATOR} ${SERVER}

# Main emulator program

${EMULATOR} : main.o emu_server.o utilities.o registers.o memspace.o debugger.o disassembler.o \
	register_display.o decoder.o flag_handler.o formatI.o formatII.o formatIII.o usci.o port1.o bcm.o \
	timer_a.o packet_queue.o io.o
	${CC} ${CFLAGS} -o $@ $^ -lreadline -lwebsockets -lpthread -lrt -lssl -lcrypto;

main.o : main.cpp
	${CC} ${CFLAGS} -c $<

utilities.o : devices/utilities.c
	${CC} ${CFLAGS} -c $<

registers.o : devices/cpu/registers.c
	${CC} ${CFLAGS} -c $<

memspace.o : devices/memory/memspace.c
	${CC} ${CFLAGS} -c $<

debugger.o : debugger/debugger.c
	${CC} ${CFLAGS} -c $<

disassembler.o : debugger/disassembler.c
	${CC} ${CFLAGS} -c $<

register_display.o : debugger/register_display.c
	${CC} ${CFLAGS} -c $<

decoder.o : devices/cpu/decoder.c
	${CC} ${CFLAGS} -c $<

flag_handler.o : devices/cpu/flag_handler.c
	${CC} ${CFLAGS} -c $<

formatI.o : devices/cpu/formatI.c
	${CC} ${CFLAGS} -c $<

formatII.o : devices/cpu/formatII.c
	${CC} ${CFLAGS} -c $<

formatIII.o : devices/cpu/formatIII.c
	${CC} ${CFLAGS} -c $<

bcm.o : devices/peripherals/bcm.c
	${CC} ${CFLAGS} -c $<

timer_a.o : devices/peripherals/timer_a.c
	${CC} ${CFLAGS} -c $<

usci.o : devices/peripherals/usci.c
	${CC} ${CFLAGS} -c $<

port1.o : devices/peripherals/port1.c
	${CC} ${CFLAGS} -c $<

io.o: debugger/io.c
	${CC} ${CFLAGS} -c $<

emu_server.o : debugger/websockets/emu_server.cpp
	${CC} ${CFLAGS} -c $<

packet_queue.o : debugger/websockets/packet_queue.c
	${CC} ${CFLAGS} -c debugger/websockets/packet_queue.c

# Server Program

${SERVER} : server.o
	${CC} ${CFLAGS} -o $@ $^ -lrt -lpthread -lwebsockets -lssl -lcrypto;

server.o : debugger/server/server.c
	${CC} ${CFLAGS} -c $<


clean :
	rm -f server.o main.o utilities.o emu_server.o registers.o \
		memspace.o debugger.o disassembler.o \
		register_display.o decoder.o flag_handler.o formatI.o \
		formatII.o formatIII.o io.o \
		usci.o port1.o packet_queue.o bcm.o timer_a.o \
		${EMULATOR} ${SERVER}
	make -C test clean

test: ${EMULATOR} test/Makefile
	make -C test test
