#include "circle_queue.h"

#define HWSERIAL Serial2

void setup() {
  Serial.begin(9600);
  HWSERIAL.begin(9600);

  Serial.println("Sending Message");
  String testMsg = "Hello World";
  HWSERIAL.println("Hello World");
  Serial.println("Checking for Messages");
}

void loop() {
  int incomingByte;

  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.print("USB received: ");
    Serial.println(incomingByte, DEC);
    HWSERIAL.print("USB received:");
    HWSERIAL.println(incomingByte, DEC);
  }
  if (HWSERIAL.available() > 0) {
    incomingByte = HWSERIAL.read();
    Serial.print("UART received: ");
    Serial.println(incomingByte, DEC);
    HWSERIAL.print("UART received:");
    HWSERIAL.println(incomingByte, DEC);
  }
}
