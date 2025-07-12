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

// servo ocontrol parameters
int armed = 0;              // armed state, 0 = not armed, 1 = armed
const double deg_pos_0 = 45.0;  // [deg] position when "OFF" (0)
const double deg_pos_1 = 90.0;  // [deg] position when "ON" (1)
const double pwm_pos_0 = 800.0;   // [us] position when "OFF" (0)
const double pwm_pos_1 = 2200.0;  // [us] position when "ON" (1)

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
}

void loop()
{
    // check if there is data available on the serial port
    if (Serial.available()) {
        
        // read the command from serial
        char cmd = Serial.read();
        armed = (cmd == '1') ? 1 : 0;

        // check the command and control the servos
        if (armed == 0) {

            // set LED indicator to OFF when disarmed
            digitalWrite(LED_BUILTIN, LOW);
            
            // write the position to the servo
            // servo1.write(deg_pos_0);
            // servo2.write(deg_pos_0);
            // servo3.write(deg_pos_0);
            // servo4.write(deg_pos_0);
            servo1.writeMicroseconds(pwm_pos_0);
            servo2.writeMicroseconds(pwm_pos_0);
            servo3.writeMicroseconds(pwm_pos_0);
            servo4.writeMicroseconds(pwm_pos_0);
        } 
        else if (armed == 1) {

            // set LED indicator to ON when armed
            digitalWrite(LED_BUILTIN, HIGH);
            
            // write the position to the servo
            // servo1.write(deg_pos_1);
            // servo2.write(deg_pos_1);
            // servo3.write(deg_pos_1);
            // servo4.write(deg_pos_1);
            servo1.writeMicroseconds(pwm_pos_1);
            servo2.writeMicroseconds(pwm_pos_1);
            servo3.writeMicroseconds(pwm_pos_1);
            servo4.writeMicroseconds(pwm_pos_1);
        }
        else {
            // unknown command, stay at previous state
        }
    }

    // delay to avoid flooding the serial port
    delay(10);
}

