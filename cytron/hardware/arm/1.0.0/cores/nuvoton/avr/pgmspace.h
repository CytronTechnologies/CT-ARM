#ifndef __PGMSPACE_H_
#define __PGMSPACE_H_ 1

#include "Arduino.h"
#include <ctype.h>
#include <inttypes.h>
//TODO: found out ways to store program in program memory instead of SRAM

/** store in flash for ARM */
#define PROGMEM
/** pointer to flash for ARM */
#define PGM_P const char*
/** store literal string in flash for ARM */
#define PSTR(x) (x)

#define _SFR_BYTE(n) (n)

typedef void prog_void;
typedef char prog_char;
typedef unsigned char prog_uchar;
typedef int8_t prog_int8_t;
typedef uint8_t prog_uint8_t;
typedef int16_t prog_int16_t;
typedef uint16_t prog_uint16_t;
typedef int32_t prog_int32_t;
typedef uint32_t prog_uint32_t;

/** read 8-bits from flash for ARM */
#define pgm_read_byte(addr) (*(const unsigned char*)(addr))
/** read 16-bits from flash for ARM */
#define pgm_read_word(addr) (*(const uint16_t*)(addr))
/** read 32-bits from flash for ARM */
#define pgm_read_dword(addr) (*(const uint32_t*)(addr))
/** read float from flash for ARM */
#define pgm_read_float(addr) (*(const float*)(addr))

#define pgm_read_byte_near(addr) pgm_read_byte(addr)
#define pgm_read_word_near(addr) pgm_read_byte(addr)
#define pgm_read_dword_near(addr) pgm_read_dword(addr)
#define pgm_read_float_near(addr) pgm_read_float(addr)
#define pgm_read_byte_far(addr) pgm_read_byte(addr)
#define pgm_read_word_far(addr) pgm_read_word(addr)
#define pgm_read_dword_far(addr) pgm_read_dword(addr)
#define pgm_read_float_far(addr) pgm_read_float(addr)

#define SIZE_IRRELEVANT 0x7FFFFFFF

#ifdef __cplusplus
extern "C" {
#endif

void* memcpy_P(void* dest, const void* src, size_t count);
char* strncpy_P(char* dest, const char* src, size_t size);
char* strncat_P(char* dest, const char* src, size_t size);
int strncmp_P(const char* str1, const char* str2P, size_t size);
int strncasecmp_P(const char* str1, const char* str2P, size_t size);
size_t strnlen_P(const char *s, size_t size);

#ifdef __cplusplus
}
#endif

#define strcpy_P(dest, src)          strncpy_P((dest), (src), SIZE_IRRELEVANT)
#define strcat_P(dest, src)          strncat_P((dest), (src), SIZE_IRRELEVANT)
#define strcmp_P(str1, str2P)          strncmp_P((str1), (str2P), SIZE_IRRELEVANT)
#define strcasecmp_P(str1, str2P)          strncasecmp_P((str1), (str2P), SIZE_IRRELEVANT)
#define strlen_P(strP)          strnlen_P((strP), SIZE_IRRELEVANT)
#define sprintf_P(s, f, ...) sprintf((s), (f), __VA_ARGS__)

//newly added these lines for testing
#define snprintf_P(s, f, ...) snprintf((s), (f), __VA_ARGS__)
#define printf_P printf

//TODO: add more stdio functions

#endif
