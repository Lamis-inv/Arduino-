#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h>


// === LCD Setup ===
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust 0x27 if your LCD address differs

// === Buttons Setup ===
const int btnUpPin = 2;
const int btnDownPin = 3;
const int btnSelectPin = 4;
const int btnBackPin = 5;
#define buttonA 2  // Rock
#define buttonB 3  // Paper
#define buttonC 4  // Scissors
#define DEBOUNCE_DELAY 50

// === Button states ===
bool btnUpState, btnDownState, btnSelectState, btnBackState;
bool lastBtnUpState = HIGH, lastBtnDownState = HIGH, lastBtnSelectState = HIGH, lastBtnBackState = HIGH;
unsigned long lastDebounceTimeUp = 0, lastDebounceTimeDown = 0, lastDebounceTimeSelect = 0, lastDebounceTimeBack = 0;

// === Menu ===
const char *menuItems[] = {
  "Reaction Timer",
  "Tik Tak Toe",
  "Dice Roller",
  "Quiz Game",
  "Fake Hacker",
  "Simon Says",
  "RPS",
  "Guess Number",
  "Morse Menu"
};
const int menuCount = sizeof(menuItems) / sizeof(menuItems[0]);
int currentMenuIndex = 0;

bool inGame = false;
int currentGame = -1; // -1 means in menu

// === Variables for games ===
// (Will define inside each game function as needed)

// === General helpers ===
void waitForNoButton() {
  // Wait until all buttons released
  while (digitalRead(btnUpPin) == LOW || digitalRead(btnDownPin) == LOW || digitalRead(btnSelectPin) == LOW || digitalRead(btnBackPin) == LOW) {
    delay(10);
  }
}

bool readButton(int pin, bool &lastState, unsigned long &lastDebounceTime) {
  bool reading = digitalRead(pin);
  if (reading != lastState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading == LOW) { // Assuming buttons connect to GND (active LOW)
      lastState = reading;
      return true;
    }
  }
  lastState = reading;
  return false;
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));  // Or use another unused analog pin

  lcd.init();
  lcd.backlight();
  pinMode(buttonA, INPUT_PULLUP);
pinMode(buttonB, INPUT_PULLUP);
pinMode(buttonC, INPUT_PULLUP);

  pinMode(btnUpPin, INPUT_PULLUP);
  pinMode(btnDownPin, INPUT_PULLUP);
  pinMode(btnSelectPin, INPUT_PULLUP);
  pinMode(btnBackPin, INPUT_PULLUP);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Arduino Games!");
  lcd.setCursor(0, 1);
  lcd.print("Press Select");

  // Wait for user to press Select to enter menu
  while (digitalRead(btnSelectPin) == HIGH) {
    delay(10);
  }
  waitForNoButton();

  lcd.clear();
  showMenu();
}

void showMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">"); // pointer
  lcd.print(menuItems[currentMenuIndex]);

  // Show next menu item on second line if exists
  if (currentMenuIndex + 1 < menuCount) {
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.print(menuItems[currentMenuIndex + 1]);
  } else {
    lcd.setCursor(0, 1);
    lcd.print(" ");
  }
}
int selectedGame = 0;

int lastGame = -1;
void loop() {
  if (selectedGame != lastGame) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Select Game:");
    lcd.setCursor(0, 1);
    lcd.print(menuItems[selectedGame]);
    lastGame = selectedGame;
  }
  if (!inGame) {
    // Menu navigation
    if (readButton(btnUpPin, lastBtnUpState, lastDebounceTimeUp)) {
      currentMenuIndex--;
      if (currentMenuIndex < 0) currentMenuIndex = menuCount - 1;
      showMenu();
      delay(150);
    }
    if (readButton(btnDownPin, lastBtnDownState, lastDebounceTimeDown)) {
      currentMenuIndex++;
      if (currentMenuIndex >= menuCount) currentMenuIndex = 0;
      showMenu();
      delay(150);
    }
    if (readButton(btnSelectPin, lastBtnSelectState, lastDebounceTimeSelect)) {
      inGame = true;
      currentGame = currentMenuIndex;
      lcd.clear();
      delay(200);
      runGame(currentGame);
      delay(200);
      showMenu();
      inGame = false;
      currentGame = -1;
    }
  }
}

