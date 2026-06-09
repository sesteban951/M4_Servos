#
#  Simple script to manually control servos over Serial communication.
#  WARNING: 1) Do not communicate over Serial while actively uploading to Arduino.
#           2) Use "ls /dev/ttyACM*" to find the correct port. This is different on Windows (COM*).
#           3) Use 'sudo' for serial port access on Linux.
#
#  Servo details:
#  - Savox SV1261MG - Mini Digital High Voltage Aluminum Case Servo 0.095/277@ 7.4V
#  - https://www.savoxusa.com/products/sv1261mg-mini-digital-high-voltage-aluminum-case-servo-0-095-277-7-4v#technical-details
#

import sys

from servo_comm import ServoComm


def main(argv):

    # Check if the user provided a device path
    if len(argv) < 2:
        print("Error: No serial device specified.", file=sys.stderr)
        print(f"\tUsage:   sudo python3 {argv[0]} <serial_device>", file=sys.stderr)
        print(f"\tExample: sudo python3 {argv[0]} /dev/ttyACM0", file=sys.stderr)
        print("\tUse [ls /dev/ttyACM*] to find the correct port. This is different on Windows (COM*).", file=sys.stderr)
        return 1

    device_path = argv[1]
    print(f"Attempting to open serial device: {device_path}")

    # create servo communication object and initialize
    servo_comm = ServoComm()
    if not servo_comm.initialize_comm(device_path):
        return 1

    print("-------------------------------------------------------")

    # communication loop
    while True:
        user_input = input("Enter 1 (LOCKED), 0 (OPENED), or q to quit: ").strip()
        if user_input == "q":
            break
        if user_input == "1":
            servo_comm.command_locked_pos()
        elif user_input == "0":
            servo_comm.command_opened_pos()

    print("Closing Teensy serial port...")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
