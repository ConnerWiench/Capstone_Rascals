#include "wiring.h"
#include <stdint.h>
#include "usb_serial.h"
#include "core_pins.h"

#define CAPTURE_SIZE 4096
#define TIMEOUT_SECONDS 10
#define CHANNEL_SIZE 4

#include "HardwareSerial.h"
#include <stdio.h>

class MCA{
  public:
    uint32_t recentCapture[CAPTURE_SIZE];
    const int captureSize = CAPTURE_SIZE;

    void clear_recent();
    void zero_out();
    uint32_t capture();
    MCA(HardwareSerial *ser);
  private:
    uint32_t startTime;
    HardwareSerial *mSerial;

};

MCA::MCA(HardwareSerial *ser){
  mSerial = ser;
  mSerial->begin(115200);

  clear_recent();
}

// Zeros out the MCA to reset collected data.
void MCA::zero_out(){
  mSerial->write(1); mSerial->write(1); // Get actual zero out bits 
}

// Clears the recentCapture array with all 0's.
void MCA::clear_recent(){
  for(int i = 0; i < captureSize; ++i){
    recentCapture[i] = 0;
  }
}

uint32_t MCA::capture(){
  uint timeout = millis() + (1000 * TIMEOUT_SECONDS); // Sets timeout if not all/any data can be read from the MCA.

  while(mSerial->available()){mSerial->read();} // Clear out any pre-existing data in the buffer
  mSerial->write((int)0); mSerial->write((int)16); // Send data request to MCA
  while((!mSerial->available()) && (timeout > millis())){} // Wait for a response with timeout

  int hold = 0;
  uint32_t buffer = 0;
  
  for(int i = 0; (i < captureSize * CHANNEL_SIZE) && (timeout > millis()); ++i){ // Reads out the serial data against a count instead of if Serial is available.  Also has timeout
    hold = mSerial->read();
    if(hold <= -1){
      --i;
      continue;
    }

    buffer = (buffer << 8) | hold; // Bitwise shift buffer 8 bits at a read.
    if(!(i % 4)){ // When 32 bits have been read, save result and clear buffer.
      recentCapture[i/4] = buffer;
      buffer = 0;
    }
  }

  if(millis() >= timeout){
    Serial.println("Capture Timed out...");
    return 2;  // Return error value if timed out.
  }

  return 0;
}