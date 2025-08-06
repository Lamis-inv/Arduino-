#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define PIN_LED_GREEN 4
#define PIN_LED_RED  3

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo servoMotor;

byte uidBlue[] = {0x63, 0x07, 0xBB, 0x1D};
byte uidWhite[]  = {0xA2, 0x84, 0x61, 0x21};

void setup() {
  servoMotor.attach(8); 
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    bool isBlue = true;
    bool isWhite = true;

    for (byte i = 0; i < 4; i++) {
      if (mfrc522.uid.uidByte[i] != uidBlue[i]) {
        isBlue = false;
      }

      if (mfrc522.uid.uidByte[i] != uidWhite[i]) {
        isWhite = false;
      }
    }

    if (isBlue) {
      digitalWrite(PIN_LED_GREEN, HIGH);
      delay(1000);  // Adjust the delay as needed
      digitalWrite(PIN_LED_GREEN, LOW);
      servoMotor.write(180); // Turn the servo to 115°
    }

    if (isWhite) {
      digitalWrite(PIN_LED_RED, HIGH);
      delay(1000);  // Adjust the delay as needed
      digitalWrite(PIN_LED_RED, LOW);
      servoMotor.write(110); // Turn the servo to 35°
    }

    mfrc522.PICC_HaltA();
    delay(1000);  // Adjust the delay as needed
  }
}
