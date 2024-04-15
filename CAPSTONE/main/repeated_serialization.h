#pragma once
#include "pb.h"
#define MAX_REPEATED_VALUES 128

/**
 * @brief A wrapper used to pass to NanoPb so that it can encode/decode an array if unsigned ints
 * Feel free to use whatever you want, NanoPb just needs to know the size of the array.  If your array are static in
 * size, you dont even need a wrapper struct
 */
typedef struct repeated_int {
  uint32_t size;
  uint32_t values[MAX_REPEATED_VALUES];
} repeated_int;

/**
 * @brief Encodes repeated fields
 * 
 * @param stream the output stream
 * @param field he field to encode
 * @param arg the encoding argument
 * @return true true if encoded
 * @return false false if not encoded
 */
bool encodeRepeatedInt(pb_ostream_t* stream, const pb_field_t* field, void* const *arg);

/**
 * @brief Decodes the repeated fields
 * 
 * @param stream the input stream
 * @param field the field to decode
 * @param arg the decoding argument
 * @return true if decoded
 * @return false if not decoded
 */
bool decodeRepeatedInt(pb_istream_t *stream, const pb_field_t *field, void **arg);