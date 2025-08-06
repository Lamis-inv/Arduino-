#include <ESP32Servo.h>

Servo myServo;

const int servoPin = 18;
int angle = 90;  // Default angle

void setup() {
  Serial.begin(115200);
  myServo.attach(servoPin);
  myServo.write(angle);
  Serial.println("Enter angle between 0 and 180:");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    int newAngle = input.toInt();

    if (newAngle >= 0 && newAngle <= 180) {
      angle = newAngle;
      myServo.write(angle);
      Serial.print("Moved to: ");
      Serial.println(angle);
    } else {
      Serial.println("Invalid angle. Enter 0-180.");
    }
  }
}
