
#include "mca.h"
// #include "inttypes.h"
#include "stdio.h"
// #include "stdint.h"

MCA mca(&Serial1);

void setup(){
  Serial.begin(9600);

  Serial.println("MCA Started waiting...");

  // delay(10000);

  Serial.println("Done Waiting, Capturing.");
  uint err = mca.capture();
  Serial.println("Done Capturing Data...");

  if(err == 2){
    Serial.println("Capture Timed Out");
    exit(-1);
  }

  for(int i = 0; i < 4096; ++i){
    // Serial.print(i); Serial.print(": "); Serial.println(mca.recentCapture[i]); // Labeled
    Serial.print(mca.recentCapture[i]); Serial.print(',');
  }
  Serial.println("\n Done!");
}

void loop(){

}
// #define ARR_SIZE 4096

// void setup() {
//   Serial.begin(9600);
//   Serial1.begin(115200);

//   Serial.println("Setup Complete");
// }

// void loop() {
//   static String input = "";
//   static uint32_t output[ARR_SIZE];
//   static uint index = 0;

//   if(Serial.available()){
//     while(Serial.available()){
//       input += Serial.read();
//     }
//     Serial.print("Input: "); Serial.println(input);
//     Serial1.write(0); Serial1.write(16); // Serial1.print(input); 

//     input = "";

//     for(int i = 0; i < ARR_SIZE; ++i){
//       output[i] = 0;
//     }
//   }

//   if(Serial1.available()){
//     Serial.println("Receiving MCA Data");

//     output[index] = Serial1.read();
//     index++;

//     Serial.println("Output: ");
//     for(int i = 0; i < ARR_SIZE; ++i){
//       Serial.print(output[i]); Serial.print(',');
//     }
//     Serial.println("\n");

//     Serial.println("Read All MCA Data");
//   }
// }