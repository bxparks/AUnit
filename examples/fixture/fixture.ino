#line 2 "fixture.ino"

/*
 * Unit test that shows how to use test fixtures using the testF() macro.
 */

#include <stdarg.h>
#include <AUnit.h>

using namespace aunit;

const int8_t NUM_RECORDS = 4;

// A single record.
struct Record {
  const char* name;
  int value;
};
  
// A container of Records.
class Container {
  public:
    Container(): numRecords(0) {}

    void clear() { numRecords = 0; }

    void insert(const char* name, int value) {
      if (numRecords >= NUM_RECORDS) return;
      records[numRecords].name = name;
      records[numRecords].value = value;
      numRecords++;
    }

    int numRecords;
    Record records[NUM_RECORDS];
};

// Create an instance of the Container.
Container container;

class LogTest: public TestOnce {
  protected:
    void setup() override {
      TestOnce::setup();
      container.clear();
    }

    void teardown() override {
      container.clear();
      TestOnce::teardown();
    }

    // Variable number of arguments so that we can check the entire
    // array of Records.
    void assertRecords(int n, ...) {
      assertEqual(n, container.numRecords);
      va_list args;
      va_start(args, n);
      for (int i = 0; i < n; i++) {
        const char* name = va_arg(args, const char*);
        int value = va_arg(args, int);
        assertEqual(name, container.records[i].name);
        assertEqual(value, container.records[i].value);
      }
      va_end(args);
    }
};

// The testF() macro creates a subclass named 'LogTest_insert' which inherits
// from the LogTest class. The code following the macro becomes the body of the
// LogTest_insert::once() method. The test body has access to the public and
// protected methods of the parent LogTest class, such as assertRecords().
testF(LogTest, insert) {
  container.insert("bike", 1);
  container.insert("car", 2);

  // Very useful for debugging assertRecords()
  // enableVerbosity(Verbosity::kAssertionPassed);

  // The assertNoFatalFailure() macro prevents execution from continuing if
  // assertRecords() contains a failure.
  assertNoFatalFailure(assertRecords(2, "bike", 1, "car", 2));

  // The assertNoFatalFailure() above prevents this statement from executing if
  // assertRecords() fails.
  container.insert("train", 3);

  // This assert will return immediately upon failure.
  assertEqual(3, container.numRecords);

  // This statement will not execute if the above assertEqual() fails.
  container.insert("plane", 4);
}

//----------------------------------------------------------------------------
// setup() and loop()
//----------------------------------------------------------------------------

void setup() {
  delay(1000); // Wait for stability on some boards, otherwise garage on Serial
  Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
  while (!Serial); // for the Arduino Leonardo/Micro only

  Serial.println(F("This test should produce the following:"));
  Serial.println(
    F("1 passed, 0 failed, 0 skipped, 0 timed out, out of 1 test(s).")
  );
  Serial.println(F("----"));
}

void loop() {
  TestRunner::run();
}
