/*
 * Determine the flash and static memory consumption of a minimal AUnit test.
 */

#include <stdint.h> // uint8_t
#include <Arduino.h>

// List of features of the AUnit library that we want to examine.
#define FEATURE_BASELINE 0
#define FEATURE_AUNIT 1
#define FEATURE_AUNIT_VERBOSE 2

// Select one of the FEATURE_* parameter and compile. Then look at the flash
// and RAM usage, compared to FEATURE_BASELINE usage to determine how much
// flash and RAM is consumed by the selected feature.
// NOTE: This line is modified by a 'sed' script in collect.sh. Be careful
// when modifying its format.
#define FEATURE 0

#if FEATURE == FEATURE_AUNIT
  #include <AUnit.h>
  using namespace aunit;
#elif FEATURE == FEATURE_AUNIT_VERBOSE
  #include <AUnitVerbose.h>
  using namespace aunit;
#endif

// Define SERIAL_PORT_MONITOR for ESP32
#ifndef SERIAL_PORT_MONITOR
  #define SERIAL_PORT_MONITOR Serial
#endif

// Set this variable to prevent the compiler optimizer from removing the code
// being tested when it determines that it does nothing.
volatile uint8_t guard;

// Define one unit test if FEATURE_AUNIT is enabled.
#if FEATURE == FEATURE_BASELINE
  // pass
#elif FEATURE == FEATURE_AUNIT || FEATURE == FEATURE_AUNIT_VERBOSE
test(atest) {
  guard = 1;
  assertEqual(1, guard);
}
#else
  #error Unknown FEATURE
#endif

void setup() {
#if ! defined(EPOXY_DUINO)
  delay(2000);
#endif

  // Setup the Serial, to force inclusion of the Print class in the Baseline, so
  // that we can measure the incremental amount of code brought in by PrintStr<>
  // and PrintStrN<>.
  SERIAL_PORT_MONITOR.begin(115200);
  while (! SERIAL_PORT_MONITOR); // Leonardo/Micro

#if defined(EPOXY_DUINO)
  SERIAL_PORT_MONITOR.setLineModeUnix();
#endif
}

void loop() {
#if FEATURE != FEATURE_BASELINE
  aunit::TestRunner::run();
#endif
}
