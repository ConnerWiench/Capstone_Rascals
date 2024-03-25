#include <stdio.h>
#include "pb_encode.h"
#include "pb_decode.h"
#include "pb.h"
#include "rad.pb.h"
#include "repeated_serialization.h"
#include "radQueue.h"

#define ARR_SIZE 15
#define BUFFER_SIZE 128

static CQueue queue;

// Function to encode the data that we will be sending
CAPSTONE_MainData encodeMainData(int bufferSize, uint8_t buffer[],
                   repeated_int data, float startTime, float totalCount, float lattitude,
                   float longitude, float altitude, uint32_t pulseTime, uint32_t *messageSize, uint32_t count){
    
    CAPSTONE_MainData message = CAPSTONE_MainData_init_zero;

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, bufferSize);

    // Load in all of the variables that we need to encode in the message
    message.startTime = startTime;
    message.totalCount = totalCount;
    message.lattitude = lattitude;
    message.longitude = longitude;
    message.altitude = altitude;
    message.pulseTime = pulseTime;
    message.spectrum_count = count;

    // Make sure to encode the array
    message.spectrum.funcs.encode = &encodeRepeatedInt;
    message.spectrum.arg = &data;

    // Make sure that there were no errors with the encoding...
    if(!pb_encode(&stream, CAPSTONE_MainData_fields, &message)){
        printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
        CAPSTONE_MainData message1 = CAPSTONE_MainData_init_zero;

        // Return array of 0's in the event that it doesn't work...
        return message1;
    }else{
        // Otherwise just return the normal data that we have gathered...
        *messageSize = stream.bytes_written;
        return message;
    }
}

// Function to "decode" what we have stored in our queue
CAPSTONE_MainData decodeMessage(int bufferSize, uint8_t buffer[], uint32_t messageSize){
    CAPSTONE_MainData returnMessage = CAPSTONE_MainData_init_zero;

    pb_istream_t outStream = pb_istream_from_buffer(buffer, messageSize);

    if(!pb_decode(&outStream, CAPSTONE_MainData_fields, &returnMessage)){
        // This decodes the data except for the array at the end of the message.
    }

    return returnMessage;
}

// Function to see the data that we have sent via the program
void printInfo(CAPSTONE_MainData message, uint8_t buffer[], uint32_t messageSize, int encodeFlag){
    if(encodeFlag == 1){
        printf("---------------Encoded Output:---------- \n");
    }else{
        printf("-------------Decoded Output:------------ \n");
    }
    printf("------------- Count: %d ---------------- \n", message.spectrum_count + 1);

    printf("Time since zero: %f\n", message.startTime);
    printf("TID Level: %f\n", message.totalCount);
    printf("Longitude: %f\n", message.longitude);
    printf("Lattitude: %f\n", message.lattitude);
    printf("Altitude: %f\n", message.altitude);
    printf("Pulse Time: %d\n", message.pulseTime);

    printf("----------------------------------------\n");
    printf("----------------Spectrum----------------\n");
    printf("----------------------------------------\n");

    // We want to just print out the array data here, not all of the data
    for(uint32_t j = messageSize - MAX_REPEATED_VALUES; j < messageSize; j++) {
        printf("%x, ", buffer[j]);
    }

    printf("\n");
    printf("----------------------------------------\n");
    printf("\n\n");
}

// The main program function
int main(){
    /*This is where we will store our message*/
    uint8_t buffer[BUFFER_SIZE];
    uint32_t messageSize = 0;

    repeated_int data = {10, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};

    // Encode the message
    for(int i = 0; i < ARR_SIZE; i++){
        CAPSTONE_MainData testMessage = encodeMainData(BUFFER_SIZE, buffer, data, 1 + i, 2 + i, 3 + i, 4 + i, 5 + i, 6 + i, &messageSize, i);

        // Print out the encoded message
        printInfo(testMessage, buffer, messageSize, 1);

        // Queue the message here
        queue.enqueue(buffer, messageSize);
    }

    // Decode all of the messages
    while(!queue.isEmpty()){
        queue.dequeue(buffer, messageSize);

        CAPSTONE_MainData returnMessage = decodeMessage(BUFFER_SIZE, buffer, messageSize);

        printInfo(returnMessage, buffer, messageSize, 0);     
    }
}