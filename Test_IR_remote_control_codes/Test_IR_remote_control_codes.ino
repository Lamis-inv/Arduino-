#include <IRremote.h>

int irReceiverPin = 2; // Pin for the IR receiver module
IRrecv irrecv(irReceiverPin);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the IR receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX); // Print the received IR code in hexadecimal
    irrecv.resume(); // Receive the next value
  }
}
