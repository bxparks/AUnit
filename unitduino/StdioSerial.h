/*
 * Copyright (c) 2019 Brian T. Park
 * MIT License
 */

#ifndef UNITDUINO_STDIO_SERIAL_H
#define UNITDUINO_STDIO_SERIAL_H

#include "Print.h"
#include "Stream.h"

/**
 * A version of Serial that reads from STDIN and sends output to STDOUT on
 * Linux or MacOS (untested).
 */
class StdioSerial: public Stream {
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
        mHead = (mHead + 1) % kBufSize;
        return c;
      }
    }

    int peek() override {
      return (mHead != mTail) ? mBuffer[mHead] : -1;
    }

    /** Insert a character into the ring buffer. */
    void insertChar(char c) {
      int newTail = (mTail + 1) % kBufSize;
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
    // Ring buffer size (should be a power of 2 for efficiency).
    static const int kBufSize = 128;

    char mBuffer[kBufSize];
    int mHead = 0;
    int mTail = 0;
};

extern StdioSerial Serial;

#endif
