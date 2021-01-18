/*
MIT License

Copyright (c) 2018 Brian T. Park

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef AUNIT_FAKE_PRINT_H
#define AUNIT_FAKE_PRINT_H

#include <stddef.h> // size_t
#include <Print.h>

namespace aunit {
namespace fake {

/**
 * An implementation of Print that writes to an in-memory buffer. The buffer
 * can be retrieved using getBuffer() to verify that the expected string was
 * written to the Print object.
 *
 * Usage:
 * @verbatim
 * {
 *   FakePrint fakePrint;
 *   object.printTo(fakePrint);
 *   assertEqual("expected", fakePrint.getBuffer());
 *   fakePrint.flush();
 *   ...
 * }
 * @endverbatim
 *
 * The internal buffer size is just big enough to hold a `long long` (8 bytes)
 * with 3 bytes to hold the `\\r\\n` from a `println()` statement and the
 * terminating NUL character.
 *
 * This class is an early version of the PrintStr class of the AceCommon
 * library. We don't want to change AUnit to use PrintStr instead because we
 * use AUnit to write the unit tests for AceCommon itself, and it would cause a
 * conceptual cyclic dependency. Also we want AUnit to be self-contained
 * without additional dependencies.
 */
class FakePrint: public Print {
  public:
    /**
     * Size of the internal buffer. This is just large enough to hold the
     * string representation of one 64-bit integer in binary format (64 bytes),
     * including 2 bytes for the '\\r\\n' from println() and 1 byte for the
     * terminating NUL.
     */
    static const uint8_t kBufSize = 8 * sizeof(long long) + 2 + 1;

    size_t write(uint8_t c) override {
      if (mIndex < kBufSize - 1) {
        mBuf[mIndex] = c;
        mIndex++;
        return 1;
      } else {
        return 0;
      }
    }

    size_t write(const uint8_t *buffer, size_t size) override {
      if (buffer == nullptr) return 0;

      while (size > 0 && mIndex < kBufSize - 1) {
        write(*buffer++);
        size--;
      }
      return size;
    }

// ESP32 and STM32duino do not provide a virtual Print::flush() method.
#if defined(ESP32) || defined(ARDUINO_ARCH_STM32)
    void flush() {
#else
    void flush() override {
#endif
      mIndex = 0;
    }

    /**
     * Return the NUL terminated string buffer. After the buffer is no longer
     * needed, the flush() method should be called to reset the internal buffer
     * index to 0.
     */
    const char* getBuffer() const {
      mBuf[mIndex] = '\0';
      return mBuf;
    }

  private:
    mutable char mBuf[kBufSize];
    uint8_t mIndex = 0;
};

}
}

#endif
