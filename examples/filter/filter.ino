#line 2 "filter.ino"

// Copied from:
// https://github.com/mmurdoch/arduinounit/blob/master/examples/filter/filter.ino

#define USE_AUNIT 1

#if USE_AUNIT == 1
#include <AUnit.h>
using aunit::TestRunner;
#else
#include <ArduinoUnit.h>
#endif

// empty tests just to play with
test(net_tcp) { pass(); }
testing(net_udp) { pass(); }
test(net_ftp) { pass(); }
test(crypto_aes) { pass(); }
testing(crypto_rng) { pass(); }
test(crypto_sha256) { pass(); }

void setup()
{
  Serial.begin(74880); // 74880 is default for some ESP8266 boards
  while(!Serial); // for the Arduino Leonardo/Micro only
  
  // all tests named net_ - something, except net_ftp
#if USE_AUNIT == 1
  TestRunner::exclude("*");
  TestRunner::include("net_*");
  TestRunner::exclude("net_ftp");
#else
  Test::exclude("*");
  Test::include("net_*");
  Test::exclude("net_ftp");
#endif
}

void loop()
{
#if USE_AUNIT == 1
  TestRunner::run();
#else
  Test::run();
#endif
}
