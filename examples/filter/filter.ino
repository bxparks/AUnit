#line 2 "filter.ino"

// Adapted from:
// https://github.com/mmurdoch/arduinounit/blob/master/examples/filter/filter.ino

#include <AUnit.h>
using aunit::TestRunner;

// empty tests just to play with
test(net_tcp) { pass(); }
testing(net_udp) { pass(); }
test(net_ftp) { pass(); }
test(crypto_aes) { pass(); }
testing(crypto_rng) { pass(); }
test(crypto_sha256) { pass(); }

void setup() {
  Serial.begin(74880); // 74880 is default for some ESP8266 boards
  while(!Serial); // for the Arduino Leonardo/Micro only
  delay(1000); // wait for stability on some boards to prevent garbage Serial

  // all tests named net_ - something, except net_ftp
  TestRunner::exclude("*");
  TestRunner::include("net_*");
  TestRunner::exclude("net_ftp");
}

void loop() {
  // Should get:
  // TestRunner summary:
  //    2 passed, 0 failed, 4 skipped, 0 timed out, out of 6 test(s).
  TestRunner::run();
}
