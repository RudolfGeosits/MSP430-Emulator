CXX=g++
CXXFLAGS=-g
LDLIBS=-lreadline -lwebsockets -lpthread -lrt -lssl -lcrypto
SERVER=server
EMULATOR=msp430-emu

.PHONY: all test

all: ${EMULATOR} ${SERVER}

# Main emulator program

${EMULATOR} : main.o emu_server.o utilities.o registers.o memspace.o debugger.o disassembler.o \
	register_display.o decoder.o flag_handler.o formatI.o formatII.o formatIII.o usci.o port1.o bcm.o \
	timer_a.o packet_queue.o io.o
	${CXX} ${CXXFLAGS} -o $@ $^ ${LDLIBS}

main.o : main.cpp
	${CXX} ${CXXFLAGS} -c $<

utilities.o : devices/utilities.c
	${CXX} ${CXXFLAGS} -c $<

registers.o : devices/cpu/registers.c
	${CXX} ${CXXFLAGS} -c $<

memspace.o : devices/memory/memspace.c
	${CXX} ${CXXFLAGS} -c $<

debugger.o : debugger/debugger.c
	${CXX} ${CXXFLAGS} -c $<

disassembler.o : debugger/disassembler.c
	${CXX} ${CXXFLAGS} -c $<

register_display.o : debugger/register_display.c
	${CXX} ${CXXFLAGS} -c $<

decoder.o : devices/cpu/decoder.c
	${CXX} ${CXXFLAGS} -c $<

flag_handler.o : devices/cpu/flag_handler.c
	${CXX} ${CXXFLAGS} -c $<

formatI.o : devices/cpu/formatI.c
	${CXX} ${CXXFLAGS} -c $<

formatII.o : devices/cpu/formatII.c
	${CXX} ${CXXFLAGS} -c $<

formatIII.o : devices/cpu/formatIII.c
	${CXX} ${CXXFLAGS} -c $<

bcm.o : devices/peripherals/bcm.c
	${CXX} ${CXXFLAGS} -c $<

timer_a.o : devices/peripherals/timer_a.c
	${CXX} ${CXXFLAGS} -c $<

usci.o : devices/peripherals/usci.c
	${CXX} ${CXXFLAGS} -c $<

port1.o : devices/peripherals/port1.c
	${CXX} ${CXXFLAGS} -c $<

io.o: debugger/io.c
	${CXX} ${CXXFLAGS} -c $<

emu_server.o : debugger/websockets/emu_server.cpp
	${CXX} ${CXXFLAGS} -c $<

packet_queue.o : debugger/websockets/packet_queue.c
	${CXX} ${CXXFLAGS} -c debugger/websockets/packet_queue.c

# Server Program

${SERVER} : server.o
	${CXX} ${CXXFLAGS} -o $@ $^ ${LDLIBS}

server.o : debugger/server/server.c
	${CXX} ${CXXFLAGS} -c $<


clean :
	rm -f server.o main.o utilities.o emu_server.o registers.o \
		memspace.o debugger.o disassembler.o \
		register_display.o decoder.o flag_handler.o formatI.o \
		formatII.o formatIII.o io.o \
		usci.o port1.o packet_queue.o bcm.o timer_a.o \
		${EMULATOR} ${SERVER}
	${MAKE} -C test clean

test : ${EMULATOR} test/Makefile
	${MAKE} -C test test