// === Game dispatcher ===
void runGame(int gameIndex) {
  lcd.clear(); 
  switch (gameIndex) {
    case 0: reactionTimer(); break;
    case 1: ticTacToe(); break;
    case 2: diceRoller(); break;
    case 3: quizGame(); break;
    case 4: fakeHackerMode(); break;
    case 5: simonSaysGame(); break;
    case 6:playRockPaperScissors();break;
    case 7:playGuessTheNumber();break;
    case 8:showMorseMenu();break;
    
    default:
      lcd.print("Game not found");
      delay(1000);
      break;
  }
}

/* -------------- GAMES -------------- */

// 0. Reaction Timer
void reactionTimer() {
  lcd.clear();
  lcd.print("Reaction Timer");
  delay(1000);

  lcd.clear();
  lcd.print("Wait for GO...");

  // Random delay 2-5 sec
  unsigned long waitTime = random(2000, 5000);
  delay(waitTime);

  lcd.clear();
  lcd.print("GO! Press!");

  unsigned long startTime = millis();

  // Wait for Select button press or Back to quit
  while (true) {
    if (readButton(btnSelectPin, lastBtnSelectState, lastDebounceTimeSelect)) {
      unsigned long reaction = millis() - startTime;
      lcd.clear();
      lcd.print("Time:");
      lcd.setCursor(0, 1);
      lcd.print(reaction);
      lcd.print(" ms");
      delay(3000);
      return;
    }
    if (readButton(btnBackPin, lastBtnBackState, lastDebounceTimeBack)) {
      lcd.clear();
      lcd.print("Quit");
      delay(1000);
      return;
    }
  }
}

// 1. ticTacToe (text-based)
// Pins for buttons (adjust as per your setup)


// Tic-Tac-Toe (2 Player) on 16x2 LCD with 4 buttons

char board[2][3];  // 2 rows, 3 columns (fits LCD)
int cursorX = 0, cursorY = 0;
char currentPlayer = 'X';

void ticTacToe() {
  // Initialize board
  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 3; x++) {
      board[y][x] = ' ';
    }
  }
  cursorX = 0;
  cursorY = 0;
  currentPlayer = 'X';

  lcd.clear();
  lcd.print("Tic-Tac-Toe!");
  delay(1000);
  lcd.clear();

  while (true) {
    // Display board
    for (int y = 0; y < 2; y++) {
      lcd.setCursor(0, y);
      String line = "";
      for (int x = 0; x < 3; x++) {
        char c = board[y][x];
        if (cursorX == x && cursorY == y) {
          // Highlight current cursor cell with <> brackets
          line += "<";
          line += (c == ' ' ? ' ' : c);
          line += ">";
        } else {
          line += " ";
          line += (c == ' ' ? ' ' : c);
          line += " ";
        }
      }
      // Pad the line to clear leftover characters
      while (line.length() < 16) line += " ";
      lcd.print(line);
    }

    // Read buttons once per loop
    bool upPressed = readButton(btnUpPin, lastBtnUpState, lastDebounceTimeUp);
    bool downPressed = readButton(btnDownPin, lastBtnDownState, lastDebounceTimeDown);
    bool backPressed = readButton(btnBackPin, lastBtnBackState, lastDebounceTimeBack);
    bool selectPressed = readButton(btnSelectPin, lastBtnSelectState, lastDebounceTimeSelect);

    if (upPressed) {
      cursorY = (cursorY == 0) ? 1 : 0;
      delay(200);
    }
    if (downPressed) {
      cursorY = (cursorY == 1) ? 0 : 1;
      delay(200);
    }
    if (backPressed) {
      cursorX = (cursorX == 0) ? 2 : cursorX - 1;
      delay(200);
    }
    if (selectPressed) {
      // If current cell is empty, place mark
      if (board[cursorY][cursorX] == ' ') {
        board[cursorY][cursorX] = currentPlayer;

        if (checkWin(currentPlayer)) {
          lcd.clear();
          lcd.print("Player ");
          lcd.print(currentPlayer);
          lcd.setCursor(0, 1);
          lcd.print("wins!       ");
          delay(3000);
          return;
        }

        if (checkDraw()) {
          lcd.clear();
          lcd.print("Draw!");
          delay(3000);
          return;
        }

        // Switch player
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
      } else {
        // If occupied, just move cursor right
        cursorX = (cursorX + 1) % 3;
      }
      delay(200);
    }
  }
}

// Win & draw checks unchanged
bool checkWin(char player) {
  for (int y = 0; y < 2; y++) {
    if (board[y][0] == player && board[y][1] == player && board[y][2] == player) return true;
  }
  for (int x = 0; x < 3; x++) {
    if (board[0][x] == player && board[1][x] == player) return true;
  }
  return false;
}

