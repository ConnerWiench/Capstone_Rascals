#include <stdio.h>
#include "pb_encode.h"
#include "pb_decode.h"
#include "pb.h"
#include "rad.pb.h"
#include "repeated_serialization.h"
#include "radQueue.h"

/************************************************
*
* Note: Without the array of values, the message 
* holds a size of 11 (messageSize value).
*
* Every integer in the array adds one size...
* I.E. if array is size 10, then the size will be 21...
*
************************************************/
#define BUFFER_SIZE 12 + MAX_REPEATED_VALUES

// Define the queue we will be storeing into
static CQueue queue;

// Fucntion ot encode the top part of the data that we will be sending
void variableDataEncode(int bufferSize, uint8_t buffer[],
                                    uint32_t startTime, float totalCount, 
                                    uint32_t pulseTime, uint32_t *messageSize){
    CAPSTONE_MainData message = CAPSTONE_MainData_init_zero;

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, bufferSize);

    // Load in all of the variables that we need to encode in the message
    message.startTime = startTime;
    message.totalCount = totalCount;
    message.pulseTime = pulseTime;

    // Make sure that there were no errors with the encoding...
    if(!pb_encode(&stream, CAPSTONE_MainData_fields, &message)){
        printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));

    }else{
        // printf("Took the first split...\n");
        // Otherwise just return the normal data that we have gathered...
        *messageSize = (uint32_t)stream.bytes_written;
        //printf("The message size is: %n\n", messageSize);
    }
}

// Function to encode the various parts of the data array that we will be sending
// This will be comprised of 32 arrays the size of 128 digits to make the full 4096
void arrayEncodeData(int bufferSize, uint8_t buffer[], repeated_int data, uint32_t *messageSize){
    CAPSTONE_MainData message = CAPSTONE_MainData_init_zero;

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, bufferSize);

    // Load in dummy values for the starting variables
    message.startTime = 0;
    message.totalCount = 0;
    message.pulseTime = 0;

    // Load in the values for the array
    message.spectrum.funcs.encode = &encodeRepeatedInt;
    message.spectrum.arg = &data;

    // Make sure that there were no errors when we encoded...
    if(!pb_encode(&stream, CAPSTONE_MainData_fields, &message)){
        printf("ENCODING HAS FAILED: %s\n", PB_GET_ERROR(&stream));
    }else{
        // If it has succeeded then write the message size
        *messageSize = stream.bytes_written;
    }
}

// Function to read the data that we have just recieved from the queue
CAPSTONE_MainData returnVariableData(int bufferSize, uint8_t buffer[],
                                    uint32_t messageSize){

    CAPSTONE_MainData message = CAPSTONE_MainData_init_zero;

    pb_istream_t stream = pb_istream_from_buffer(buffer, messageSize);

    if(!pb_decode(&stream, CAPSTONE_MainData_fields, &message)){
        // This decodes the data except for the array at the end of the message.
    }

    return message;
}

// The main program function
int main(){
    /*This is where we will store our message*/
    uint8_t buffer[BUFFER_SIZE];
    uint32_t messageSize = 0;
    repeated_int data;
    data.size = MAX_REPEATED_VALUES;
    int currentArrayPlace = 0;

    // Test to see if we can store the entire 4096 in the queue structure
    uint32_t testDataBuffer[4096];
    for(uint32_t i = 0; i < 4096; i++){
        testDataBuffer[i] = i;
    }
    
    // printf("Data size: %d\n", MAX_REPEATED_VALUES);
    // printf("Buffer size: %d\n", BUFFER_SIZE);
    // printf("Finished...\n");

    // Encode the data here
    for(int i = 0; i < QUEUEBUFFERSIZE; i++){
        // If this is the first message, we only want to get the variables encoded in the queue
        if(i == 0){
            // Encode the recieved variables
            variableDataEncode(BUFFER_SIZE, buffer, 101, 102, 103, &messageSize);

            // Store this in the first queue element
            if(queue.isEmpty()){
                queue.enqueue(buffer, BUFFER_SIZE);
            }else{
                printf("ERROR: The queue is not empty, we need to clear the queue...\n");
            }
        }else{
            // Encode the recieved array
            for(uint32_t i = 0; i < data.size; i++){
                data.values[i] = testDataBuffer[i + currentArrayPlace];
                //printf("%d\n", data.values[i]);
            }

            // Set the current array place so we always get the new data
            currentArrayPlace = currentArrayPlace + 128;
            if(currentArrayPlace == 4096){
                currentArrayPlace = 0;
            }
            arrayEncodeData(BUFFER_SIZE, buffer, data, &messageSize);
            queue.enqueue(buffer, BUFFER_SIZE);
        }
    }

    // Print the encoded message
    for(int i = 0; i < QUEUEBUFFERSIZE; i++){
        // Get the stored data structure from the queue
        if(!queue.isEmpty()){
            queue.dequeue(buffer, messageSize);
            CAPSTONE_MainData testMessage = returnVariableData(BUFFER_SIZE, buffer, messageSize);
            // Check to see if it is the first item, if yes then only get the variables 
            // DO NOT GET THE ARRAY
            if(i == 0){
                // printf("CurrentMessageSize: %d\n", messageSize);
                printf("\n\n");
                printf("----------Encoded Output:--------------- \n");
                printf("------------- Count: %d ---------------- \n", i + 1);

                printf("Time since zero: %d\n", testMessage.startTime);
                printf("TID Level: %f\n", testMessage.totalCount);
                printf("Pulse Time: %d\n", testMessage.pulseTime);
            }else{
                printf("----------------------------------------\n");
                printf("----------------Spectrum----------------\n");
                printf("----------------------------------------\n");
        
                // We want to just print out the array data here, not all of the data
                for(uint32_t j = 3; j < MAX_REPEATED_VALUES + 3; j++) {
                    printf("%d, ", buffer[j]);
                }

                printf("\n");
                printf("----------------------------------------\n");
                printf("\n\n");
            }
        }
    }
}