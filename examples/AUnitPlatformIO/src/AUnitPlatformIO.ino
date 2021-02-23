#line 2 "AUnitPlatformIO.ino"

/*
 * Duplicated from ../AUnitPlatformIO.ino because the Arduino Library Manager
 * does not allow symlinks (see
 * https://github.com/arduino/Arduino/wiki/Library-Manager-FAQ). So when I
 * created the symlink at v1.3 on 2019-06-05, the Library Manager stopped
 * updating the library for almost 2 years, until I removed the symlink at
 * v1.5.2 on 2021-02-23.
 */

#include <AUnit.h>

test(exampleTest) {
  assertEqual(3, 3);
}

//----------------------------------------------------------------------------
// setup() and loop()
//----------------------------------------------------------------------------

void setup() {
#if ARDUINO
  delay(1000); // wait for stability on some boards to prevent garbage Serial
#endif
  Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
  while(!Serial); // for the Arduino Leonardo/Micro only
}

void loop() {
  aunit::TestRunner::run();
}
