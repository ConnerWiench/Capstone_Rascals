#include <iostream>
#include <cstdint>

class CQueue{
    private: 
        static const size_t bufferSize = 200;
        uint8_t buffer[bufferSize][128];
        size_t front;
        size_t rear;
        size_t currentSize;

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

        void unqueue(uint8_t result[128]);

        void addqueue(const uint8_t value[128]);
};

void CQueue::unqueue(uint8_t result[128]){
    if(!isEmpty()){
        std::copy(buffer[front], buffer[front] + 128, result);
        front = (front + 1) % bufferSize;
        --currentSize;
    }else{
        //the queue is empty and cannot yeild a value (for debug)
        std::cerr << "Error: queue is empty..." << std::endl;
        std::fill_n(result, 128, 0); //return an array filled with 0 as a default
    }
}

void CQueue::addqueue(const uint8_t value[128]){
    if(!isFull()){
        std::copy(value, value + 128, buffer[rear]);
        rear = (rear + 1) % bufferSize;
        ++currentSize;
    }else{
        //print a warnign the the queue is full (for debug)
        std::cerr << "Warning: Queue is full, overwriting first element..." << std::endl;
        uint8_t dummyValues[128];
        unqueue(dummyValues); //remove the first element
        addqueue(value); // add the new value to the newfound space
    }
}
