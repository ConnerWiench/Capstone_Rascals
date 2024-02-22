#include <stdio.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "rad.pb.h"
#include "radQueue.h"

static CQueue queue;

//note - the nanopb needs to be a uint8_t, but we can still add the other 32 to the struct
//we created with the .proto file. --honestly works pretty freaking smooth
int main(){
    /*This is where we will store our message*/
    uint8_t buffer[128];
    size_t messageLength;
    bool status;

    //encode the message
    {
        for(int i = 0; i < 128; i++){
            Data message = Data_init_zero;

            //make a stream that will write to the file (the buffer)
            pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

            //fill in the message fields
            message.radiationTotalCount = 10001 + i;
            message.radiationTotalPulseTime = 10002 + i;
            message.integerBitSpectrum = 10003 + i;

            //now we can encode the message
            status = pb_encode(&stream, Data_fields, &message);
            messageLength = stream.bytes_written;

            queue.enqueue(buffer);
        

        //double check for any errors
        if(!status){
            printf("Encoding failed! %s\n", PB_GET_ERROR(&stream));
            return 1;
        }
        }
    }

    //here we can do whatever in the background, or we can just send it

    //decode the message
    {
        int count = 0;
        while(!queue.isEmpty()){
            Data message1 = Data_init_zero;

            //get the data from the queue
            queue.dequeue(buffer);

            //make a stream that will read from the buffer
            pb_istream_t stream = pb_istream_from_buffer(buffer, messageLength);

            //we are ready to decode the message now
            status = pb_decode(&stream, Data_fields, &message1);

            //double check if there are any errors
            if(!status){
                printf("Decodeing failed! %s\n", PB_GET_ERROR(&stream));
                return 1;
            }

            //print the message below
            printf("Count: %d\n", count);
            printf("Radiation count: %f\n", message1.radiationTotalCount);
            printf("Radiation Total pulse: %f\n", message1.radiationTotalPulseTime);
            printf("IntegerBitSpectrum: %d\n", message1.integerBitSpectrum);
            printf("\n");

            count++;
        }

    }
}