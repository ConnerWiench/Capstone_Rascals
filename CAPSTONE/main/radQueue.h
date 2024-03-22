#pragma once
#include <iostream>
#include <cstdint>

class CQueue{
    private: 
        static const size_t bufferSize = 256;
        uint8_t buffer[bufferSize][128];
        size_t front;
        size_t rear;
        size_t currentSize;
        size_t messageLength[bufferSize];

    public:
        CQueue() : front(0), rear(0), currentSize(0) {}

        bool isEmpty() const {
            return currentSize == 0;
        }

        bool isFull() const {
            return currentSize == bufferSize;
        }

        size_t size() const {
            return currentSize;
        }

        void dequeue(uint8_t result[128], size_t &length);

        void enqueue(const uint8_t value[128], size_t length);
};

void CQueue::dequeue(uint8_t result[128], size_t &length){
    if(!isEmpty()){
        std::copy(buffer[front], buffer[front] + 128, result);
        length = messageLength[front];
        front = (front + 1) % bufferSize;
        --currentSize;
    }else{
        //the queue is empty and cannot yeild a value (for debug)
        std::cerr << "Error: queue is empty..." << std::endl;
        std::fill_n(result, 128, 0); //return an array filled with 0 as a default
        length = 0; // return a length of 0
    }
}

void CQueue::enqueue(const uint8_t value[128], size_t length){
    if(!isFull()){
        std::copy(value, value + 128, buffer[rear]);
        messageLength[rear] = length;
        rear = (rear + 1) % bufferSize;
        ++currentSize;
    }else{
        //print a warnign the the queue is full (for debug)
        std::cerr << "Warning: Queue is full, overwriting first element..." << std::endl;
        uint8_t dummyValues[128];
        size_t dummyLength;
        dequeue(dummyValues, dummyLength); //remove the first element
        enqueue(value, length); // add the new value to the newfound space
    }
}
