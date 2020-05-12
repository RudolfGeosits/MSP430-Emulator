CXX=g++
CXXFLAGS=-g
LDLIBS=-lreadline -lwebsockets -lpthread -lrt -lssl -lcrypto
SERVER=server
EMULATOR=msp430-emu

.PHONY: all test clean

all: ${EMULATOR} ${SERVER}

# Main emulator program

${EMULATOR} : main.o emu_server.o utilities.o registers.o memspace.o debugger.o disassembler.o \
	register_display.o decoder.o flag_handler.o formatI.o formatII.o formatIII.o usci.o port1.o bcm.o \
	timer_a.o packet_queue.o io.o
	${CXX} ${CXXFLAGS} -o $@ $^ ${LDLIBS}

main.o : main.cpp main.h
	${CXX} ${CXXFLAGS} -c $<

utilities.o : devices/utilities.c devices/utilities.h
	${CXX} ${CXXFLAGS} -c $<

registers.o : devices/cpu/registers.c devices/cpu/registers.h
	${CXX} ${CXXFLAGS} -c $<

memspace.o : devices/memory/memspace.c devices/memory/memspace.h
	${CXX} ${CXXFLAGS} -c $<

debugger.o : debugger/debugger.c debugger/debugger.h
	${CXX} ${CXXFLAGS} -c $<

disassembler.o : debugger/disassembler.c debugger/disassembler.h
	${CXX} ${CXXFLAGS} -c $<

register_display.o : debugger/register_display.c debugger/register_display.h
	${CXX} ${CXXFLAGS} -c $<

decoder.o : devices/cpu/decoder.c devices/cpu/decoder.h
	${CXX} ${CXXFLAGS} -c $<

flag_handler.o : devices/cpu/flag_handler.c devices/cpu/flag_handler.h
	${CXX} ${CXXFLAGS} -c $<

formatI.o : devices/cpu/formatI.c devices/cpu/formatI.h
	${CXX} ${CXXFLAGS} -c $<

formatII.o : devices/cpu/formatII.c devices/cpu/formatII.h
	${CXX} ${CXXFLAGS} -c $<

formatIII.o : devices/cpu/formatIII.c devices/cpu/formatIII.h
	${CXX} ${CXXFLAGS} -c $<

bcm.o : devices/peripherals/bcm.c devices/peripherals/bcm.h
	${CXX} ${CXXFLAGS} -c $<

timer_a.o : devices/peripherals/timer_a.c devices/peripherals/timer_a.h
	${CXX} ${CXXFLAGS} -c $<

usci.o : devices/peripherals/usci.c devices/peripherals/usci.h
	${CXX} ${CXXFLAGS} -c $<

port1.o : devices/peripherals/port1.c devices/peripherals/port1.h
	${CXX} ${CXXFLAGS} -c $<

io.o: debugger/io.c debugger/io.h
	${CXX} ${CXXFLAGS} -c $<

emu_server.o : debugger/websockets/emu_server.cpp debugger/websockets/emu_server.h
	${CXX} ${CXXFLAGS} -c $<

packet_queue.o : debugger/websockets/packet_queue.c debugger/websockets/packet_queue.h
	${CXX} ${CXXFLAGS} -c $<

# Server Program

${SERVER} : server.o
	${CXX} ${CXXFLAGS} -o $@ $^ ${LDLIBS}

server.o : debugger/server/server.c debugger/server/server.h
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