bool checkDraw() {
  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 3; x++) {
      if (board[y][x] == ' ') return false;
    }
  }
  return true;
}

/////////////////////

//3 dice roller

void diceRoller() {
  // Seed random number generator once before the first roll
  static bool seeded = false;
  if (!seeded) {
    randomSeed(analogRead(A0));  // Use a floating analog pin for randomness
    seeded = true;
  }

  lcd.clear();
  lcd.print("Dice Roller");
  lcd.setCursor(0, 1);
  lcd.print("Press Select");

  while (true) {
    if (readButton(btnSelectPin, lastBtnSelectState, lastDebounceTimeSelect)) {
      int roll = random(1, 7);  // 1 to 6 inclusive
      lcd.clear();
      lcd.print("You rolled:");
      lcd.setCursor(0, 1);
      lcd.print(roll);
      
      delay(2000);  // Show result for 2 seconds

      lcd.clear();
      lcd.print("Roll again");
      lcd.setCursor(0, 1);
      lcd.print("or Back");
    }
    
    if (readButton(btnBackPin, lastBtnBackState, lastDebounceTimeBack)) {
      lcd.clear();
      lcd.print("Exiting...");
      delay(1000);
      return;
    }

    delay(50);  // Small delay to debounce and avoid button chatter
  }
}


// Quiz Game
struct QuizQ {
  const char* question;
  const char* options[4];
  int correct; // 0-3
};

QuizQ quizQuestions[] = {
  {"Capital of FR?", {"Paris", "Rome", "Berlin", "Madrid"}, 0},
  {"2+2=?", {"3", "4", "5", "6"}, 1},
  {"Color of sky?", {"Blue", "Red", "Green", "Yellow"}, 0},
  {"Sun rises?", {"East", "West", "North", "South"}, 0},
  {"Water freezes at?", {"0C", "100C", "50C", "-10C"}, 0}
};

const int quizCount = sizeof(quizQuestions) / sizeof(quizQuestions[0]);

void quizGame() {
  int score = 0;

  for (int i = 0; i < quizCount; i++) {
    lcd.clear();
    lcd.print("Q:");
    lcd.print(i + 1);
    lcd.print("/");
    lcd.print(quizCount);
    lcd.setCursor(0, 1);
    lcd.print(quizQuestions[i].question);

    delay(1500);

    int optIndex = 0;
    bool answered = false;

    while (!answered) {
      lcd.clear();
      lcd.print("A:");
      lcd.print(quizQuestions[i].options[optIndex]);
      lcd.setCursor(0, 1);
      lcd.print("Sel=Next B=Exit");

      // Button input handling
      if (readButton(btnUpPin, lastBtnUpState, lastDebounceTimeUp)) {
        optIndex--;
        if (optIndex < 0) optIndex = 3;
        delay(200); // debounce delay
      }
      if (readButton(btnDownPin, lastBtnDownState, lastDebounceTimeDown)) {
        optIndex++;
        if (optIndex > 3) optIndex = 0;
        delay(200);
      }
      if (readButton(btnSelectPin, lastBtnSelectState, lastDebounceTimeSelect)) {
        // Check answer
        lcd.clear();
        if (optIndex == quizQuestions[i].correct) {
          lcd.print("Correct!");
          score++;
        } else {
          lcd.print("Wrong!");
          lcd.setCursor(0, 1);
          lcd.print("Ans:");
          lcd.print(quizQuestions[i].options[quizQuestions[i].correct]);
        }
        delay(1500);
        answered = true; // move to next question
      }
      if (readButton(btnBackPin, lastBtnBackState, lastDebounceTimeBack)) {
        // Quit early and show score
        lcd.clear();
        lcd.print("Quit Quiz");
        delay(1000);
        lcd.clear();
        lcd.print("Score:");
        lcd.print(score);
        delay(2000);
        return;
      }
    }
  }

  // Quiz finished, show final score
  lcd.clear();
  lcd.print("Quiz Over!");
  lcd.setCursor(0, 1);
  lcd.print("Score:");
  lcd.print(score);
  delay(3000);
}


