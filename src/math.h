/*
    Math functionality test on Arduino Uno
   Andrew Bizyaev (ANB) github.com/andrewbiz
*/
#include <Arduino.h>
#include <inttypes.h>
#include <Logging.h>
#include <Fp32s.hpp>

#define LOGLEVEL LOG_LEVEL_DEBUG // see Logging.h for options
#define LOG_PRINT_TS true  // print time stamp in logging
#define LOG_AUTO_LN  true  // print auto LN (CR) after each call

#define Byte1(w) ((uint8_t) ((w) & 0xff))
#define Byte2(w) ((uint8_t) ((w) >> 8))
#define Byte3(w) ((uint8_t) ((w) >> 16))
#define Byte4(w) ((uint8_t) ((w) >> 24))
#define Byte5(w) ((uint8_t) ((w) >> 32))
#define Byte6(w) ((uint8_t) ((w) >> 40))
#define Byte7(w) ((uint8_t) ((w) >> 48))
#define Byte8(w) ((uint8_t) ((w) >> 56))

void print_bin64(const char * vname, uint64_t v);
void print_bin32(const char * vname, uint32_t v);
