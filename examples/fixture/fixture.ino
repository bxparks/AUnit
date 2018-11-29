#line 2 "fixture.ino"

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

void setup() {
  delay(1000); // Wait for stability on some boards, otherwise garage on Serial
  Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
  while (!Serial); // for the Arduino Leonardo/Micro only
}

void loop() {
  TestRunner::run();
}

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

  assertRecords(2, "bike", 1, "car", 2);

  // Warning: This statement will execute even if assertRecords() fails. See
  // README.md about "early returns" from assert statements.
  container.insert("train", 3);

  // This assert will return immediately if assertRecords() fails. See
  // README.md about delayed status verification.
  assertEqual(3, container.numRecords);

  // This statement will not execute if assertRecords() fails because the
  // above assertEqual() will return.
  container.insert("plane", 4);
}
