MSP430 : main.o utilities.o server.o registers.o memspace.o debugger.o disassembler.o \
	register_display.o decoder.o flag_handler.o formatI.o formatII.o formatIII.o \
	usci.o port1.o

	cc -o MSP430 main.o server.o utilities.o registers.o memspace.o debugger.o disassembler.o \
	register_display.o decoder.o flag_handler.o formatI.o formatII.o formatIII.o usci.o port1.o \
	-lreadline -lwebsockets -lpthread

main.o : main.c
	cc -c main.c

server.o : debugger/server/server.c
	cc -c debugger/server/server.c	

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

usci.o : devices/peripherals/usci.c
	cc -c devices/peripherals/usci.c

port1.o : devices/peripherals/port1.c
	cc -c devices/peripherals/port1.c

clean :
	rm main.o server.o utilities.o registers.o memspace.o debugger.o disassembler.o register_display.o decoder.o flag_handler.o formatI.o formatII.o formatIII.o usci.o port1.o tmp.* *~

