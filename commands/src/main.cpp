// srandard includes
#include <iostream>

// for communicating to Teensy over serial
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>

// main function
int main() {

    // open serial port
    // Replace with your Teensy's port (e.g., 'COM3' on Windows or '/dev/ttyACM0' on Linux)
    // NOTE: use "ls /dev/ttyACM*" to find the correct port
    int serial_port = open("/dev/ttyACM0", O_RDWR);
    if (serial_port < 0) {
        std::cerr << "Failed to open serial port!\n";
        return 1;
    }

    // configure serial port settings
    struct termios tty;
    tcgetattr(serial_port, &tty);

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
    tcsetattr(serial_port, TCSANOW, &tty);

    // beginning of communication loop
    while (true) {
        std::cout << "Enter 1 (on), 0 (off), or q to quit: ";
        std::string input;
        std::cin >> input;
        if (input == "q") break;
        if (input == "1" || input == "0") {
            write(serial_port, input.c_str(), 1);
        }
    }

    std::cout << "Closing serial port...\n";

    close(serial_port);
    return 0;
}