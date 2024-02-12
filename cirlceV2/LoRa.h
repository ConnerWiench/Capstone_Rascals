#include "wiring.h"
#include "WString.h"
#include "HardwareSerial.h"
#include "Arduino.h"
#include "LoRa_E220.h"

#define FREQUENCY_915

class LoRa{
  private:
    LoRa_E220 *radio;

  public:
    LoRa(HardwareSerial &serial, int aux);
    bool send_message(String msg, int dstH, int dstL, int chan);
    bool check_receive();
    String receive_message();
};

LoRa::LoRa(HardwareSerial &serial, int aux){
  radio = new LoRa_E220(serial, aux);
  radio->begin();
}

bool LoRa::send_message(String msg, int dstH, int dstL, int chan){
  radio->sendFixedMessage(dstH, dstL, chan, msg);
  return true;
}

bool LoRa::check_receive(){
  // If something available
  if (radio->available()>1) {
    return true;
  }
  return false;
}

String LoRa::receive_message(){
  // read the String message
  ResponseContainer rc = radio->receiveMessage();
  return rc.data;
}