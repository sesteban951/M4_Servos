/*

    Simple Arduino sketch to control servos over Serial communication.
    WARNING: do not upload this sketch while actively communicating over Serial.

*/

// standrad arduino inlucdes
#include <Arduino.h>

// standard servo 
#include <Servo.h>

// servo objects
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// sine wave parameters
double sin_value = 0.0; // variable to store the sine value
double frequency = 0.5; // frequency of the sine wave
int pos = 0;            // variable to store the servo position

void setup()
{
    // begin the serial communication over USB
    Serial.begin(9600);

    // attach the servo
    servo1.attach(14); 
    servo2.attach(15); 
    servo3.attach(18); 
    servo4.attach(19); 

    // turn on LED to indicate ON
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
    // calculate the sine value
    sin_value = sin(2 * 3.14159  * frequency * millis() / 1000.0); // calculate sine value based on time
    pos = map(sin_value, -1, 1, 0, 180); // map the sine value to servo position (0 to 180 degrees)

    // write the position to the servo
    servo1.write(pos);
    servo2.write(pos);
    servo3.write(pos);
    servo4.write(pos);

    // check if there is data available on the serial port
    if (Serial.available()) {
        
        // read the command from serial
        char cmd = Serial.read();

        // check the command and control the servos
        if (cmd == '1') {
            digitalWrite(LED_BUILTIN, HIGH);
            // write the position to the servo
            servo1.write(pos);
            servo2.write(pos);
            servo3.write(pos);
            servo4.write(pos);
        } 
        
        else if (cmd == '0') {
            digitalWrite(LED_BUILTIN, LOW);
            servo1.write(0);
            servo2.write(0);
            servo3.write(0);
            servo4.write(0);
        }
    }

    // delay to avoid flooding the serial port
    delay(500);
}

