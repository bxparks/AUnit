#ifndef AUNIT_FAKE_PRINT_H
#define AUNIT_FAKE_PRINT_H

#include <stddef.h> // size_t
#include <Print.h>

namespace aunit {
namespace fake {

/**
 * An implementation of Print that writes to an in-memory buffer.
 * The buffer can be retrieved for unit testing.
 */
class FakePrint: public Print {
  public:
    // +2 for \r\n in println(), +1 for NUL
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

    int availableForWrite() override {
      return kBufSize - 1 - mIndex;
    }

    void flush() override {
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
