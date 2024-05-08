#include "pins_arduino.h"
#include <cstddef>
#include "wiring.h"
#include <stdint.h>
#include "usb_serial.h"
#include "core_pins.h"

// How many unsigned integers are expected from an MCA capture.
#define CAPTURE_SIZE 4096
// How much time is given to capture.  Will end the function if it hangs for longer than this time.
#define TIMEOUT_SECONDS 10
// How many bits are being stored in every recentCapture index. 
// 32 bit integers at 8 bit transfers take 4 data units.
#define CHANNEL_SIZE 4

#include "HardwareSerial.h"
#include <stdio.h>

class MCA{
  public:
    uint32_t recentCapture[CAPTURE_SIZE];
    const int captureSize = CAPTURE_SIZE;

    MCA(HardwareSerial *ser);r
    void zero_out();
    bool capture();
  private:
    void clear_recent();
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

// Performs a 4096 byte capture from the MCA.
bool MCA::capture(){
  int hold = 0;   // Holds the immediate read output (8 bits max)
  uint32_t buffer = 0;    // Combines holds until it reads 32 bits of data.
  size_t retries = 0;   // Stores the number of times the serial attempts, but can't read the MCA.
  uint bitOrder[4] = {8,16,24,0};   // Holds the bit shift values for the buffer based on octet receive order.
  uint timeout = millis() + (1000 * TIMEOUT_SECONDS); // Sets timeout if not all/any data can be read from the MCA.

  while(mSerial->available()){mSerial->read();} // Clear out any pre-existing data in the buffer
  mSerial->write((int)0); mSerial->write((int)16); // Send data request to MCA
  while((!mSerial->available()) && (timeout > millis())){} // Wait for a response with timeout

  // Reads out the serial data against a count instead of if Serial is available.  Also has timeout
  for(int i = 0; (i < captureSize * CHANNEL_SIZE) && (timeout > millis()); ++i){ 
    if(!mSerial->available()){
      retries += 1;
      --i;
      continue;
    }

    hold = mSerial->read();

    // Get what octet we're on.
    short octet = i % 4;
    // Combine the read into the overall buffer.
    // The bit shift must reference the order table because MCA maniacs like doing things out of order.
    buffer = buffer | (hold << bitOrder[octet]);

    // When 32 bits have been read, save result and clear buffer.
    if(!(octet)){ 
      recentCapture[i/4] = buffer;
      buffer = 0;
    }
  }
  Serial.printf("Retries: %u\n", retries); // How many times there was no data ready for a read.
  if(millis() >= timeout){
    Serial.println("Capture Timed out...");
    return 2;  // Return error value if timed out.
  }

  return 1;
}