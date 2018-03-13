#include <string.h>
#include "Compare.h"
#include "FCString.h"

namespace aunit {

// Flash memory must be read using 4-byte alignment on the ESP8266. AVR doesn't
// care. Teensy-ARM fakes the flash memory API but really just uses the normal
// static RAM. The following code will work for all 3 environments.

int compareString(
    const __FlashStringHelper* a, const __FlashStringHelper* b) {
  const char* aa = reinterpret_cast<const char*>(a);
  const char* bb = reinterpret_cast<const char*>(b);
  char bufa[4];
  char bufb[4];
  for (uint8_t i = 0; true; i++) {
    if ((i & 0x03) == 0) {
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

// Not really used anywhere right now, so the linker will
// remove it. But I think I might needit in the future.
int compareStringN(const __FlashStringHelper* a,
    const __FlashStringHelper* b, uint16_t n) {
  const char* aa = reinterpret_cast<const char*>(a);
  const char* bb = reinterpret_cast<const char*>(b);
  char bufa[4];
  char bufb[4];
  for (uint16_t i = 0; i < n; i++) {
    if ((i & 0x03) == 0) {
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

int compareStringN(const __FlashStringHelper* a, const char* b, uint16_t n) {
  const char* aa = reinterpret_cast<const char*>(a);
  char bufa[4];
  for (uint16_t i = 0; i < n; i++) {
    if ((i & 0x03) == 0) {
      memcpy_P(bufa, aa, 4);
      aa += 4;
    }
    char ca = bufa[i & 0x03];
    char cb = b[i];
    if (ca < cb) return -1;
    if (ca > cb) return 1;
    // we hit this condition only if both strings were the same length
    if (ca == '\0' || cb == '\0') return 0;
  }
  return 0;
}

int compareStringN(const FCString& a, const __FlashStringHelper* b,
    uint16_t n) {
  if (a.getType() == FCString::kCStringType) {
    return compareStringN(a.getCString(), b, n);
  } else {
    return compareStringN(a.getFString(), b, n);
  }
}

int compareStringN(const FCString& a, const char* b, uint16_t n) {
  if (a.getType() == FCString::kCStringType) {
    return compareStringN(a.getCString(), b, n);
  } else {
    return compareStringN(a.getFString(), b, n);
  }
}

}
