#
#  Simple script to control servos over Serial communication.
#  WARNING: 1) Do not communicate over Serial while actively uploading to Arduino.
#           2) Use "ls /dev/ttyACM*" to find the correct port. This is different on Windows (COM*).
#  Servo details:
#  - Savox SV1261MG - Mini Digital High Voltage Aluminum Case Servo 0.095/277@ 7.4V
#  - https://www.savoxusa.com/products/sv1261mg-mini-digital-high-voltage-aluminum-case-servo-0-095-277-7-4v#technical-details
#

import serial
import time

# open serial port
# Replace with your Teensy's port (e.g., 'COM3' on Windows or '/dev/ttyACM0' on Linux)
# NOTE: use "ls /dev/ttyACM*" to find the correct port
PORT = '/dev/ttyACM0'
BAUD = 9600

# setup the serial connection
ser = serial.Serial(PORT, BAUD, timeout=1)
time.sleep(1)  # wait to initialize

# beginning of communication loop
while True:
    cmd = input("Enter 1 (on) or 0 (off), or q to quit: ")
    if cmd == 'q':
        break
    elif cmd in ['0', '1']:
        ser.write(cmd.encode())
    else:
        print("Invalid input.")

print("Closing serial port...\n")
ser.close()
