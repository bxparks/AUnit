#include <stdio.h>
#include "SoftwareSerial.h"

size_t SoftwareSerial::write(uint8_t c) {
  int result = putchar(c);
  return (result == EOF) ? 0 : 1;
}

SoftwareSerial Serial;
