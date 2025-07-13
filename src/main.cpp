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

// position variables
bool armed = false;                // current state of the servos
const double deg_pos_0 = 30.0;     // [deg] position when "OFF" (0)
const double deg_pos_1 = 90.0;     // [deg] position when "ON" (1)
double interp_start_deg, interp_end_deg;  // interpolation start and end positions

// interpolation variables
const double T_interp = 250.0;  // [ms] time to ramp from one position to another
double t_start, t_now;
double alpha, deg_pos;

// LED indicator
bool LED_STATE = false;

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

    // servos go to initial position
    servo1.write(deg_pos_0);
    servo2.write(deg_pos_0);
    servo3.write(deg_pos_0);
    servo4.write(deg_pos_0);
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
            interp_start_deg = deg_pos_1;
            interp_end_deg = deg_pos_0;
        } 
        else if (command_curr == 1) {
            interp_start_deg = deg_pos_0;
            interp_end_deg = deg_pos_1;
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
            deg_pos = (1.0 - alpha) * interp_start_deg + alpha * interp_end_deg;
            
            // write the position to the servo
            servo1.write(deg_pos);
            servo2.write(deg_pos);
            servo3.write(deg_pos);
            servo4.write(deg_pos);
            
            // set LED indicator to ON when command
            LED_STATE = !LED_STATE;
            digitalWrite(LED_BUILTIN, LED_STATE);
            
            // delay just because
            delay(50);
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
            servo1.write(deg_pos_0);
            servo2.write(deg_pos_0);
            servo3.write(deg_pos_0);
            servo4.write(deg_pos_0);
        } 
        else if (command_curr == 1) {
            // set LED indicator to ON when command
            LED_STATE = true;
            digitalWrite(LED_BUILTIN, LED_STATE);
            
            // write the position to the servo
            servo1.write(deg_pos_1);
            servo2.write(deg_pos_1);
            servo3.write(deg_pos_1);
            servo4.write(deg_pos_1);
        }
    }

    // delay to avoid flooding the serial port
    delay(10);
}

