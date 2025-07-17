/*
    Simple Arduino sketch to control servos over Serial communication.
    WARNING: 1) Do not upload this sketch while actively communicating over Serial.
             2) Make sure to set the correct serial port in the parent program.
             3) You cannot open the Serial Monitor in Arduino IDE while this sketch is running
                since it uses the same serial bus.

    Servo details:
    - Savox SV1261MG - Mini Digital High Voltage Aluminum Case Servo 0.095/277@ 7.4V
    - https://www.savoxusa.com/products/sv1261mg-mini-digital-high-voltage-aluminum-case-servo-0-095-277-7-4v#technical-details
*/

// standard inlucdes
#include <Arduino.h>

// standard servo library
#include <Servo.h>

// servo pins on Teensy 4.0
// WARNING: Do not change pins this as they are permanently soldered. 
//          Same thing with baud rate.
#define SERVO1_PIN 14
#define SERVO2_PIN 15
#define SERVO3_PIN 18
#define SERVO4_PIN 19
#define BAUD_RATE 9600

// servo objects
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// command variables
int command_prev = 0;         // previous command received
int command_curr = 0;         // current command received 
bool command_change = false;  // flag to indicate if command changed

// interpo start and position variables
double interp_start_deg_servo1, interp_end_deg_servo1;
double interp_start_deg_servo2, interp_end_deg_servo2;
double interp_start_deg_servo3, interp_end_deg_servo3;
double interp_start_deg_servo4, interp_end_deg_servo4;

// interpolation variables
const double T_interp = 50.0;  // [ms] time to ramp from one position to another
double t_start, t_now;
double alpha;
double deg_pos_servo1;
double deg_pos_servo2;
double deg_pos_servo3;
double deg_pos_servo4;

// LED indicator
bool LED_STATE = false;

// open positions for the servos
const double deg_pos_open_servo1 = 20.0;
const double deg_pos_open_servo2 = 35.0;
const double deg_pos_open_servo3 = 35.0;
const double deg_pos_open_servo4 = 40.0;

// locked positions for the servos
const double deg_pos_lock_servo1 = 65.0;
const double deg_pos_lock_servo2 = 90.0;
const double deg_pos_lock_servo3 = 99.0;
const double deg_pos_lock_servo4 = 99.0;


void setup()
{
    // begin the serial communication over USB. This is the USB cable connected to the computer.
    Serial.begin(BAUD_RATE);

    // Attach the servo pins to the Teensy
    servo1.attach(SERVO1_PIN);
    servo2.attach(SERVO2_PIN);
    servo3.attach(SERVO3_PIN);
    servo4.attach(SERVO4_PIN);

    // builtinLED to indicate status
    pinMode(LED_BUILTIN, OUTPUT);

    // servos go to open 
    servo1.write(deg_pos_open_servo1);
    servo2.write(deg_pos_open_servo2);
    servo3.write(deg_pos_open_servo3);
    servo4.write(deg_pos_open_servo4);

    // servo1.write(deg_pos_lock_servo1);
    // servo2.write(deg_pos_lock_servo2);
    // servo3.write(deg_pos_lock_servo3);
    // servo4.write(deg_pos_lock_servo4);
}


void loop()
{

    // check if there is data available on the serial port
    if (Serial.available()) {       
 
        // read the command from serial
        char cmd = Serial.read();
 
        // set the current command
        if (cmd == '0') {
            command_curr = 0;  // unarm command
        } 
        else if (cmd == '1') {
            command_curr = 1;  // arm command
        } 
        else {
            // really bad, detach the servos. TODO: this is not working
            command_curr = -1;
            servo1.detach();
            servo2.detach();
            servo3.detach();
            servo4.detach();
            // pinMode(SERVO1_PIN, INPUT);
            // pinMode(SERVO2_PIN, INPUT);
            // pinMode(SERVO3_PIN, INPUT);
            // pinMode(SERVO4_PIN, INPUT);
        }
    }
    // see if there has been an command change
    if (command_curr != command_prev) {
        
        // set flag 
        command_change = true;
        command_prev = command_curr;
        
        // record start time
        t_start = millis();
        
        // set interpolation targets
        if (command_curr == 0) {
            // interp_start_deg = deg_pos_1;
            // interp_end_deg = deg_pos_0;

            interp_start_deg_servo1 = deg_pos_lock_servo1;
            interp_end_deg_servo1 = deg_pos_open_servo1;

            interp_start_deg_servo2 = deg_pos_lock_servo2;
            interp_end_deg_servo2 = deg_pos_open_servo2;

            interp_start_deg_servo3 = deg_pos_lock_servo3;
            interp_end_deg_servo3 = deg_pos_open_servo3;

            interp_start_deg_servo4 = deg_pos_lock_servo4;
            interp_end_deg_servo4 = deg_pos_open_servo4;
        } 
        else if (command_curr == 1) {
            // interp_start_deg = deg_pos_0;
            // interp_end_deg = deg_pos_1;

            interp_start_deg_servo1 = deg_pos_open_servo1;
            interp_end_deg_servo1 = deg_pos_lock_servo1;

            interp_start_deg_servo2 = deg_pos_open_servo2;
            interp_end_deg_servo2 = deg_pos_lock_servo2;

            interp_start_deg_servo3 = deg_pos_open_servo3;
            interp_end_deg_servo3 = deg_pos_lock_servo3;

            interp_start_deg_servo4 = deg_pos_open_servo4;
            interp_end_deg_servo4 = deg_pos_lock_servo4;
        }
    } 

    // interpolate to new desired state
    if (command_change == true) {
 
        // get current time
        t_start = millis();

        // interpolate positions
        while (millis() - t_start < T_interp) {
     
            // compute interpolation factor
            t_now = millis() - t_start;
            alpha = t_now / T_interp;
     
            // interpolate position
            // deg_pos = (1.0 - alpha) * interp_start_deg + alpha * interp_end_deg;

            deg_pos_servo1 = (1.0 - alpha) * interp_start_deg_servo1 + alpha * interp_end_deg_servo1;
            deg_pos_servo2 = (1.0 - alpha) * interp_start_deg_servo2 + alpha * interp_end_deg_servo2;
            deg_pos_servo3 = (1.0 - alpha) * interp_start_deg_servo3 + alpha * interp_end_deg_servo3;
            deg_pos_servo4 = (1.0 - alpha) * interp_start_deg_servo4 + alpha * interp_end_deg_servo4;
     
            // write the position to the servo
            servo1.write(deg_pos_servo1);
            servo2.write(deg_pos_servo2);
            servo3.write(deg_pos_servo3);
            servo4.write(deg_pos_servo4);
        }

        // reset command change flag
        command_change = false;
    }

    // no new command, keep previous command
    else {
        if (command_curr == 0) {
            // set LED indicator to OFF when discommand
            LED_STATE = true;
            digitalWrite(LED_BUILTIN, LOW);
     
            // write the position to the servo
            servo1.write(deg_pos_open_servo1);
            servo2.write(deg_pos_open_servo2);
            servo3.write(deg_pos_open_servo3);
            servo4.write(deg_pos_open_servo4);
        } 
        else if (command_curr == 1) {
            // set LED indicator to ON when command
            LED_STATE = true;
            digitalWrite(LED_BUILTIN, LED_STATE);
     
            // write the position to the servo
            servo1.write(deg_pos_lock_servo1);
            servo2.write(deg_pos_lock_servo2);
            servo3.write(deg_pos_lock_servo3);
            servo4.write(deg_pos_lock_servo4);
        }
    }
    
    // delay to avoid flooding the serial port
    delay(10);
}

