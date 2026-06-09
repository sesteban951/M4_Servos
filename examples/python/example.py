#
#  Simple script to control servos over Serial communication.
#  WARNING: 1) Do not communicate over Serial while actively uploading to Arduino.
#           2) Use "ls /dev/ttyACM*" to find the correct port. This is different on Windows (COM*).
#           3) Use 'sudo' for serial port access on Linux.
#
#  Servo details:
#  - Savox SV1261MG - Mini Digital High Voltage Aluminum Case Servo 0.095/277@ 7.4V
#  - https://www.savoxusa.com/products/sv1261mg-mini-digital-high-voltage-aluminum-case-servo-0-095-277-7-4v#technical-details
#

import sys
import time

from servo_comm import ServoComm


def main():

    # create servo communication object
    servo_comm = ServoComm()

    # initialize serial communication
    comm_initialized = servo_comm.initialize_comm("/dev/ttyACM0")

    seconds_to_wait = 3.0

    if comm_initialized:

        print("Commanded CLOSED position...")
        servo_comm.command_locked_pos()
        time.sleep(seconds_to_wait)

        print("Commanded OPENED position...")
        servo_comm.command_opened_pos()
        time.sleep(seconds_to_wait)

        print("Commanded CLOSED position...")
        servo_comm.command_locked_pos()
        time.sleep(seconds_to_wait)

        print("Commanded OPENED position...")
        servo_comm.command_opened_pos()
        time.sleep(seconds_to_wait)

    else:
        print("Failed to initialize communication. Exiting.", file=sys.stderr)


if __name__ == "__main__":
    main()
