/**
 * @file SoftwareSerial.h
 *
 * A version of SoftwareSerial that supports AUnit tests on Linux or MaxOS.
 */

#ifndef UNITDUINO_SOFTWARE_SERIAL_H
#define UNITDUINO_SOFTWARE_SERIAL_H

#include "Print.h"

class SoftwareSerial: public Print {
  public:
    void begin(unsigned long baud) { }

    size_t write(uint8_t c) override;

    operator bool() { return true; }
};

extern SoftwareSerial Serial;

#endif
