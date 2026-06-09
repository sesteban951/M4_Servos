#
#  Python servo controller class (mirrors src/servo_comm.hpp / src/servo_comm.cpp).
#  Communicates with the Teensy over a serial port.
#

import serial


# servo controller class
class ServoComm:

    # constructor
    def __init__(self):
        self.teensy_serial_port = None

    # destructor: close the serial port if it is open
    def __del__(self):
        if self.teensy_serial_port is not None and self.teensy_serial_port.is_open:
            self.teensy_serial_port.close()
            print("Closed Teensy serial port.")

    # initialize and setup serial communication
    def initialize_comm(self, port):

        try:
            # open Teensy serial port with 9600 baud, 8N1
            self.teensy_serial_port = serial.Serial(
                port=port,
                baudrate=9600,
                bytesize=serial.EIGHTBITS,
                parity=serial.PARITY_NONE,
                stopbits=serial.STOPBITS_ONE,
                timeout=1,
            )

        # serial port failed to open
        except serial.SerialException:
            print(f"Failed to open Teensy serial port at {port}.")
            return False

        # serial port opened successfully
        print(f"Opened Teensy serial port at {port}.")
        return True

    # command the servo to be in the open positions
    def command_opened_pos(self):
        self.teensy_serial_port.write(b"0")

    # command the servo to be in the closed positions
    def command_locked_pos(self):
        self.teensy_serial_port.write(b"1")
