/**
 * @file pgmspace.h
 *
 * A version of the <avr/pgmspace.h> file in the Arduino environment with
 * enough mappings to allow AUnit tests to compile and run on Linux or MacOS.
 */

#ifndef UNITDUINO_PGMSPACE_H
#define UNITDUINO_PGMSPACE_H

#define PGM_P const char *
#define PGM_VOID_P const void *
#define PSTR(str) (str)

#define PROGMEM

#define pgm_read_byte(address_short)    (*(address_short))

#define strlen_P strlen
#define strcat_P strcat
#define strcpy_P strcpy
#define strncpy_P strncpy
#define strcmp_P strcmp
#define strncmp_P strncmp
#define strcasecmp_P strcasecmp

#endif
