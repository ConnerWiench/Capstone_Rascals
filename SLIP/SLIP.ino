#include "SLIP.h"

SLIPStream testSend(Serial3);
SLIPStream testRec(Serial5);

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
  Serial5.begin(9600);

  // size_t bufSize = test.availableForWrite();

  // Serial.println(bufSize);
}

void loop() {
  if(Serial.available()){
    int buffer = Serial.read();

    if(buffer != '\n'){
      testSend.printf("%c", buffer);
    }
    else {
      testSend.writeEnd();
      testSend.flush();
    }
  }

  if(testRec.available()){
    char buffer = testRec.read();

    if(buffer != 254){
      Serial.printf("%c", buffer);
    }
    else {
      Serial.println("END");
    }
  }
}
