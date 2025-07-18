#pragma once

// srandard includes
#include <iostream>

// for communicating to Teensy over serial
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>


// servo controller class
class ServoComm {

    public:

        // constructor and destructor
        ServoComm(){};
        ~ServoComm();

        // initialize and setup serial communication
        bool initialize_comm(const std::string& port);

        // send command to Teensy over serial
        void command_opened_pos();
        void command_closed_pos();
        
    private:

        // initialize serial communication
        void setup_serial_port();
        
        // holds the serial port descrptions
        int teensy_serial_port = -1;
        struct termios tty;

};