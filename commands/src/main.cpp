/*
    Simple script to control servos over Serial communication.
    WARNING: 1) Do not communicate over Serial while actively uploading to Arduino.
             2) Use "ls /dev/ttyACM*" to find the correct port. This is different on Windows (COM*).

    Servo details:
    - Savox SV1261MG - Mini Digital High Voltage Aluminum Case Servo 0.095/277@ 7.4V
    - https://www.savoxusa.com/products/sv1261mg-mini-digital-high-voltage-aluminum-case-servo-0-095-277-7-4v#technical-details
*/

// srandard includes
#include <iostream>

// for communicating to Teensy over serial
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>

// main function
int main() {

    // open  Teensy serial port
    int teensy_serial_port = open("/dev/ttyACM0", O_RDWR); // NOTE: (2)
    if (teensy_serial_port < 0) {
        std::cerr << "Failed to open Teensy serial port!\n";
        return 1;
    }

    // configure serial port settings
    struct termios tty;
    tcgetattr(teensy_serial_port, &tty);

    // set baud rate
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    // set other serial port settings
    tty.c_cflag &= ~PARENB; // no parity
    tty.c_cflag &= ~CSTOPB; // one stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;     // 8 bits per byte
    tty.c_cflag |= CREAD | CLOCAL;

    // apply settings
    tcsetattr(teensy_serial_port, TCSANOW, &tty);

    // beginning of communication loop
    while (true) {
        std::cout << "Enter 1 (ON), 0 (OFF), or q to quit: ";
        std::string input;
        std::cin >> input;
        if (input == "q") break;
        if (input == "1" || input == "0") {
            write(teensy_serial_port, input.c_str(), 1);
        }
    }

    std::cout << "Closing Teensy serial port...\n";

    close(teensy_serial_port);
    return 0;
}