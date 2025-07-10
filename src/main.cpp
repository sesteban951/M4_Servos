// standard arduino
#include <Arduino.h>

// standard servo 
#include <Servo.h>

// servo object
Servo servo;

// compute the sine wave positions
double sin_value = 0.0; // variable to store the sine value
double frequency = 2.0; // frequency of the sine wave
int pos = 0;            // variable to store the servo position

void setup()
{
    // attach the servo
    servo.attach(23); // attach the servo on pin 9 to the servo object

    // turn on LED to indicate ON
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
    // calculate the sine value
    sin_value = sin(frequency * millis() / 1000.0); // calculate sine value based on time
    pos = map(sin_value, -1, 1, 0, 180); // map the sine value to servo position (0 to 180 degrees)

    // write the position to the servo
    servo.write(pos);

    // wait for a short period to create a smooth motion
    // delay(10);   
}