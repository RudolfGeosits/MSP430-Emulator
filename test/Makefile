TOP_DIR=$(realpath ../)
EMU=${TOP_DIR}/MSP430
OUT_DIR=${TOP_DIR}/test/build
SRC_DIR=${TOP_DIR}/test/src

DEVICE=msp430g2553

CC = msp430-gcc
OBJCOPY = msp430-objcopy

CFLAGS= -mmcu=$(DEVICE) -msmall -Wall -Os -g

USCI_UART_TEST = usci_uart_test

.PHONY: all clean test uart_test

all: ${OUT_DIR}/${USCI_UART_TEST}.bin

${OUT_DIR}:
	mkdir -p $@

${OUT_DIR}/${USCI_UART_TEST}.bin: ${SRC_DIR}/${USCI_UART_TEST}.c ${OUT_DIR}
	${CC} $< -o ${OUT_DIR}/${USCI_UART_TEST}.elf -L ${SRC_DIR}
	${OBJCOPY} -O binary ${OUT_DIR}/${USCI_UART_TEST}.elf $@

clean:
	rm -r -f ${OUT_DIR}

test: uart_test

uart_test: ${OUT_DIR}/${USCI_UART_TEST}.bin
	rm -r -f ${OUT_DIR}/uart_pipe.in
	rm -r -f ${OUT_DIR}/uart_pipe.out
	printf "Adventurer;" > ${OUT_DIR}/uart_pipe.in
	touch ${OUT_DIR}/uart_pipe.out	
	printf "reset\ns 10000\nquit\n" |\
		${EMU} -m cli -b ${OUT_DIR}/${USCI_UART_TEST}.bin \
		-i ${OUT_DIR}/uart_pipe.in \
		-o ${OUT_DIR}/uart_pipe.out \
		> /dev/null
	printf "Hello there!Echo - Adventurer" > ${OUT_DIR}/uart_pipe.out.ref
	diff ${OUT_DIR}/uart_pipe.out ${OUT_DIR}/uart_pipe.out.ref