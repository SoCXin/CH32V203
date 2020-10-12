////////////////////////////////////////////////////////////////////////////////
/// @file:   dtype.h
/// @author   AE TEAM
/// @brief   Define the data types to be used in the project, including the function
///          library and application code. Use the data types defined in this file.
////////////////////////////////////////////////////////////////////////////////
#ifndef __DTYPE_H
#define __DTYPE_H     //This is done to avoid including the header file repeatedly in the same file


//Defines the read and write characteristics of data, which is often used for storage limits of peripheral registers

#define     __I     volatile const    //only read
#define     __O     volatile          //only write
#define     __IO    volatile          //read write


//Common data type definitions

typedef unsigned char  int8u;         //haven't symbol8 bit integer variable
typedef signed   char  int8s;         //have symbol8 bit integer variable
typedef unsigned short int16u;        //haven't symbol16 bit integer variable
typedef signed   short int16s;        //have symbol16 bit integer variable
typedef unsigned int   int32u;        //haven't symbol32 bit integer variable
typedef signed   int   int32s;        //have symbol32 bit integer variable
typedef float          fp32;          //Single-precision floating-point number (32-bit length)
typedef double         fp64;          //Double-precision floating-point number (64-bit length)


//Data type definitions used by the standard library

//typedef   signed          char int8_t;
//typedef   signed short     int int16_t;
//typedef   signed           int int32_t;
//
//typedef unsigned          char uint8_t;
//typedef unsigned short     int uint16_t;
//typedef unsigned           int uint32_t;
//
//typedef int32_t   s32;
//typedef int16_t   s16;
//typedef int8_t    s8;
//typedef uint32_t  u32;
//typedef uint16_t  u16;
//typedef uint8_t   u8;

// typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
// typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
// typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
// #define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

#endif  //__DTYPE_H
