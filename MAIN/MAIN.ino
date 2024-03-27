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

    switch (opcode) {  // Respond to a check if we're alive.
      case 1:
        RS422_SERIAL.write(2);
        return;
      case 3:
        if(!RS422_SERIAL.available()){
          Serial.print("Expected Additional Data");
          return;
        }
        byte timeLength = RS422_SERIAL.read();
        mcaRuntime = millis() + (timeLength * 1000 * 60); // Set runtime by 'timeLength' minutes.
        mca.zero_out();
        return;
    }
  }

  if(mcaRuntime && millis() > mcaRuntime){
    mcaRuntime = millis() + (1 * 1000 * 60);  // Create 1 minute timeout for waiting.
    RS422_SERIAL.write(4);  // Send interface code for MCA ready.

    while(!(RS422_SERIAL.available()) && (millis() < mcaRuntime)){} // Wait for a response message to come in.
    byte response = RS422_SERIAL.read();

    if(response == 5){
      mca.capture();
      for(int i = 0; i < CAPTURE_SIZE; ++i){
        RS422_SERIAL.write(mca.recentCapture[i]);
      }
      RS422_SERIAL.write(-2);
    }
    else{
      RS422_SERIAL.write(-3);
    }
  }

}
