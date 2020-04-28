TOP_DIR=$(realpath ../)
OUT_DIR=${TOP_DIR}/test/build
SRC_DIR=${TOP_DIR}/test/src

DEVICE=msp430g2553

CC = msp430-gcc
OBJCOPY = msp430-objcopy

CFLAGS= -mmcu=$(DEVICE) -msmall -Wall -Os -g

USCI_UART_TEST = usci_uart_test

.PHONY: all clean test

all: ${OUT_DIR}/${USCI_UART_TEST}.bin

${OUT_DIR}:
	mkdir -p $@

${OUT_DIR}/${USCI_UART_TEST}.bin: ${SRC_DIR}/${USCI_UART_TEST}.c ${OUT_DIR}
	${CC} $< -o ${OUT_DIR}/${USCI_UART_TEST}.elf -L ${SRC_DIR}
	${OBJCOPY} -O binary ${OUT_DIR}/${USCI_UART_TEST}.elf $@

clean:
	rm -r -f ${OUT_DIR}
