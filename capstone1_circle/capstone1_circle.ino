#include "circle_queue.h"

void setup() {
  Serial.begin(9600);
  while (!Serial);

  CQueue queue;

  // Load test data into the queue.
  for(int i = 0; i < 300; ++i){
    StaticJsonDocument<CELL_SIZE> doc;
    doc["hello"] = i;
    queue.add(doc);
    Serial.println("Object Added");
  }

  // Pop test data from the queue and read it.
  for(int i = 0; i < 258; ++i){
    StaticJsonDocument<CELL_SIZE> doc = queue.pop();
    int num = doc["hello"];
    String hello = String(num);
    Serial.println("Object Popped: doc[\"hello\"]: " + hello);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}