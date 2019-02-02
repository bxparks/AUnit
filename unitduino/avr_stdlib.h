/**
 * @file avr_stdlib.h
 *
 * Non-standard functions from AVR's <stdlib.h> library which do not exist on
 * Linux or MacOS. This contains only the ones required to compile
 * AUnit tests on Linux or MacOS.
 */

#ifndef UNITDUINO_AVR_STDLIB_H
#define UNITDUINO_AVR_STDLIB_H

extern char *itoa(int n, char *str, int base);
extern char *utoa(unsigned n, char *str, int base);
extern char *ltoa(long n, char *str, int base);
extern char *ultoa(unsigned long n, char *str, int base);
extern char *dtostrf(double val, signed char width, unsigned char prec,
    char *s);

#endif
