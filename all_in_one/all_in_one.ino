#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RECEIVER_PIN 11
#define FAN_PIN 9
#define BUZZER_PIN 10
#define RED_PIN 6
#define GREEN_PIN 7
#define BLUE_PIN 8
#include <DHT.h>

#define DHT_PIN 2 // Digital pin connected to DHT sensor
#define DHT_TYPE DHT11 // DHT sensor type

DHT dht(DHT_PIN, DHT_TYPE);

IRrecv irReceiver(RECEIVER_PIN);
decode_results irResults;

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD I2C address and dimensions (change if necessary)

bool fanRunning = false;
bool buzzerRunning = false;
int ledRed = 0;
int ledGreen = 0;
int ledBlue = 0;

void setup() {
irReceiver.enableIRIn();

Serial.begin(9600); // Initialize serial communication
dht.begin(); // Initialize DHT sensor

Serial.begin(9600);
lcd.begin(16, 2); // Initialize the LCD
lcd.backlight();
lcd.print("Weirdo Squid");
delay(500);
pinMode(FAN_PIN, OUTPUT);
pinMode(BUZZER_PIN, OUTPUT);
pinMode(RED_PIN, OUTPUT);
pinMode(GREEN_PIN, OUTPUT);
pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
// Read temperature and humidity from the sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

 // Print temperature and humidity to LCD Dispaly
 
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print("c ");

  lcd.setCursor(9, 1);
  lcd.print("H:");
  lcd.print(humidity);
  lcd.print(" %");


 
  


  
if (irReceiver.decode(&irResults)) {
switch (irResults.value) {
case 0x1A82: // Button '1A82' or '1282'
case 0x1282:
fanRunning = false; // Close the fan
digitalWrite(FAN_PIN, LOW);
break;
case 0x1A81: // Button '1A81' or '1281'
case 0x1281:
fanRunning = true; // Run the fan
digitalWrite(FAN_PIN, HIGH);
break;

case 0xAAB: // Button 'AAB'
case 0x2AB:
setColor(255, 0, 0); // Red
break;
case 0xAAC: // Button 'AAC'
case 0x2AC:
setColor(0, 255, 0); // Green
break;
case 0xAAE: // Button 'AAE'
case 0x2AE:
setColor(0, 0, 255); // Blue
break;
case 0x1280: // Button '1280'
case 0x1A80:
setColor(255, 105, 180); // Pink
break;
case 0xAAD: // Button 'AAD'
case 0x2AD:
setColor(255, 255, 0); // Yellow
break;
case 0x128C: // Button '128C' or '1A8C'
case 0x1A8C:
fanRunning = false; // Close the fan
buzzerRunning = false; // Close the buzzer
noTone(BUZZER_PIN);
digitalWrite(FAN_PIN, LOW);
setColor(0, 0, 0); // Turn off RGB LED
break;

case 0x1A8A: // Button '1A8A' or '128A'
case 0x128A:
setColor(255, 165, 0); // Orange
break;

case 0x1AA3: // Button '1AA3' or '12A3'
case 0x12A3:
setColor(173, 216, 230); // Light blue
break;
}

irReceiver.resume(); // Receive the next value
}

updateLCD(); // Update the LCD display
}

void setColor(int red, int green, int blue) {
analogWrite(RED_PIN, red);
analogWrite(GREEN_PIN, green);
analogWrite(BLUE_PIN, blue);
ledRed = red;
ledGreen = green;
ledBlue = blue;
}

void updateLCD() {
lcd.setCursor(0, 0);
lcd.print("Fan:");
if (fanRunning) {
lcd.print("ON  ");
} else {
lcd.print("OFF ");
}

lcd.setCursor(8, 0);
lcd.print("LED:");
lcd.print(getColorName());
}

String getColorName() {
if (ledRed == 255 && ledGreen == 0 && ledBlue == 0) {
return "Red  ";
} else if (ledRed == 0 && ledGreen == 255 && ledBlue == 0) {
return "Green  ";
} else if (ledRed == 0 && ledGreen == 0 && ledBlue == 255) {
return "Blue";
} else if (ledRed == 255 && ledGreen == 105 && ledBlue == 180) {
return "Pink   ";
} else if (ledRed == 255 && ledGreen == 255 && ledBlue == 0) {
return "Yellow";
} else if (ledRed == 255 && ledGreen == 165 && ledBlue == 0) {
return "Orange  ";
} else if (ledRed == 173 && ledGreen == 216 && ledBlue == 230) {
return "Cyan  ";
} else {
return "OFF     ";
}
}
