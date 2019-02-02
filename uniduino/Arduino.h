#ifndef ARDUINO_ARDUINO_H
#define ARDUINO_ARDUINO_H

#include "Print.h"
#include "SoftwareSerial.h"

extern void delay(unsigned long ms);
extern void yield();
extern unsigned long millis();

/** Provided in the client code's *.ino file. */
extern void setup();

/** Provided in the client code's *.ino file. */
extern void loop();

#endif
