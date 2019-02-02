#ifndef ARDUINO_PGMSPACE_H
#define ARDUINO_PGMSPACE_H

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
