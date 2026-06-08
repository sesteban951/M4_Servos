#include "servo_comm.hpp"

// Destructor
ServoComm::~ServoComm() {

    // if the serial port is open, close it
    if (this->teensy_serial_port >= 0){
        close(this->teensy_serial_port);
    }

    std::cout << "Closed Teensy serial port." << std::endl;
};

// intialize the serial communication
bool ServoComm::initialize_comm(const std::string& port) {

    // open Teensy serial port
    this->teensy_serial_port = open(port.c_str(), O_RDWR);

    // serial port failed to open
    if (this->teensy_serial_port < 0) {

        // print error message
        std::cerr << "Failed to open Teensy serial port at " << port << "." << std::endl;
        return false;
    }

    // serial port opened successfully
    else {

        // setup the serial port settings
        this->setup_serial_port();

        // print success message
        std::cout << "Opened Teensy serial port at " << port << "." << std::endl;
        return true;
    }
}

// setup the serial port settings
void ServoComm::setup_serial_port() {

    // configure serial port settings
    tcgetattr(this->teensy_serial_port, &this->tty);

    // set baud rate
    cfsetispeed(&this->tty, B9600);
    cfsetospeed(&this->tty, B9600);

    // set other serial port settings
    this->tty.c_cflag &= ~PARENB; // no parity
    this->tty.c_cflag &= ~CSTOPB; // one stop bit
    this->tty.c_cflag &= ~CSIZE;
    this->tty.c_cflag |= CS8;     // 8 bits per byte
    this->tty.c_cflag |= CREAD | CLOCAL;

    // apply settings
    tcsetattr(this->teensy_serial_port, TCSANOW, &this->tty);
}

// command the servo to be in the open positions
void ServoComm::command_opened_pos() {

    // send the open command to the Teensy
    std::string command;
    command = "0";

    // write to the serial port
    write(this->teensy_serial_port, command.c_str(), 1);
}

// command the servo to be in the closed positions
void ServoComm::command_locked_pos() {

    // send the closed command to the Teensy
    std::string command;
    command = "1";

    // write to the serial port
    write(this->teensy_serial_port, command.c_str(), 1);
}