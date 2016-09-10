all: MSP430 SERVER clean

MSP430 : main.o utilities.o emu_server.o registers.o memspace.o debugger.o disassembler.o \
	register_display.o decoder.o flag_handler.o formatI.o formatII.o formatIII.o \
	usci.o port1.o packet_queue.o bcm.o timer_a.o

	cc -o MSP430 main.o emu_server.o utilities.o registers.o memspace.o debugger.o disassembler.o \
	register_display.o decoder.o flag_handler.o formatI.o formatII.o formatIII.o usci.o port1.o bcm.o timer_a.o packet_queue.o -lreadline -L"libs/libwebsockets-2.0.2/" -lwebsockets -lpthread -lrt -lssl -lcrypto;

main.o : main.c
	cc -c main.c

utilities.o : devices/utilities.c
	cc -c devices/utilities.c

registers.o : devices/cpu/registers.c
	cc -c devices/cpu/registers.c

memspace.o : devices/memory/memspace.c
	cc -c devices/memory/memspace.c

debugger.o : debugger/debugger.c
	cc -c debugger/debugger.c

disassembler.o : debugger/disassembler.c
	cc -c debugger/disassembler.c

register_display.o : debugger/register_display.c
	cc -c debugger/register_display.c

decoder.o : devices/cpu/decoder.c
	cc -c devices/cpu/decoder.c

flag_handler.o : devices/cpu/flag_handler.c
	cc -c devices/cpu/flag_handler.c

formatI.o : devices/cpu/formatI.c
	cc -c devices/cpu/formatI.c

formatII.o : devices/cpu/formatII.c
	cc -c devices/cpu/formatII.c

formatIII.o : devices/cpu/formatIII.c
	cc -c devices/cpu/formatIII.c

bcm.o : devices/peripherals/bcm.c
	cc -c devices/peripherals/bcm.c

timer_a.o : devices/peripherals/timer_a.c
	cc -c devices/peripherals/timer_a.c

usci.o : devices/peripherals/usci.c
	cc -c devices/peripherals/usci.c

port1.o : devices/peripherals/port1.c
	cc -c devices/peripherals/port1.c

emu_server.o : debugger/websockets/emu_server.c
	cc -c debugger/websockets/emu_server.c

packet_queue.o : debugger/websockets/packet_queue.c
	cc -c debugger/websockets/packet_queue.c

# Server Program

SERVER : server.o
	cc -o server server.o -lrt -lpthread -L"libs/libwebsockets-2.0.2/" -lwebsockets -lssl -lcrypto;

server.o : debugger/server/server.c
	cc -c debugger/server/server.c


clean :
	rm server.o main.o utilities.o emu_server.o registers.o \
	memspace.o debugger.o disassembler.o \
	register_display.o decoder.o flag_handler.o formatI.o \
	formatII.o formatIII.o \
	usci.o port1.o packet_queue.o bcm.o timer_a.o \
	*.bin *.tmp *.elf;
