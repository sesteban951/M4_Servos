
// #include <Arduino.h>
// #include <Servo.h>

// // define the relay pins
// #define RELAY_PIN 8

// // servo pin
// #define SERVO_PIN 15

// //create servo object
// Servo servo;

// // default position for the servo
// double deg_pos = 90;

// // some useful global vairables
// // volatile byte relay_state = LOW; // current state of the relay

// // the setup function runs once when you press reset or power the board
// void setup() {

//     //turn on built in LED to indicate that the program is running
//     pinMode(LED_BUILTIN, OUTPUT);

//     // write the position of the zervo
//     servo.write(deg_pos);
    
//     // pin mode for the relay pin
//     pinMode(RELAY_PIN, OUTPUT);

//     // initialize the relay state
//     digitalWrite(RELAY_PIN, HIGH); // initialize the relay state
// }

// // the loop function runs over and over again forever
// void loop() {
  
//     digitalWrite(RELAY_PIN, LOW); // turn the relay ON
//     digitalWrite(LED_BUILTIN, LOW);
//     delay(5000); // wait for 500 milliseconds
                                                                          
//     digitalWrite(RELAY_PIN, HIGH); // turn the relay OFF
//     digitalWrite(LED_BUILTIN, HIGH);
//     servo.write(deg_pos);
//     delay(5000); // wait for 500 milliseconds
// }
