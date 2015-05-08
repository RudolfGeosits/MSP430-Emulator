# msp430\_usi\_i2c

This is a small library that implements an I2C (IIC, I²C, or I squared C) master on MSP430 devices that only have the USI module (for example, MSP430G2412 or MSP430G2452).

# License

MIT. I believe in freedom, which means I believe in letting you do whatever you want with this code.

# Features

* Small.
* Works.
* Reads and writes.
* Implements repeated start.
* Uses the Bus Pirate convention.

# Rationale

I wrote this out of frustration. There is lots of code floating around, most of which I didn't like. TI supplies examples which seem to have been written by an intern and never looked at again. The examples are overly complex, unusable in practical applications, ugly and badly formatted, and sometimes even incorrect.

The MSP430G2xx2 devices are tiny and inexpensive and could be used in many application requiring I2C, but many people avoid them because it is so annoyingly difficult to use I2C with the USI module.

This code is very, very loosely based on the `msp430g2xx2_usi_16.c` example from TI, but if you compare you will notice that:

* the state machine is different (simpler): see `doc/usi-i2c-state-diagram.pdf` for details,
* it actually has a useful interface,
* it is smaller.

# Limitations

This is a simple I2C master that needs to fit on devices that have 128 bytes of RAM, so scale your expectations
accordingly. There is no error detection, no arbitration loss detection, only master mode is implemented. Addressing is
fully manual: it is your responsibility to shift the 7-bit I2C address to the left and add the R/W bit (actually, I see this as an advantage).

# Usage

There are two functions: `i2c_init()` and `i2c_send_sequence()`.

You call `i2c_init()` once to initialize the USI module. You have to provide the constants used to configure the USI
clock: one of the `USIDIV_*` constants as a `usi_clock_divider` parameter, which will set the clock divider used for USI I2C
communications. The `usi_clock_source` parameter should be set to one of the `USISSEL*` constants. As an example,
`i2c_init(USIDIV_5, USISSEL_2)` uses SMCLK/16.

Data transmission (both transmit and receive) is handled by `i2c_send_sequence()`. It sends a command/data sequence that
can include restarts, writes and reads. Every transmission begins with a START, and ends with a STOP so you do not have
to specify that.

`i2c_send_sequence()` will busy-spin if another transmission is in progress. Note that this is interrupt-driven asynchronous code: you can't just call `i2c_send_sequence` from an interrupt handler and expect it to work: you risk a deadlock if another transaction is in progress and nothing will happen before the interrupt handler is done anyway. So the proper way to use this is in normal code. This should not be a problem, as performing such lengthy tasks as I2C communication inside an interrupt handler is a bad idea anyway. `wakeup_sr_bits` should be a bit mask of bits to clear in the SR register when the transmission is completed (to exit LPM0: `LPM0_BITS` (`CPUOFF`), for LPM3: `LPM3_bits` (`SCG1+SCG0+CPUOFF`))

`i2c_send_sequence()` takes four parameters:

* `handle` is the handle returned from `i2c_open()`
* `sequence` is the I2C operation sequence that should be performed. It can include any number of writes, restarts and reads. Note that the sequence is composed of `uint16_t`, not `uint8_t` elements. This is because we have to support out-of-band signalling of `I2C_RESTART` and `I2C_READ` operations, while still passing through 8-bit data.
* `sequence_length` is the number of sequence elements (not bytes). Sequences of length up to 65535 are supported.
* `received_data` should point to a buffer that can hold as many bytes as there are `I2C_READ` operations in the   sequence. If there are no reads, 0 can be passed, as this parameter will not be used.

`i2c_send_sequence()` uses the Bus Pirate I2C convention, which I found to be very useful and compact. As an example, this
Bus Pirate sequence:

	 "[0x38 0x0c [ 0x39 r ]"

is specified as:

	 {0x38, 0x0c, I2C_RESTART, 0x39, I2C_READ};

in I2C terms, this sequence means:

1. Write 0x0c to device 0x1c (0x0c is usually the register address).
2. Do not release the bus.
3. Issue a repeated start.
4. Read one byte from device 0x1c (which would normally be the contents of register 0x0c on that device).

The sequence may read multiple bytes:

	{0x38, 0x16, I2C_RESTART, 0x39, I2C_READ, I2C_READ, I2C_READ};

This will normally read three bytes from device 0x1c starting at register 0x16. In this case you need to provide a pointer to a buffer than can hold three bytes.

Note that start and stop are added for you automatically, but addressing is fully manual: it is your responsibility to shift the 7-bit I2C address to the left and add the R/W bit. The examples above communicate with a device whose I2C address is 0x1c, which shifted left gives 0x38. For reads we use 0x39, which is `(0x1c<<1)|1`.

If you wonder why I consider the Bus Pirate convention useful, note that what you specify in the sequence is very close to the actual bytes on the wire. This makes debugging and reproducing other sequences easy. Also, you can use the Bus Pirate to prototype, and then easily convert the tested sequences into actual code.

Steps to use this code:

1. Add the files to your project.

2. Call `i2c_init()` with appropriate USI settings:

	i2c_init(USIDIV_5, USISSEL_2);

3. Communicate. Here's an example of performing a read with repeated start (restart) from an MMA845x accelerometer. Note that the chip goes into LPM0 sleep while I2C transmission is interrupt-driven. We will get woken up after the transmit/receive is done, but we still check `i2c_done()` just in case something else woke us up.

```
uint16_t mma8453_read_interrupt_source[] = {0x38, 0x0c, I2C_RESTART, 0x39, I2C_READ};
uint8_t status;
i2c_send_sequence(mma8453_read_interrupt_source, 5, &status, LPM0_BITS);
LPM0;
while(!i2c_done());
```


# Does it work?

It does for me: I've been using this code in a number of projects and had no problems with it. I've used it on
MSP430G2412 and MSP430G2452 chips. That said, there are no guarantees.