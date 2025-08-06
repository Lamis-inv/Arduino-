const int kPinBtn = 7; // Push Button - Pin 7
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcdekranim(0x27, 16, 2);

byte patlama[] = {
  B00000,
  B00000,
  B10101,
  B01110,
  B01110,
  B10101,
  B00000,
  B00000
};

byte s1[] = {
  B11110,
  B10010,
  B11110,
  B11110,
  B11110,
  B10010,
  B10011,
  B11000
};

byte s2[] = {
  B11110,
  B10010,
  B11110,
  B11110,
  B11110,
  B10010,
  B11010,
  B00011
};

byte c1[] = {
  B00000,
  B00000,
  B00110,
  B00110,
  B10110,
  B11110,
  B00110,
  B00110
};

bool spriteBool = false;
float jump = 0;
int cactiRng = 40; // Increased the delay to slow down cacti spawning
int cacti[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int skor = 0;
int hskor = 0;
bool isDed = false;
bool d1 = false;
int db = 0;

void setup() {
  Serial.begin(9600);
  pinMode(kPinBtn, INPUT_PULLUP); // Push Button as INPUT with pull-up
  lcdekranim.init();
  lcdekranim.createChar(0, s1);
  lcdekranim.createChar(1, s2);
  lcdekranim.createChar(2, c1);
  lcdekranim.createChar(3, patlama);
  lcdekranim.backlight();
  lcdekranim.setCursor(0, 0);
}

void loop() {
  if (isDed == false) {
    delay(100); // Decreased the delay to speed up the game

    cactiRng -= random(4, 12);
    if (cactiRng <= 0) {
      db--;
      cactiRng = 60; // Increased the delay to slow down cacti spawning
      if (skor > 250) {
        cactiRng = 50; // Increased the delay for slower spawning
      }
      if (skor > 500) {
        cactiRng = 40; // Increased the delay for even slower spawning
      }
      if (skor > 750) {
        cactiRng = 30; // Increased the delay for even slower spawning
      }
      cacti[15] = 1;
      if (random(0, 6) == 3 && db <= 0) {
        cactiRng = 0;
        db = 2;
      }
    }
    for (int i = 0; i < 15; i++) {
      if (i == 15) {
        cacti[i] = 0;
      } else {
        cacti[i] = cacti[i + 1];
        cacti[i + 1] = 0;
      }
    }
    for (int i = 0; i < 15; i++) {
      lcdekranim.setCursor(i, 1);
      if (cacti[i] == 1) {
        lcdekranim.write(2);
      } else {
        if (!(jump == 0 && i == 1)) {
          lcdekranim.print(" ");
        }
      }
    }
    if (digitalRead(kPinBtn) == LOW && jump <= 0) {
      jump = 2;
    }
    if (jump > 0) {
      jump -= 0.5;
    }
    if (jump > 0) {
      lcdekranim.setCursor(1, 1);
      if (cacti[1] == 0) {
        lcdekranim.print(" ");
      }
      lcdekranim.setCursor(1, 0);
    } else {
      lcdekranim.setCursor(1, 0);
      lcdekranim.print(" ");
      lcdekranim.setCursor(1, 1);
    }
    if (spriteBool == true) {
      lcdekranim.write(0);
      spriteBool = false;
    } else {
      lcdekranim.write(1);
      spriteBool = true;
    }
    if (cacti[1] && jump == 0) {
      lcdekranim.setCursor(1, 1);
      lcdekranim.write(3);
      delay(2000);
      isDed = true;
      lcdekranim.clear();
      Serial.print("you died");
    } else {
      skor += 5;
      lcdekranim.setCursor(12, 0);
      lcdekranim.print(skor);
    }
  } else {
    if (d1 == false) {
      lcdekranim.setCursor(0, 0);
      lcdekranim.print("Score " + String(skor));
      if (skor > hskor) {
        hskor = skor;
      }
      lcdekranim.setCursor(0, 1);
      lcdekranim.print("HighScore " + String(hskor));
      d1 = true;
    }
    if (digitalRead(kPinBtn) == LOW) {
      d1 = false;
      isDed = false;
      skor = 0;
      for (int i = 0; i < 15; i++) {
        cacti[i] = 0;
      }
      cactiRng = 60;
      lcdekranim.clear();
    }
  }
}
