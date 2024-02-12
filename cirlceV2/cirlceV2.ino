#include <ArduinoJson.h>

#include "circle_queueV2.h"
#include "LoRa.h"

#define DEST_ADDL 2
#define DEST_ADDH 0

LoRa radio(Serial5, 19);
CQueue list();

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  Serial.println("The Serial is connected to the computer...");

  Serial.println("Radio Config:");
  radio.print_config();

  radio.send_message("Hello World", DEST_ADDH, DEST_ADDL, 23);
  
  StaticJsonDocument<CELL_SIZE> doc;
  for(int i = 0; i < 10; i++){
    
  }
}

void loop(){
  if(radio.check_receive()){
    String msg = radio.receive_message();
    Serial.println(msg);
  }

  

  if (Serial.available()) {
		String input = Serial.readString();
    Serial.print("Sending... "); Serial.println(input);
		radio.send_message(input, DEST_ADDH, DEST_ADDL, 23);
  }
}
//   // put your main code here, to run repeatedly:
  
//   static CQueue queue;

//   StaticJsonDocument<CELL_SIZE> incommingData;
//   if(Serial.available()){
//     incomingData["test"] = Serial.read();
//     queue.enqueue(incommingData);
//     Serial.println("Object Added");
//   }

//   if(!incommingData.isNull()){
//     incommingData = queue.dequeue();
//     int num = incommingData["test"];
//     String test = String(num);
//     Serial.println("Object Popped: incommingData[\"test\"]: " + test);
//   }

//   char str[256];
//   int iterator = 0;
//   static unsigned long int from = 0;

//   if (Serial1.available() > 0) {
//     while (Serial1.available() > 0) {
//       str[iterator] = Serial1.read();
//       iterator++;
//       delay(10);
//     }
//     str[iterator] = '\0';
//     Serial.print("Serial1: ");
//     Serial.println(str);
//     iterator = 0;
//   }

//   if ((millis() - from) > 5000) {
//     Serial.println("Sending msg...");
//     Serial1.println("Hello World");
//     Serial1.flush();
//     from = millis();
//   }
// }

// void test() {
//   // Load test data into the queue.
//   static CQueue queue1;
//   StaticJsonDocument<CELL_SIZE> testData;

//   for ( int i = 0; i < 256; i++){
//     testData["testing_full_size"] = i;
//     queue1.enqueue(testData);
//     Serial.println("Object Added(number): " + i);
//   }

//   for (int i = 0; i < 256; i++){
//     testData = queue1.dequeue();
//     int num = testData["testing_full_size"];
//     String testing = String(num);
//     Serial.println("Object dequeued: testData[\"testing_full_size\"]: " + testing);
//   }
// }

// void test2(){

//   static CQueue queue2;

//   StaticJsonDocument<CELL_SIZE> testData;

//   testData["test"] = 1;
//   queue2.enqueue(testData);
//   testData["test"] = 2;
//   queue2.enqueue(testData);

//   testData = queue2.dequeue();
//   int num = testData["test"];
//   String testing = String(num);
//   Serial.println("Object dequeued: testData: " + testing);

//   testData = queue2.dequeue();
//   num = testData["test"];
//   testing = String(num);
//   Serial.println("Object dequeued: testData: " + testing);
// }