// 4. Fake Hacker Mode
void fakeHackerMode() {
  lcd.clear();
  lcd.print("Hacking...");
  delay(1000);

  // Simulate "hacking" by scrolling random hex digits
  for (int i = 0; i < 30; i++) {
    lcd.clear();
    for (int pos = 0; pos < 16; pos++) {
      char c = "0123456789ABCDEF"[random(0,16)];
      lcd.print(c);
    }
    delay(200);

    if (readButton(btnBackPin, lastBtnBackState, lastDebounceTimeBack)) {
      lcd.clear();
      lcd.print("Abort Hack");
      delay(1000);
      return;
    }
  }
  lcd.clear();
  lcd.print("Hack Complete!");
  delay(1500);
}
const int buzzer = 10; // or whichever pin your buzzer is connected to
const int leds[4] = {6, 7, 8, 9};          // Change pins to match your wiring
const int buttons[4] = {2, 3, 4, 5};   // Or 10, 11, 12, 13 if using digital pins

int led_simonSaid[100]; // Store Simon pattern
int bt_simonSaid[100];  // Store user input

int level = 1;
int stage = 1;
int game_play = 1;
bool lost = false;

// 5. Simon Says (4 LEDs)
// We assume 4 LEDs connected on pins 6,7,8,9
void playBuzzer(int toneIndex) {
  int frequency;
  switch (toneIndex) {
    case 1: frequency = 262; break; // C
    case 2: frequency = 294; break; // D
    case 3: frequency = 330; break; // E
    case 4: frequency = 349; break; // F
    default: frequency = 262; break;
  }
  tone(buzzer, frequency, 200);
}
void simonSaysGame() {
  // Initialize LEDs and buttons
  for (int i = 0; i < 4; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
    pinMode(leds[i], OUTPUT);
  }
  pinMode(buzzer, OUTPUT);

  randomSeed(analogRead(0));
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Simon Says ");
  lcd.setCursor(0, 1);
  lcd.print("Press Red Btn");

  // Wait for red button (button[0])
  while (digitalRead(buttons[0]) == HIGH) {
    if (readButton(btnBackPin, lastBtnBackState, lastDebounceTimeBack)) {
      lcd.clear();
      lcd.print("Exit Simon");
      delay(1000);
      return;
    }
  }

  delay(300); // Debounce
  level = 1;
  stage = 1;
  game_play = 1;
  lost = false;

  while (!lost) {
    switch (stage) {
      case 1: { // Show sequence
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Level: ");
        lcd.print(level);
        lcd.setCursor(0, 1);
        lcd.print("Memorize...");
        delay(1000);

        led_simonSaid[level] = random(0, 4);
        for (int i = 1; i <= level; i++) {
          int ledIndex = led_simonSaid[i];
          digitalWrite(leds[ledIndex], HIGH);
          playBuzzer(ledIndex + 1);
          delay(500);
          digitalWrite(leds[ledIndex], LOW);
          delay(300);
        }
        stage = 2;
        break;
      }

      case 2: { // Player input
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Your Turn");

        game_play = 1;
        while (game_play <= level) {
          for (int i = 0; i < 4; i++) {
            if (digitalRead(buttons[i]) == LOW) {
              bt_simonSaid[game_play] = i;
              digitalWrite(leds[i], HIGH);
              playBuzzer(i + 1);
              delay(300);
              digitalWrite(leds[i], LOW);
              while (digitalRead(buttons[i]) == LOW); // Wait release
              game_play++;
            }
          }
          if (readButton(btnBackPin, lastBtnBackState, lastDebounceTimeBack)) {
            lcd.clear();
            lcd.print("Exit Simon");
            delay(1000);
            return;
          }
        }
        stage = 3;
        break;
      }

      case 3: { // Check answer
        lcd.clear();
        lcd.print("Checking...");
        delay(1000);
        for (int i = 1; i <= level; i++) {
          if (bt_simonSaid[i] != led_simonSaid[i]) {
            lost = true;
            break;
          }
        }
        if (!lost) {
          lcd.clear();
          lcd.print("Correct!");
          delay(1000);
          level++;
          stage = 1;
        }
        break;
      }
    }
  }

  // If lost
  lcd.clear();
  lcd.print("!! You Lost !!");
  for (int i = 0; i < 4; i++) digitalWrite(leds[i], HIGH);
  tone(buzzer, 300);
  delay(2000);
  noTone(buzzer);
  for (int i = 0; i < 4; i++) digitalWrite(leds[i], LOW);
  lcd.clear();
  lcd.print("Thanks 4 Playing");
  delay(2000);
}
/////
//6th game rock paper scissor



