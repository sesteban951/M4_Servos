/*
    Simple script to control servos over Serial communication.
    WARNING: 1) Do not communicate over Serial while actively uploading to Arduino.
             2) Use "ls /dev/ttyACM*" to find the correct port. This is different on Windows (COM*).
             3) Use 'sudo' for serial port access on Linux.

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

// main function
int main(int argc, char* argv[]) {
    
    // Check if the user provided a device path
    if (argc < 2) {

        // print error message
        std::cerr << "Error: No serial device specified.\n";
        std::cerr << "\tUsage:   sudo" << argv[0] << " <serial_device>\n";
        std::cerr << "\tExample: sudo" << argv[0] << " /dev/ttyACM0\n";
        return 1;
    }

    const char* device_path = argv[1];
    std::cout << "Attempting to open serial device: " << device_path << "\n";

    // open Teensy serial port
    int teensy_serial_port = open(device_path, O_RDWR);
    if (teensy_serial_port < 0) {
        std::cerr << "Failed to open Teensy serial port.\n";
        return 1;
    }
    else {
        std::cout << "Teensy serial port opened successfully.\n";
        std::cout << "-------------------------------------------------------\n";
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

    // communication loop
    while (true) {
        std::cout << "Enter 1 (LOCKED), 0 (OPENED), or q to quit: ";
        std::string input;
        std::cin >> input;
        if (input == "q") break;
        if (input == "1" || input == "0") {
            write(teensy_serial_port, input.c_str(), 1);
        }
    }

    // while (true) {
    //     std::cout << "Enter servo position (00–99) or q to quit: ";
    //     std::string input;
    //     std::cin >> input;

    //     if (input == "q") break;

    //     if (input.length() == 1) input = "0" + input;  // pad with leading zero

    //     if (input.length() == 2 && isdigit(input[0]) && isdigit(input[1])) {
    //         write(teensy_serial_port, input.c_str(), 2);  // send 2 bytes
    //     } else {
    //         std::cerr << "Invalid input. Enter a number between 00 and 99.\n";
    //     }
    // }

    std::cout << "Closing Teensy serial port...\n";

    close(teensy_serial_port);
    return 0;
}