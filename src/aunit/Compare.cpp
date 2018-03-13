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
  char bufa[4];
  char bufb[4];

  // Using uint8_t instead of size_t is not a bug because we only need the
  // least significant 2 bits of the counter. Rolling over the uint8_t counter
  // is ok too.
  for (uint8_t i = 0; true; i++) {
    if ((i & 0x03) == 0) {
      // NOTE: There ought to be a simpler, more efficient way of extracting 4
      // characters from flash memory the pointer is already 4-byte aligned.
      // But I don't know what it is right now.
      memcpy_P(bufa, aa, 4);
      memcpy_P(bufb, bb, 4);
      aa += 4;
      bb += 4;
    }
    // Avoid '%' operator since some 8-bit processors lack hardware division.
    char ca = bufa[i & 0x03];
    char cb = bufb[i & 0x03];
    if (ca < cb) return -1;
    if (ca > cb) return 1;
    // we hit this condition only if both strings were the same length
    if (ca == '\0' || cb == '\0') return 0;
  }
  return 0;
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
  char bufa[4];
  char bufb[4];
  for (size_t i = 0; i < n; i++) {
    if ((i & 0x03) == 0) {
      // NOTE: There ought to be a simpler, more efficient way of extracting 4
      // characters from flash memory the pointer is already 4-byte aligned.
      // But I don't know what it is right now.
      memcpy_P(bufa, aa, 4);
      memcpy_P(bufb, bb, 4);
      aa += 4;
      bb += 4;
    }
    char ca = bufa[i & 0x03];
    char cb = bufb[i & 0x03];
    if (ca < cb) return -1;
    if (ca > cb) return 1;
    // we hit this condition only if both strings were the same length
    if (ca == '\0' || cb == '\0') return 0;
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
