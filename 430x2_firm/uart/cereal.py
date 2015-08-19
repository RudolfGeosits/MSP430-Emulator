import time, serial, sys, threading

active = True
ser = None

def run():
    while active:
        out = ''
        
        while ser.inWaiting() > 0:
            out += ser.read(1)
            
        if out != '':
            sys.stdout.write(out)
            sys.stdout.flush()

ser = serial.Serial(
    port = '/dev/ttyUSB0',
    baudrate = 9600,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_TWO,
    bytesize = serial.EIGHTBITS,
    timeout = None,
)

t = threading.Thread(target = run)
t.start()

try:
    while True:
        input = raw_input()
        output = ""
        next3 = 0
        hexi = ""

        if input == 'exit':
            active = False
            exit()
        else:
            for c in input:
                if (next3 > 0):                                 
                    if (next3 == 1):
                        next3 += 1
                    elif (next3 == 2):
                        hexi += c
                        next3 += 1                        
                    elif (next3 == 3):
                        hexi += c
                        print "hexi is: " + hexi
                        output += hexi.decode('hex')
                        next3 = 0
                        hexi = ""
                elif (c == "\\"):
                    next3 = 1
                else:
                    output += c
                    
        print "output is " + output + "\n"
        ser.write(output + '\n\r')

except KeyboardInterrupt:
        print 'Interrupted'
        active = False
        exit()
