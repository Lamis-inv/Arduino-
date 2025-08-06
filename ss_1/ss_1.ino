void setup() {
  Serial.begin(9600);

  // Initialize LED pins
  pinMode(2, OUTPUT); // Green LED 
  pinMode(3, OUTPUT); // Red LED
  pinMode(4, OUTPUT); // Yellow LED
  pinMode(5, OUTPUT); // Blue LED

  // Initialize button pins
  pinMode(7, INPUT_PULLUP); // Button 1 Green 
  pinMode(8, INPUT_PULLUP); // Button 2 Red
  pinMode(9, INPUT_PULLUP); // Button 3 Yellow
  pinMode(10, INPUT_PULLUP); // Button 4 Blue
}

void loop() {
  // Read button states
  int buttonState1 = digitalRead(7);
  int buttonState2 = digitalRead(8);
  int buttonState3 = digitalRead(9);
  int buttonState4 = digitalRead(10);

  // Print button states to the serial monitor
  Serial.print("Button 1: ");
  Serial.println(buttonState1 == LOW ? "Pressed" : "Not Pressed");
  Serial.print("Button 2: ");
  Serial.println(buttonState2 == LOW ? "Pressed" : "Not Pressed");
  Serial.print("Button 3: ");
  Serial.println(buttonState3 == LOW ? "Pressed" : "Not Pressed");
  Serial.print("Button 4: ");
  Serial.println(buttonState4 == LOW ? "Pressed" : "Not Pressed");

  // Control LEDs based on button states
  digitalWrite(2, buttonState1 == LOW ? HIGH : LOW); // Green LED
  digitalWrite(3, buttonState2 == LOW ? HIGH : LOW); // Red LED
  digitalWrite(4, buttonState3 == LOW ? HIGH : LOW); // Yellow LED
  digitalWrite(5, buttonState4 == LOW ? HIGH : LOW); // Blue LED

  // Delay to debounce buttons
  delay(100);
}