void playRockPaperScissors() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("R-P-S Game!");
  delay(1000);

  lcd.clear();
  lcd.print("A:Paper B:Rock");
  lcd.setCursor(0, 1);
  lcd.print("C:Scissors");

  int playerChoice = -1;
  while (playerChoice == -1) {
    if (digitalRead(buttonA) == LOW) {
      playerChoice = 0;
      delay(200);
    } else if (digitalRead(buttonB) == LOW) {
      playerChoice = 1;
      delay(200);
    } else if (digitalRead(buttonC) == LOW) {
      playerChoice = 2;
      delay(200);
    }
  }

  int cpuChoice = random(0, 3);

  const char* options[] = {"Rock", "Paper", "Scissors"};

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("You: ");
  lcd.print(options[playerChoice]);
  lcd.setCursor(0, 1);
  lcd.print("CPU: ");
  lcd.print(options[cpuChoice]);
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  if (playerChoice == cpuChoice) {
    lcd.print("Draw!");
  } else if ((playerChoice == 0 && cpuChoice == 2) ||
             (playerChoice == 1 && cpuChoice == 0) ||
             (playerChoice == 2 && cpuChoice == 1)) {
    lcd.print("You win!");
  } else {
    lcd.print("You lose!");
  }

  delay(3000);
  lcd.clear();
  lcd.print("Back to menu...");
  delay(1000);
}

//7  guess number

void playGuessTheNumber() {
  int minGuess = 1;
  int maxGuess = 99;
  int target = random(minGuess, maxGuess + 1);  // Random number in full range
  int guess = minGuess;
  bool guessed = false;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Guess the Number!");
  delay(1000);
  lcd.clear();

  while (!guessed) {
    // Display current guess
    lcd.setCursor(0, 0);
    lcd.print("Guess: ");
    lcd.print(guess);
    lcd.print("    "); // Clear leftovers

    // Show controls
    lcd.setCursor(0, 1);
    lcd.print("Up/Down=+/- Sel=OK");

    // Increase guess
    if (readButton(btnUpPin, lastBtnUpState, lastDebounceTimeUp)) {
      guess++;
      if (guess > maxGuess) guess = minGuess;
      delay(200);
    }

    // Decrease guess
    if (readButton(btnDownPin, lastBtnDownState, lastDebounceTimeDown)) {
      guess--;
      if (guess < minGuess) guess = maxGuess;
      delay(200);
    }

    // Confirm guess
    if (readButton(btnSelectPin, lastBtnSelectState, lastDebounceTimeSelect)) {
      lcd.clear();
      if (guess == target) {
        lcd.print("Correct!");
        guessed = true;
      } else if (guess < target) {
        lcd.print("Too low!");
      } else {
        lcd.print("Too high!");
      }
      delay(1500);
      lcd.clear();
    }

    // Back to exit early
    if (readButton(btnBackPin, lastBtnBackState, lastDebounceTimeBack)) {
      lcd.clear();
      lcd.print("Game Cancelled");
      delay(1000);
      return;
    }
  }

  delay(2000);
}

//////Morse code train
const char morse_0[] PROGMEM = ".-";    // A
const char morse_1[] PROGMEM = "-...";  // B
const char morse_2[] PROGMEM = "-.-.";  // C
const char morse_3[] PROGMEM = "-..";   // D
const char morse_4[] PROGMEM = ".";     // E
const char morse_5[] PROGMEM = "..-.";  // F
const char morse_6[] PROGMEM = "--.";   // G
const char morse_7[] PROGMEM = "....";  // H
const char morse_8[] PROGMEM = "..";    // I
const char morse_9[] PROGMEM = ".---";  // J
const char morse_10[] PROGMEM = "-.-";  // K
const char morse_11[] PROGMEM = ".-.."; // L
const char morse_12[] PROGMEM = "--";   // M
const char morse_13[] PROGMEM = "-.";   // N
const char morse_14[] PROGMEM = "---";  // O
const char morse_15[] PROGMEM = ".--."; // P
const char morse_16[] PROGMEM = "--.-"; // Q
const char morse_17[] PROGMEM = ".-.";  // R
const char morse_18[] PROGMEM = "...";  // S
const char morse_19[] PROGMEM = "-";    // T
const char morse_20[] PROGMEM = "..-";  // U
const char morse_21[] PROGMEM = "...-"; // V
const char morse_22[] PROGMEM = ".--";  // W
const char morse_23[] PROGMEM = "-..-"; // X
const char morse_24[] PROGMEM = "-.--"; // Y
const char morse_25[] PROGMEM = "--.."; // Z

