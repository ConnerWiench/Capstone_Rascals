#include <stdio.h>
#include "pb_encode.h"
#include "pb_decode.h"
#include "pb.h"
#include "rad.pb.h"
#include "repeated_serialization.h"

/************************************************
*
* Note: Without the array of values, the message 
* holds a size of 11 (messageSize value).
*
* Every integer in the array adds one size...
* I.E. if array is size 10, then the size will be 21...
*
************************************************/

#define ARR_SIZE 15
#define BUFFER_SIZE 12 + MAX_REPEATED_VALUES

// Function to encode the data that we will be sending
CAPSTONE_MainData encodeMainData(int bufferSize, uint8_t buffer[],
                   repeated_int data, uint32_t startTime, float totalCount,
                   uint32_t pulseTime, uint32_t *messageSize){
    
    CAPSTONE_MainData message = CAPSTONE_MainData_init_zero;

    printf("Created the data...\n");

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, bufferSize);


    printf("Opened the stream...\n");

    // Load in all of the variables that we need to encode in the message
    message.startTime = startTime;
    message.totalCount = totalCount;
    message.pulseTime = pulseTime;

    printf("Loaded the basic messages...\n");

    // Make sure to encode the array
    message.spectrum.funcs.encode = &encodeRepeatedInt;
    message.spectrum.arg = &data;

    printf("Loaded the spectrum of values...\n");

    // Make sure that there were no errors with the encoding...
    if(!pb_encode(&stream, CAPSTONE_MainData_fields, &message)){
        printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
        CAPSTONE_MainData message1 = CAPSTONE_MainData_init_zero;

        // Return array of 0's in the event that it doesn't work...
        return message1;
    }else{
        printf("Took the first split...\n");
        // Otherwise just return the normal data that we have gathered...
        *messageSize = (uint32_t)stream.bytes_written;
        //printf("The message size is: %n\n", messageSize);
        return message;
    }
}

// The main program function
int main(){
    /*This is where we will store our message*/
    uint8_t buffer[BUFFER_SIZE];
    uint32_t messageSize = 0;
    repeated_int data;
    data.size = MAX_REPEATED_VALUES;
    
    for(int i = 0; i < data.size; i++){
        data.values[i] = (int)i;
        //printf("%d\n", data.values[i]);
    }
    printf("Data size: %d\n", MAX_REPEATED_VALUES);
    printf("Buffer size: %d\n", BUFFER_SIZE);
    printf("Finished...\n");

    // Encode the message
    for(int i = 0; i < ARR_SIZE; i++){
        printf("Encoding Data...\n");
        CAPSTONE_MainData testMessage = encodeMainData(BUFFER_SIZE, buffer, data, 1 + i, 2 + i, 3 + i, &messageSize);
        printf("Finished encoding data...\n");
        printf("CurrentMessageSize: %d\n", messageSize);
        printf("\n\n");
        printf("----------Encoded Output:--------------- \n");
        printf("------------- Count: %d ---------------- \n", i + 1);

        printf("Time since zero: %d\n", testMessage.startTime);
        printf("TID Level: %f\n", testMessage.totalCount);
        printf("Pulse Time: %d\n", testMessage.pulseTime);

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
}