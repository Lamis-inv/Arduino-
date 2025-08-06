#include <Servo.h>

// Pin numbers
const int servoPin = 9;        // The pin number for the servo motor
const int potentiometerPin = A0;  // The pin number for the potentiometer

// Servo object
Servo myservo;

void setup() {
  myservo.attach(servoPin);  // Attach servo to the specified pin
  myservo.write(0);  // Set initial position to 0 degrees
  
  pinMode(potentiometerPin, INPUT);  // Set potentiometer pin as input
}

void loop() {
  int potValue = analogRead(potentiometerPin);  // Read potentiometer value (0 to 1023)
  
  // Map the potentiometer value to servo angle (0 to 180 degrees)
  int servoAngle = map(potValue, 0, 1023, 0, 180);
  
  myservo.write(servoAngle);  // Set servo position based on potentiometer value
  
  delay(15);  // Small delay for smoother servo movement
}
