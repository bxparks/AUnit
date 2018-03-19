// If ArduinoUnit is used, this unit test no longer fits in a 32kB
// Arduino UNO or Nano.
#define USE_AUNIT 1

#if USE_AUNIT == 1

#include <AUnit.h>
using aunit::TestRunner;
using aunit::Verbosity;

#else

#include <ArduinoUnit.h>

// These are available only in AUnit so make them disappear in ArduinoUnit.
#define assertTestExpire(x)
#define assertTestNotExpire(x)
#define checkTestExpire(x) true
#define checkTestNotExpire(x) true

#endif

#ifndef FPSTR
#define FPSTR(pstr_pointer) (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))
#endif
