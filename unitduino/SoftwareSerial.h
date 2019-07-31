/**
 * @file SoftwareSerial.h
 *
 * A version of SoftwareSerial that supports AUnit tests on Linux or MaxOS.
 */

#ifndef UNITDUINO_SOFTWARE_SERIAL_H
#define UNITDUINO_SOFTWARE_SERIAL_H

#include "Print.h"
#include "Stream.h"

// Ring buffer size (should be a power of 2 for efficiency).
#define SOFTWARE_SERIAL_BUF_SIZE 128

class SoftwareSerial: public Stream {
  public:
    void begin(unsigned long baud) { }

    size_t write(uint8_t c) override;

    operator bool() { return true; }

    int available() override { return mHead != mTail; }

    int read() override {
      if (mHead == mTail) {
        return -1;
      } else {
        char c = mBuffer[mHead];
        mHead = (mHead + 1) % SOFTWARE_SERIAL_BUF_SIZE;
        return c;
      }
    }

    int peek() override {
      return (mHead != mTail) ? mBuffer[mHead] : -1;
    }

    /** Insert a character into the ring buffer. */
    void insertChar(char c) {
      int newTail = (mTail + 1) % SOFTWARE_SERIAL_BUF_SIZE;
      if (newTail == mHead) {
        // Buffer full, drop the character. (Strictly speaking, there's one
        // remaining slot in the buffer, but we can't use it because we need to
        // distinguish between buffer-empty and buffer-full).
        return;
      }
      mBuffer[mTail] = c;
      mTail = newTail;
    }

  private:
    char mBuffer[SOFTWARE_SERIAL_BUF_SIZE];
    int mHead = 0;
    int mTail = 0;
};

extern SoftwareSerial Serial;

#endif
