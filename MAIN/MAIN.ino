/*
* @author Irradiated Rascals
*
* @date 03-26-2024
*/

#include "./LORA/LoRa.h"
#include "./MCA/mca.h"
// #include "./NANOPB/*"
// #include "./SLIP/SLIP.h"  Decorator breaks other libraries, may just implement on own.

#include <stdio.h>
#include "./NANOPB/pb_encode.h"
#include "./NANOPB/pb_decode.h"
#include "./NANOPB/rad.pb.h"
#include "./NANOPB/radQueue.h"

#define RS422_SERIAL Serial5
#define PLAIN_SERIAL Serial6
#define LORA_BYTE_MAX 128

LoRa radio(Serial3, 9, 15, 5, 5);     // Define global LoRa object
MCA mca(&Serial1);          // Define global Scintillator/MCA object
// SLIPStream rs422(Serial7);  // Define Serial interface for RS-422
// SLIPStream redun(Serial6);  // Define redundacnt Serial link

bool redundant_available();
byte redundant_read();
void redundant_write(byte);
// void redundant_send_data(Data);

void setup() {
  Serial.begin(9600);

  RS422_SERIAL.begin(9600);
  PLAIN_SERIAL.begin(9600);
}

void loop() {
  // static byte action = 0;
  static ulong mcaRuntime = 0;
  static char loraPayload[LORA_BYTE_MAX];

  if(redundant_available()){
    byte opcode = 0;
    opcode = redundant_read();

    switch(opcode){ // Respond to a check if we're alive.

      case 1: // Response to Hello message.
        Serial.println("Received Hello Message");
        redundant_write(2);
        return;

      case 3: // Zero's scintillator and sets the next read by time.
      { // Need braces to seperate scope for timeLength.  Else causes error.
        if(!redundant_available()){
          Serial.print("Error: Expected Additional Data");
          return;
        }
        byte timeLength = redundant_read();
        mcaRuntime = millis() + (timeLength * 1000 * 60); // Set runtime by 'timeLength' minutes.
        Serial.printf("Received Timed MCA Instruction, Length=%d\n", timeLength);
        mca.zero_out();
        return;
      }

      case 5: // Send current MCA data.
        for(int i = 0; i < mca.captureSize; ++i){
          redundant_write(mca.recentCapture[i]); // Change to NanoPB
        }
        redundant_write(254);
        return;

      case 7: // Zero out MCA without auto-read.
        Serial.println("Instant Zero Out MCA");
        mca.zero_out();
        redundant_write(8);
        return;

      case 9: // Pull more data from MCA without zero-ing it out.
      {
        Serial.println("Instantly pulling data from MCA");
        uint32_t status = mca.capture();
        if(!status){
          redundant_write(-10);
        }
        redundant_write(10);
        return;
      }

      case 11: // Load LoRa payload
        for(int i = 0; (redundant_available()) && (i < LORA_BYTE_MAX - 1); ++i){
          loraPayload[i] = redundant_read();
        }
        loraPayload[LORA_BYTE_MAX - 1] = '\0';

      case 15: // Send LoRa payload
        radio.send_message(loraPayload);
    }
  }

  if(mcaRuntime && millis() > mcaRuntime){  // Auto-read MCA from time check.
    mcaRuntime = 0;
    mca.capture();
    RS422_SERIAL.write(4);  // Send interface code for MCA ready.
  }

}


bool redundant_available(){
  return RS422_SERIAL.available() || PLAIN_SERIAL.available();
}

byte redundant_read(){
  if(RS422_SERIAL.available()){
    return RS422_SERIAL.read();
  }
  if(PLAIN_SERIAL.available()){
    return PLAIN_SERIAL.read();
  }
  return (byte)-1;
}

void redundant_write(byte payload){
  RS422_SERIAL.write(payload);
  PLAIN_SERIAL.write(payload);
}