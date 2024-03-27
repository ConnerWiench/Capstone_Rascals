#include <stdio.h>
#include "pb_encode.h"
#include "pb_decode.h"
#include "pb.h"
#include "rad.pb.h"
#include "radQueue.h"
#include "repeated_serialization.h"

#define ARR_SIZE 256

static CQueue queue;

int main(){
    /*This is where we will store our message*/
    uint8_t buffer[100];
    size_t messageLength;
    bool status;

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

            status = pb_encode(&stream, CAPSTONE_MainData_fields, &message);
            messageLength = stream.bytes_written;

            queue.enqueue(buffer, messageLength);

            //printf("Current Count: %d\n", i);
            if (!status) {
                printf("Encoding failed! %s\n", PB_GET_ERROR(&stream));
                return 1;
            }
        }

    }

    // Decode the message
    {
        int count = 0;
        while (!queue.isEmpty()) {
            CAPSTONE_MainData message1 = CAPSTONE_MainData_init_zero;

            queue.dequeue(buffer, messageLength);

            pb_istream_t stream = pb_istream_from_buffer(buffer, messageLength);

            status = pb_decode(&stream, CAPSTONE_MainData_fields, &message1);

            if (!status) {
                printf("Decoding failed! %s\n", PB_GET_ERROR(&stream));
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


            printf("\n");
            printf("----------------------------------------\n\n\n");
            count++;
        }
    }

}