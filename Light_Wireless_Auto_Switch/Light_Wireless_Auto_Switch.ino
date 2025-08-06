#include <Servo.h>
#include <IRremote.h>

Servo servoMotor;
int ledPin = 8; // Pin for the LED

// Define IR sensor pin
int irSensorPin = 2; // Change this to the appropriate pin connected to your IR sensor

// Define IR code values for 85° and 95° positions
unsigned long code85 = 0x12345678; // Replace with the actual IR code for 85°
unsigned long code95 = 0x87654321; // Replace with the actual IR code for 95°

// Define custom IR patterns for code85 and code95
unsigned long pattern85_1 = 0x1A8C;
unsigned long pattern85_2 = 0x128C;
unsigned long pattern95_1 = 0xA97;
unsigned long pattern95_2 = 0x297;

IRrecv irrecv(irSensorPin);
decode_results results;

void setup() {
  servoMotor.attach(9); // Attach the servo to pin 9

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Initially, turn off the LED

  irrecv.enableIRIn(); // Start the IR receiver
  Serial.begin(9600);
}

void loop() {
  if (irrecv.decode(&results)) {
    unsigned long receivedCode = results.value;

    // Check for custom IR patterns
    if (receivedCode == pattern85_1 || receivedCode == pattern85_2) {
      servoMotor.write(100); // Turn the servo to 85°
      digitalWrite(ledPin, HIGH); // Turn on the LED
    } else if (receivedCode == pattern95_1 || receivedCode == pattern95_2) {
      servoMotor.write(150); // Turn the servo to 95°
      digitalWrite(ledPin, LOW); // Turn off the LED
    }

    irrecv.resume(); // Receive the next value
  }
}
