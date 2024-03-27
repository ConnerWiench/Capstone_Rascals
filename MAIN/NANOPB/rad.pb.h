/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.9-dev */

#ifndef PB_CAPSTONE_RAD_PB_H_INCLUDED
#define PB_CAPSTONE_RAD_PB_H_INCLUDED
#include "pb.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
typedef struct _CAPSTONE_MainData {
    float startTime; /* Time since zeroed out */
    float totalCount; /* TID level (Total Ionizing dose) */
    float longitude; /* longitude of the gps at data read */
    float lattitude; /* lattitude of the gps at data read */
    float altitude; /* how high the payload is */
    int32_t pulseTime; /* Duration of the read */
    int32_t spectrum_count;
    pb_callback_t spectrum; /* spectrum data */
} CAPSTONE_MainData;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define CAPSTONE_MainData_init_default           {0, 0, 0, 0, 0, 0, 0, {{NULL}, NULL}}
#define CAPSTONE_MainData_init_zero              {0, 0, 0, 0, 0, 0, 0, {{NULL}, NULL}}

/* Field tags (for use in manual encoding/decoding) */
#define CAPSTONE_MainData_startTime_tag          1
#define CAPSTONE_MainData_totalCount_tag         2
#define CAPSTONE_MainData_longitude_tag          5
#define CAPSTONE_MainData_lattitude_tag          6
#define CAPSTONE_MainData_altitude_tag           7
#define CAPSTONE_MainData_pulseTime_tag          8
#define CAPSTONE_MainData_spectrum_count_tag     9
#define CAPSTONE_MainData_spectrum_tag           10

/* Struct field encoding specification for nanopb */
#define CAPSTONE_MainData_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, FLOAT,    startTime,         1) \
X(a, STATIC,   SINGULAR, FLOAT,    totalCount,        2) \
X(a, STATIC,   SINGULAR, FLOAT,    longitude,         5) \
X(a, STATIC,   SINGULAR, FLOAT,    lattitude,         6) \
X(a, STATIC,   SINGULAR, FLOAT,    altitude,          7) \
X(a, STATIC,   SINGULAR, INT32,    pulseTime,         8) \
X(a, STATIC,   SINGULAR, INT32,    spectrum_count,    9) \
X(a, CALLBACK, REPEATED, UINT32,   spectrum,         10)
#define CAPSTONE_MainData_CALLBACK pb_default_field_callback
#define CAPSTONE_MainData_DEFAULT NULL

extern const pb_msgdesc_t CAPSTONE_MainData_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define CAPSTONE_MainData_fields &CAPSTONE_MainData_msg

/* Maximum encoded size of messages (where known) */
/* CAPSTONE_MainData_size depends on runtime parameters */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif