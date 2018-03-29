#include "TestOnce.h"

namespace aunit {

void TestOnce::loop() {
  once();
  if (getStatus() == kStatusSetup) {
    pass();
  }
}

}
