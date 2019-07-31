/*
 * Copyright (c) 2019 Brian T. Park
 * MIT License
 */

#include <stdio.h>
#include "StdioSerial.h"

size_t StdioSerial::write(uint8_t c) {
  int result = putchar(c);
  return (result == EOF) ? 0 : 1;
}

StdioSerial Serial;
