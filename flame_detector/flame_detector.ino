#include <Wire.h>
#include <LiquidCrystal_I2C.h>


const int flameSensorPin = A0; // Analog pin for flame sensor output
const int buzzerPin = 9;       // Digital pin for the buzzer

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.backlight();
  pinMode(flameSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(1, 0);
  lcd.print("_Weirdo Squid_");
  Serial.begin(9600);
  
  
}

void loop() {
  int flameValue = analogRead(flameSensorPin); 

  if (flameValue > 500) { 
    lcd.setCursor(1, 1); 
    lcd.print("Fire Detected!     ");
    Serial.println("Fire Detected!");
    activateBuzzer(); // Call the function to activate the buzzer
  } else {
    lcd.setCursor(0, 1);
    Serial.println("No Fire Detected");
    lcd.print("     No Fire        ");
    noTone(buzzerPin); // Turn off the buzzer if no flame is detected
  }
  delay(500); // Add a small delay for stability
}

void activateBuzzer() {
  tone(buzzerPin, 1000, 1000); // Sound the buzzer for 1 second (adjust as needed)
}
