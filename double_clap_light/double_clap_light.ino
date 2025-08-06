#include <Servo.h>

#define Sensor 8

// Define servo motor object
Servo servoMotor;

int clap = 0;
long detection_range_start = 0;
long detection_range = 0;
boolean status_lights = false;

void setup() {
  pinMode(Sensor, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW); // Ensure LED starts off
  
  // Attach servo to pin 9
  servoMotor.attach(9);
}

void loop() {
  int status_sensor = digitalRead(Sensor);

  if (status_sensor == 0) {
    if (clap == 0) {
      detection_range_start = detection_range = millis();
      clap++;
    } else if (clap > 0 && millis() - detection_range >= 50) {
      detection_range = millis();
      clap++;
    }
  }

  if (millis() - detection_range_start >= 400) {
    if (clap == 2) {
      if (!status_lights) {
        status_lights = true;
        digitalWrite(13, HIGH); // Turn on LED on pin 13
        servoMotor.write(140); // Turn servo to 180° when LED is on
      } else if (status_lights) {
        status_lights = false;
        digitalWrite(13, LOW); // Turn off LED on pin 13
        servoMotor.write(40); // Turn servo to 90° when LED is off
      }
    }
    clap = 0;
  }
}
