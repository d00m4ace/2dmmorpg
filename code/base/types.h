//******************************
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int64;
//******************************
#ifndef NULL
#define NULL ((void*)0)
#endif
//******************************
typedef void* hal_handler;
//******************************
// cstl base structs
//******************************
typedef struct c_vec_of(const char*) c_vec_str;

typedef struct c_vec_of(void*) c_vec_void;

typedef struct c_vec_of(uint8) c_vec_uint8;
typedef struct c_vec_of(uint16) c_vec_uint16;
typedef struct c_vec_of(uint32) c_vec_uint32;
typedef struct c_vec_of(uint64) c_vec_uint64;

typedef struct c_vec_of(int8) c_vec_int8;
typedef struct c_vec_of(int16) c_vec_int16;
typedef struct c_vec_of(int32) c_vec_int32;
typedef struct c_vec_of(int64) c_vec_int64;
//******************************
