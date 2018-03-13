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

#include <string.h>
#include "Compare.h"
#include "FCString.h"

// Flash memory must be read using 4-byte alignment on the ESP8266. AVR doesn't
// care. Teensy-ARM fakes the flash memory API but really just uses the normal
// static RAM. The following code will work for all 3 environments.

namespace aunit {

int compareString(const __FlashStringHelper* a, const __FlashStringHelper* b) {
  const char* aa = reinterpret_cast<const char*>(a);
  const char* bb = reinterpret_cast<const char*>(b);

  // On ESP8266, pgm_read_byte() already takes care of 4-byte alignment, and
  // memcpy_P(s, p, 4) makes 4 calls to pgm_read_byte() anyway, so don't bother
  // optimizing for 4-byte alignment here.
  while (true) {
    char ca = pgm_read_byte(aa);
    char cb = pgm_read_byte(bb);
    if (ca < cb) return -1;
    if (ca > cb) return 1;
    // we hit this condition only if both strings were the same length
    if (ca == '\0' || cb == '\0') return 0;
    aa++;
    bb++;
  }
}

int compareString(const FCString& a, const FCString& b) {
  if (a.getType() == FCString::kCStringType) {
      if (b.getType() == FCString::kCStringType) {
        return compareString(a.getCString(), b.getCString());
      } else {
        return compareString(a.getCString(), b.getFString());
      }
  } else {
      if (b.getType() == FCString::kCStringType) {
        return compareString(a.getFString(), b.getCString());
      } else {
        return compareString(a.getFString(), b.getFString());
      }
  }
}

int compareStringN(const __FlashStringHelper* a, const __FlashStringHelper* b,
    size_t n) {
  const char* aa = reinterpret_cast<const char*>(a);
  const char* bb = reinterpret_cast<const char*>(b);

  // On ESP8266, pgm_read_byte() already takes care of 4-byte alignment, and
  // memcpy_P(s, p, 4) makes 4 calls to pgm_read_byte() anyway, so don't bother
  // optimizing for 4-byte alignment here.
  while (n > 0) {
    char ca = pgm_read_byte(aa);
    char cb = pgm_read_byte(bb);
    if (ca < cb) return -1;
    if (ca > cb) return 1;
    // we hit this condition only if both strings were the same length
    if (ca == '\0' || cb == '\0') return 0;
    aa++;
    bb++;
    n--;
  }
  return 0;
}

int compareStringN(const FCString& a, const char* b, size_t n) {
  if (a.getType() == FCString::kCStringType) {
    return compareStringN(a.getCString(), b, n);
  } else {
    return compareStringN(a.getFString(), b, n);
  }
}

int compareStringN(const FCString& a, const __FlashStringHelper* b, size_t n) {
  if (a.getType() == FCString::kCStringType) {
    return compareStringN(a.getCString(), b, n);
  } else {
    return compareStringN(a.getFString(), b, n);
  }
}

}
