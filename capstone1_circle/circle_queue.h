#include <ArduinoJson.h>

#define ARR_SIZE 256 // How many array segments are created.  This controls how many objects can be held at once.
#define CELL_SIZE 256 // The size of each Json Document object in bytes.  This of each key-value pair as a single string for size estimate.

class CQueue{
  private:
    StaticJsonDocument<CELL_SIZE> arr[ARR_SIZE];  // Array of statically sized (on stack) json documents.
    int cursor; // Stores the current position in the queue. 

    // I Probably didn't need seperate functions for these but ¯\_(ツ)_/¯
    void foward();  // Moves cursor fowards.
    void backward();  // Moves cursor backwards.

  public:

    CQueue();
    void add(StaticJsonDocument<CELL_SIZE>);  // Takes Json Document by value and adds to the queue.
    StaticJsonDocument<CELL_SIZE> pop();  // Returns a Json Document by value and clears it from the queue.
};

CQueue::CQueue(){
  cursor = 0;
}

// Moves cursor fowards.
void CQueue::foward(){
  if(cursor < (ARR_SIZE - 1)){
    cursor += 1;
  }
  else{
    cursor = 0;
  }
}

// Moves cursor backwards.
void CQueue::backward(){
  if(cursor >= 1){
    cursor -= 1;
  }
  else{
    cursor = (ARR_SIZE - 1);
  }
}

// Takes Json Document by value and adds to the queue.
void CQueue::add(StaticJsonDocument<CELL_SIZE> doc){
  foward();

  arr[cursor] = doc;
}

// Returns a Json Document by value and clears it from the queue.
StaticJsonDocument<CELL_SIZE> CQueue::pop(){
  StaticJsonDocument<CELL_SIZE> doc = arr[cursor];
  
  if(!arr[cursor].isNull()){
    arr[cursor].clear();
  }
  backward();
  
  return doc;
}