
#include "mca.h"
// #include "inttypes.h"
#include "stdio.h"
// #include "stdint.h"

MCA mca(&Serial1);

void setup(){
  Serial.begin(9600);

  Serial.println("MCA Starting...");

  delay(5000);

  Serial.println("MCA Started.");
}

void loop(){
  if(Serial.available()){
    char hold = Serial.read();

    if(hold == 'z'){
      mca.zero_out();
      Serial.println("MCA Zeroed Out");
    }
    else{
      Serial.println("Starting Data Capture");
      uint err = mca.capture();
      Serial.println("Done Capturing Data...");

      if(err == 2){
        Serial.println("Capture Timed Out");
      }

      for(int i = 0; i < mca.captureSize; ++i){
        // Serial.print(i); Serial.print(": "); Serial.println(mca.recentCapture[i]); // Labeled
        Serial.print(mca.recentCapture[i]); Serial.print(',');
      }
      Serial.println("\n Done!");
    }

    Serial.clear();
  }
}
