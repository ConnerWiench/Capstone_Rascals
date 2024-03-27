/*
* @author Irradiated Rascals
*
* @date 03-26-2024
*/

#include "./LORA/LoRa.h"
#include "./MCA/mca.h"
// #include "./NANOPB/*"
// #include "./SLIP/SLIP.h"  Decorator breaks other libraries, may just implement on own.

#define RS422_SERIAL Serial5
#define PLAIN_SERIAL Serial6

LoRa radio(Serial3, 9);     // Define global LoRa object
MCA mca(&Serial1);          // Define global Scintillator/MCA object
// SLIPStream rs422(Serial7);  // Define Serial interface for RS-422
// SLIPStream redun(Serial6);  // Define redundacnt Serial link

void setup() {
  Serial.begin(9600);

  RS422_SERIAL.begin(9600);
  PLAIN_SERIAL.begin(9600);
}

void loop() {
  static byte action = 0;
  static ulong mcaRuntime = 0;

  if(RS422_SERIAL.available()){
    byte opcode = 0;
    opcode = RS422_SERIAL.read();

    switch(opcode){ // Respond to a check if we're alive.

      case 1: // Response to Hello message.
        Serial.println("Received Hello Message");
        RS422_SERIAL.write(2);
        return;

      case 3: // Zero's scintillator and sets the next read by time.
      { // Need braces to seperate scope for timeLength.  Else causes error.
        if(!RS422_SERIAL.available()){
          Serial.print("Error: Expected Additional Data");
          return;
        }
        byte timeLength = RS422_SERIAL.read();
        mcaRuntime = millis() + (timeLength * 1000 * 60); // Set runtime by 'timeLength' minutes.
        Serial.printf("Received Timed MCA Instruction, Length=%d\n", timeLength);
        mca.zero_out();
        return;
      }

      case 5: // Send current MCA data.
        for(int i = 0; i < mca.captureSize; ++i){
          RS422_SERIAL.write(mca.recentCapture[i]);
        }
        RS422_SERIAL.write(254);
        return;

      case 7: // Zero out MCA without auto-read.
        Serial.println("Instant Zero Out MCA");
        mca.zero_out();
        RS422_SERIAL.write(8);
        return;

      case 9: // Pull more data from MCA without zero-ing it out.
        Serial.println("Instantly pulling data from MCA");
        mca.capture();
        RS422_SERIAL.write(10);
        return;
    }
  }

  if(mcaRuntime && millis() > mcaRuntime){
    mcaRuntime = 0;
    mca.capture();
    RS422_SERIAL.write(4);  // Send interface code for MCA ready.
  }

}
