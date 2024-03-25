#include "repeated_serialization.h"
#include "pb_encode.h"
#include "pb_decode.h"

bool encodeRepeatedInt(pb_ostream_t* stream, const pb_field_t* field, void* const *arg) {
  if(stream == NULL || field == NULL || arg == NULL) return false;
  repeated_int *data = (repeated_int *)*arg;

  // you guys may want to add a check for array out of bounds

  // 1. Get the encoding size
  pb_ostream_t sizeStream = PB_OSTREAM_SIZING;
  for(uint8_t i = 0; i < data->size; ++i) {
    if(!pb_encode_varint(&sizeStream, data->values[i])) return false;
  }

  // 2. Encode all the parameters
  if(!pb_encode_tag_for_field(stream, field)) return false;
  if(!pb_encode_varint(stream, sizeStream.bytes_written)) return false;
  for(uint8_t i = 0; i < data->size; ++i) {
    if(!pb_encode_varint(stream, data->values[i])) return false;
  }

  return true;
}

bool decodeRepeatedInt(pb_istream_t *stream, const pb_field_t *field, void **arg) {
  if(stream == NULL || field == NULL || arg == NULL) return false;
  repeated_int *data = (repeated_int *)*arg;

  // you guys may want to add a check for array out of bounds

  // 1. Decode the next value
  uint32_t val = 0;
  if(!pb_decode_varint32(stream, &val)) return false;

  data->values[data->size] = val;
  data->size += 1;

  return true;
}