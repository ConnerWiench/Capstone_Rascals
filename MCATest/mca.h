#include "usb_serial.h"
#include "core_pins.h"

#define CAPTURE_SIZE 4096
#define TIMEOUT_SECONDS 30

#include "HardwareSerial.h"
#include <stdio.h>

class MCA{
  public:
    uint32_t recentCapture[CAPTURE_SIZE];

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

  // zero_out();
  clear_recent();
}

// Zeros out the MCA to reset collected data.
void MCA::zero_out(){
  mSerial->write(1); mSerial->write(1); // Get actual zero out bits 
}

void MCA::clear_recent(){
  for(int i = 0; i < CAPTURE_SIZE; ++i){
    recentCapture[i] = 0;
  }
}

uint32_t MCA::capture(){
  uint timeout = millis() + (1000 * TIMEOUT_SECONDS); // Sets timeout if not all/any data can be gotten from MCA

  mSerial->write((int)0); mSerial->write(16); // Send data request to MCA
  while((!mSerial->available()) && (timeout > millis())){} // Wait for a response with timeout

  int hold = 0;
  for(int i = 0; (i < CAPTURE_SIZE) && (timeout > millis()); ++i){
    hold = mSerial->read();
    if(hold <= -1){
      --i;
      continue;
    }
    recentCapture[i] = hold;
  }
  if(millis() >= timeout){
    Serial.println("Capture Timed out...");
    return 2;
  }

  return 1;
}