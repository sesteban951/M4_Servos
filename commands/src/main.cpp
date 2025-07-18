/*
    Simple script to control servos over Serial communication.
    WARNING: 1) Do not communicate over Serial while actively uploading to Arduino.
             2) Use "ls /dev/ttyACM*" to find the correct port. This is different on Windows (COM*).

    Servo details:
    - Savox SV1261MG - Mini Digital High Voltage Aluminum Case Servo 0.095/277@ 7.4V
    - https://www.savoxusa.com/products/sv1261mg-mini-digital-high-voltage-aluminum-case-servo-0-095-277-7-4v#technical-details
*/

// srandard includes
#include "servo_comm.hpp"
#include <chrono>
#include <thread>

// main function
int main() {

    // create servo communication object
    ServoComm servo_comm;

    // initialize serial communication
    bool comm_initilized = servo_comm.initialize_comm("/dev/ttyACM0");

    double seconds_to_wait = 3.0;

    if (comm_initilized) {

        std::cout << "Commanded CLOSED position..." << std::endl;
        servo_comm.command_locked_pos();
        std::this_thread::sleep_for(std::chrono::milliseconds(int(seconds_to_wait * 1000)));

        std::cout << "Commanded OPENED position..." << std::endl;
        servo_comm.command_opened_pos();
        std::this_thread::sleep_for(std::chrono::milliseconds(int(seconds_to_wait * 1000)));

        std::cout << "Commanded CLOSED position..." << std::endl;
        servo_comm.command_locked_pos();
        std::this_thread::sleep_for(std::chrono::milliseconds(int(seconds_to_wait * 1000)));

        std::cout << "Commanded OPENED position..." << std::endl;
        servo_comm.command_opened_pos();
        std::this_thread::sleep_for(std::chrono::milliseconds(int(seconds_to_wait * 1000)));

    } else {
        std::cerr << "Failed to initialize communication. Exiting." << std::endl;
    }
    

    return 0;
}