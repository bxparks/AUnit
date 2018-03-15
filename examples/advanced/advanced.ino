#line 2 "advanced.ino"

// Copied from:
// https://github.com/mmurdoch/arduinounit/blob/master/examples/advanced/advanced.ino

#define USE_AUNIT 1

#if USE_AUNIT == 1
#include <AUnit.h>
using namespace aunit;
#else
#include <ArduinoUnit.h>
#endif

test(simple1)
{
  assertTrue(true);
}

test(simple2)
{
  assertTrue(false);
}

class MyTestOnce : public TestOnce
{
public:
  // constructor must name test

  MyTestOnce(const char *name) 
  : TestOnce(name) 
  {
    // lightweight constructor, since
    // this test might be skipped.
    // you can adjust verbosity here

#if USE_AUNIT == 0
    verbosity = TEST_VERBOSITY_ALL;
#endif
  }
  
  int n;

  void setup()
  {
    n = random(6);
    if (n == 0) skip();
  }

  void once()
  {
    for (int i=-n; i<=n; ++i) {
      for (int j=-n; j<=n; ++j) {
        assertEqual(i+j,j+i);
      }
    }
  }
};

MyTestOnce myTestOnce1("myTestOnce1");
MyTestOnce myTestOnce2("myTestOnce2");
MyTestOnce myTestOnce3("myTestOnce3");

class MyTest : public Test
{
public:
  uint16_t when;
  MyTest(const char *name) 
  : Test(name)
  {
    when = random(100,200);
  }

  void loop()
  {
    if (millis() >= when) 
    {
      assertLess(random(100),50L);
      pass(); // if assertion is ok
    }
  }  
};

MyTest myTest1("myTest1");
MyTest myTest2("myTest2");
MyTest myTest3("myTest3");

void setup()
{
  Serial.begin(74880); // 74880 is default for some ESP8266 boards
  while(!Serial); // for the Arduino Leonardo/Micro only

#if USE_AUNIT == 1
  TestRunner::setVerbosity(Verbosity::kAll);
  TestRunner::exclude("myTestOnce2");
  TestRunner::exclude("myTest2");
#else
  Test::min_verbosity |= TEST_VERBOSITY_ASSERTIONS_ALL;
  Test::exclude("my*2");
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
