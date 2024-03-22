#include <stdio.h>
#include "pb_encode.h"
#include "pb_decode.h"
#include "pb.h"
#include "rad.pb.h"
#include "radQueue.h"
#include "repeated_serialization.h"

#define ARR_SIZE 15

static CQueue queue;

int main(){
    /*This is where we will store our message*/
    uint8_t buffer[128];
    size_t messageLength;
    bool status;

    repeated_int data = {10, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    // Encode the message
    {
        for(int i = 0; i < ARR_SIZE; i++){
            CAPSTONE_MainData message = CAPSTONE_MainData_init_zero; //{0, 0, 0, 0, 0, 0, {{NULL}, NULL}, 0}

            pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

            message.startTime = 10001 + i;
            message.totalCount = 10002 + i;
            message.lattitude = 10003 + i;
            message.longitude = 10004 + i;
            message.altitude = 10005 + i;
            message.pulseTime = 10006 + i;

            //encode the array here
            message.spectrum.funcs.encode = &encodeRepeatedInt;
            message.spectrum.arg = &data;

            status = pb_encode(&stream, CAPSTONE_MainData_fields, &message);
            messageLength = stream.bytes_written;

            // TroubleShooting
            printf("[%d]: MessageLength: %ld\n", i + 1, messageLength);

            queue.enqueue(buffer, messageLength);

            //printf("Current Count: %d\n", i);
            if (!status) {
                printf("Encoding failed! %s\n", PB_GET_ERROR(&stream));
                return 1;
            }

            // Test the print here...
            for(uint32_t i = 0; i < data.size; i++) {
                printf("%x, ", data.values[i]);
            }
            printf("\n");
        }

    }

    printf("--------------------------------------------------\n");

    // Decode the message
    {
        int count = 0;
        while (!queue.isEmpty()) {
            CAPSTONE_MainData message1 = CAPSTONE_MainData_init_zero;
            
            messageLength = 0;
            queue.dequeue(buffer, messageLength);
            
            printf("Message length: %ld\n", messageLength);

            pb_istream_t stream1 = pb_istream_from_buffer(buffer, messageLength);

            //decode the spectrum...
            repeated_int *data1;
            message1.spectrum.funcs.decode = &decodeRepeatedInt;
            data1 = (repeated_int*)message1.spectrum.arg;
            
            status = pb_decode(&stream1, CAPSTONE_MainData_fields, &message1);

            if (!status) {
                printf("Decoding -- failed! %s\n", PB_GET_ERROR(&stream1));
                return 1;
            }

            printf("------------- Count: %d ---------------- \n", count);

            printf("Time since zero: %f\n", message1.startTime);
            printf("TID Level: %f\n", message1.totalCount);
            printf("Longitude: %f\n", message1.longitude);
            printf("Lattitude: %f\n", message1.lattitude);
            printf("Altitude: %f\n", message1.altitude);
            printf("Pulse Time: %d\n", message1.pulseTime);

            printf("----------------------------------------\n");
            printf("----------------Spectrum----------------\n");
            printf("----------------------------------------\n");

            for(uint32_t j = 0; j < data1->size; j++){
                printf("%u", data1->values[j]);
            }

            printf("\n");
            printf("----------------------------------------\n\n\n");
            count++;
        }
    }

}