const char* const morseCodes[] PROGMEM = {
  morse_0, morse_1, morse_2, morse_3, morse_4, morse_5, morse_6, morse_7, morse_8,
  morse_9, morse_10, morse_11, morse_12, morse_13, morse_14, morse_15, morse_16,
  morse_17, morse_18, morse_19, morse_20, morse_21, morse_22, morse_23, morse_24, morse_25
};
void showMorseMenu() {
  int option = 0;  // 0: Trainer, 1: Quiz
  int lastOption = -1;

  while (true) {
    // Only update LCD if selection changed
    if (option != lastOption) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(option == 0 ? "> Trainer" : "  Trainer");
      lcd.setCursor(0, 1);
      lcd.print(option == 1 ? "> Quiz" : "  Quiz");
      lastOption = option;
    }

    // Navigate
    if (readButton(btnUpPin, lastBtnUpState, lastDebounceTimeUp)) {
      option = (option - 1 + 2) % 2;  // wrap around
      delay(200);
    }

    if (readButton(btnDownPin, lastBtnDownState, lastDebounceTimeDown)) {
      option = (option + 1) % 2;
      delay(200);
    }

    // Select
    if (readButton(btnSelectPin, lastBtnSelectState, lastDebounceTimeSelect)) {
      lcd.clear();
      if (option == 0) {
        playMorseTrainer();
      } else {
        playMorseQuiz();
      }
      lastOption = -1; // Force redraw after return
    }

    // Exit
    if (readButton(btnBackPin, lastBtnBackState, lastDebounceTimeBack)) {
      lcd.clear();
      lcd.print(F("Exiting..."));
      delay(500);
      return;
    }
  }
}


// Morse Trainer
void playMorseTrainer() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Morse Trainer"));

  delay(1000);
  lcd.clear();

  while (true) {
    int randIndex = random(0, 26);  // A-Z
    char buffer[10];
    strcpy_P(buffer, (char*)pgm_read_word(&(morseCodes[randIndex])));
    char letter = 'A' + randIndex;

    // Show Morse Code
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Code: "));
    lcd.print(buffer);
    lcd.setCursor(0, 1);
    lcd.print(F("Sel=Show Back=Exit"));

    // Wait for Select or Back
    while (true) {
      if (readButton(btnSelectPin, lastBtnSelectState, lastDebounceTimeSelect)) {
        lcd.clear();
        lcd.print(F("Answer: "));
        lcd.print(letter);
        delay(1500);
        break;
      }

      if (readButton(btnBackPin, lastBtnBackState, lastDebounceTimeBack)) {
        lcd.clear();
        lcd.print(F("Exiting..."));
        delay(1000);
        return;
      }
    }
  }
}
void playMorseQuiz() {
  lcd.clear();
  lcd.print(F("Morse Quiz!"));
  delay(1000);

  while (true) {
    int randIndex = random(0, 26);  // A-Z
    char correctLetter = 'A' + randIndex;

    // Fetch Morse code string from PROGMEM
    char correctCode[10];
    strcpy_P(correctCode, (const char*)pgm_read_ptr(&(morseCodes[randIndex])));

    char userInput[10] = "";
    int userLen = 0;

    // Show letter prompt
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Letter: ");
    lcd.print(correctLetter);
    lcd.setCursor(0, 1);
    lcd.print("Code: ");
    int cursorX = 6;

    while (true) {
      if (readButton(btnUpPin, lastBtnUpState, lastDebounceTimeUp)) {
        if (userLen < 9) {
          userInput[userLen++] = '.';
          userInput[userLen] = '\0';
          lcd.setCursor(cursorX++, 1);
          lcd.print('.');
        }
        delay(200);
      }

      if (readButton(btnDownPin, lastBtnDownState, lastDebounceTimeDown)) {
        if (userLen < 9) {
          userInput[userLen++] = '-';
          userInput[userLen] = '\0';
          lcd.setCursor(cursorX++, 1);
          lcd.print('-');
        }
        delay(200);
      }

      if (readButton(btnSelectPin, lastBtnSelectState, lastDebounceTimeSelect)) {
        lcd.clear();
        if (strcmp(userInput, correctCode) == 0) {
          lcd.print("Correct!");
        } else {
          lcd.print("Wrong!");
          lcd.setCursor(0, 1);
          lcd.print("Ans: ");
          lcd.print(correctCode);
        }
        delay(2000);
        break;
      }

      if (readButton(btnBackPin, lastBtnBackState, lastDebounceTimeBack)) {
        lcd.clear();
        lcd.print("Exiting...");
        delay(1000);
        return;
      }
    }
  }
}
