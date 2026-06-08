/*
    Simple Arduino sketch to control servos over Serial communication.
    WARNING: 1) Do not upload this sketch while actively communicating over Serial.
             2) Make sure to set the correct serial port in the parent program.
             3) You cannot open the Serial Monitor in Arduino IDE while this sketch is running
                since it uses the same serial bus.
             4) Use 'sudo' for serial port access on Linux.

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
#define SERVO4_PIN 19    // BACKUP SERVO
#define BAUD_RATE 9600

// use the SERVO4 BACKUP, 0 = off, 1 = on.
#define USE_BACKUP_SERVO4 0

// servo objects
Servo servo1;
Servo servo2;
Servo servo3;
#if USE_BACKUP_SERVO4
    Servo servo4;
#endif

// LOCKED and OPEN status LED pins
// WARNING: Do not change pins this as they are permanently soldered.
#define LED_BLUE_PIN 23
#define LED_RED_PIN 22

// command variable
int command_curr = 0;         // current command received

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
    #if USE_BACKUP_SERVO4
        servo4.attach(SERVO4_PIN);    // BACKUP
    #endif

    // builtinLED to indicate status
    pinMode(LED_BLUE_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);

    // servos go to open by default
    servo1.write(deg_pos_open_servo1);
    servo2.write(deg_pos_open_servo2);
    servo3.write(deg_pos_open_servo3);
    #if USE_BACKUP_SERVO4
        servo4.write(deg_pos_open_servo4);
    #endif

    // turn on the Teensy LED to make sure Teensy is running
    digitalWrite(LED_BLUE_PIN, HIGH);  // BLUE LED for OPEN
    digitalWrite(LED_RED_PIN, LOW);    // RED LED for LOCKED
    digitalWrite(LED_BUILTIN, HIGH);   // AMBER LED for Teensy is on status
}


void loop()
{
    // check if there is data available on the serial port
    if (Serial.available()) 
    {

        // read the command from serial
        char cmd = Serial.read();

        // set the current command
        if (cmd == '0') 
        {
            command_curr = 0;  // unarm command
            digitalWrite(LED_BLUE_PIN, HIGH);
            digitalWrite(LED_RED_PIN, LOW);
        }
        else if (cmd == '1') {
            command_curr = 1;  // arm command
            digitalWrite(LED_RED_PIN, HIGH);
            digitalWrite(LED_BLUE_PIN, LOW);
        }
        else {
            // bad, detach the servos.
            // TODO: this is not working
            command_curr = -1;
            servo1.detach();
            servo2.detach();
            servo3.detach();
            #if USE_BACKUP_SERVO4
                        servo4.detach();
            #endif
        }
    }

    // drive servos straight to the commanded position
    if (command_curr == 0) {
        servo1.write(deg_pos_open_servo1);
        servo2.write(deg_pos_open_servo2);
        servo3.write(deg_pos_open_servo3);
        #if USE_BACKUP_SERVO4
                servo4.write(deg_pos_open_servo4);
        #endif
    }
    else if (command_curr == 1) {
        servo1.write(deg_pos_lock_servo1);
        servo2.write(deg_pos_lock_servo2);
        servo3.write(deg_pos_lock_servo3);
        #if USE_BACKUP_SERVO4
                servo4.write(deg_pos_lock_servo4);
        #endif
    }

    // delay to avoid flooding the serial port
    delay(10);
}

