TOP_DIR=$(realpath ../)
EMU=${TOP_DIR}/MSP430
OUT_DIR=${TOP_DIR}/test/build
SRC_DIR=${TOP_DIR}/test/src

DEVICE=msp430g2553

CC = msp430-gcc
OBJCOPY = msp430-objcopy

CFLAGS= -mmcu=$(DEVICE) -msmall -Wall -Os -g

USCI_UART_TEST = usci_uart_test
MATH_TEST = math_test

.PHONY: all clean test ${USCI_UART_TEST} ${MATH_TEST}

all: ${OUT_DIR}/${USCI_UART_TEST}.bin ${OUT_DIR}/${MATH_TEST}.bin

${OUT_DIR}:
	mkdir -p $@

${OUT_DIR}/${USCI_UART_TEST}.bin: ${SRC_DIR}/${USCI_UART_TEST}.c ${OUT_DIR}
	${CC} $< -o ${OUT_DIR}/${USCI_UART_TEST}.elf -L ${SRC_DIR}
	${OBJCOPY} -O binary ${OUT_DIR}/${USCI_UART_TEST}.elf $@

${OUT_DIR}/${MATH_TEST}.bin: ${SRC_DIR}/${MATH_TEST}.c ${OUT_DIR}
	${CC} $< -o ${OUT_DIR}/${MATH_TEST}.elf -L ${SRC_DIR}
	${OBJCOPY} -O binary ${OUT_DIR}/${MATH_TEST}.elf $@

clean:
	rm -r -f ${OUT_DIR}

test: ${USCI_UART_TEST} ${MATH_TEST}

${USCI_UART_TEST}: ${OUT_DIR}/${USCI_UART_TEST}.bin
	# Setup input/output
	rm -r -f ${OUT_DIR}/${USCI_UART_TEST}_pipe.in
	rm -r -f ${OUT_DIR}/${USCI_UART_TEST}_pipe.out
	printf "Adventurer;" > ${OUT_DIR}/${USCI_UART_TEST}_pipe.in
	touch ${OUT_DIR}/${USCI_UART_TEST}_pipe.out
	# Run the simulator with the test binary
	printf "reset\ns 10000\nquit\n" |\
		${EMU} -m cli -b ${OUT_DIR}/${USCI_UART_TEST}.bin \
		-i ${OUT_DIR}/${USCI_UART_TEST}_pipe.in \
		-o ${OUT_DIR}/${USCI_UART_TEST}_pipe.out \
		> /dev/null
	# Create reference file
	printf "Hello there!Echo - Adventurer" > ${OUT_DIR}/${USCI_UART_TEST}_pipe.out.ref
	diff ${OUT_DIR}/${USCI_UART_TEST}_pipe.out ${OUT_DIR}/${USCI_UART_TEST}_pipe.out.ref

${MATH_TEST}: ${OUT_DIR}/${MATH_TEST}.bin
	# Setup output
	rm -r -f ${OUT_DIR}/${MATH_TEST}_pipe.out
	touch ${OUT_DIR}/${MATH_TEST}_pipe.out
	# Run the simulator with the test binary
	printf "reset\ns 10000\nquit\n" |\
		${EMU} -m cli -b ${OUT_DIR}/${MATH_TEST}.bin \
		-o ${OUT_DIR}/${MATH_TEST}_pipe.out \
		> /dev/null
	# Create reference file
	printf "Test calculation from 17 and 111 yields 44" > ${OUT_DIR}/${MATH_TEST}_pipe.out.ref
	diff ${OUT_DIR}/${MATH_TEST}_pipe.out ${OUT_DIR}/${MATH_TEST}_pipe.out.ref
