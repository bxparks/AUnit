#ifndef AUNIT_VERBOSITY_H
#define AUNIT_VERBOSITY_H

namespace aunit {

/**
 * Utility class to hold the Verbosity constants. Current used only by
 * TestRunner but potentially could be used by Test and TestOnce in the future,
 * so it seemed better to pull these out into a separate file.
 */
class Verbosity {
  public:
    static const uint8_t kTestRunSummary = 0x01;
    static const uint8_t kTestFailed = 0x02;
    static const uint8_t kTestPassed = 0x04;
    static const uint8_t kTestSkipped = 0x08;
    static const uint8_t kTestAll = 0x0F;
    static const uint8_t kAssertionFailed = 0x10;
    static const uint8_t kAssertionPassed = 0x20;
    static const uint8_t kAssertionAll = 0x30;
    static const uint8_t kDefault = (
        kAssertionFailed | kTestAll);
    static const uint8_t kAll = 0xFF;
    static const uint8_t kNone = 0x00;

  private:
    // Disable constructor, copy-constructor and assignment operator
    Verbosity() = delete;
    Verbosity(const Verbosity&) = delete;
    Verbosity& operator=(const Verbosity&) = delete;
};

}

#endif
