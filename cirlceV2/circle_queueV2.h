#include <ArduinoJson.h>

#define ARR_SIZE 256 // How many array segments are allowed at once. This defines the objects in the queue
#define CELL_SIZE 196 // The size of each Json Document object in bytes.  This of each key-value pair as a single string for size estimate.

class CQueue{
    private:
        StaticJsonDocument<CELL_SIZE> arr[ARR_SIZE];        // Array of statically sized json documents.
        int front;                                          // Current position of where the oldest entry in the queue is.
        int back;                                           // Most recent object in the array (Tells us where to enqueue)
        void inc(int num);                                  // incriment the number by 1
    public:
        CQueue();
        void enqueue(StaticJsonDocument<CELL_SIZE>); // Adds the Json Document tot he queue by value 
        StaticJsonDocument<CELL_SIZE> dequeue();     // Returns the oldest queue entry that is availible
        bool isEmpty();                              // Checks to see if the Queue is empty
        bool isFull();                               // CHecks to see if the Queue is full
};

CQueue::CQueue(){
    front = -1;
    back = -1;
}

// Checks to see if the Queue is Empty
bool CQueue::isEmpty(){
  return (front == -1);
}

// Checks to see if the Queue is Full
bool CQueue::isFull(){
  return (front == 0 && back == ARR_SIZE - 1) || (front == back + 1);
}

// Takes Json Document by value and adds to the queue
void CQueue::enqueue(StaticJsonDocument<CELL_SIZE> doc){
  //if the queue is full then we need to move it
  if(isFull()){
    if(back == ARR_SIZE - 1){
      back = 0;
    }else{
      back += 1;
    }

    if(front == ARR_SIZE - 1){
      front = 0;
    }else{
      front += 1;
    }
  }

  // Check to see if it is empty, if it is, then initialize...
  if(isEmpty()){
    front = 0;
    back = 0;
  }else{
    back += 1;
  }

  arr[back] = doc;
}

//Returns a Json Document by value and clears it from the queue
StaticJsonDocument<CELL_SIZE> CQueue::dequeue(){
  //get the object at the beginning of the queue
  StaticJsonDocument<CELL_SIZE> doc = arr[front];
  
  if(front == back){
    front = -1;
    back = -1;
  }else{
    front += 1;
  }

  return doc;
}