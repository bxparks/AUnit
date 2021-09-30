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

#include <Print.h>
#include "Compare.h"
#include "FCString.h"

namespace aunit {
namespace internal {

void FCString::print(Print* printer) const {
  if (mString.cstring == nullptr) return;

  if (mStringType == kCStringType) {
    printer->print(getCString());
  } else {
    printer->print(getFString());
  }
}

void FCString::println(Print* printer) const {
  if (mString.cstring == nullptr) {
    printer->println();
    return;
  }

  if (mStringType == kCStringType) {
    printer->println(getCString());
  } else {
    printer->println(getFString());
  }
}

int FCString::compareTo(const FCString& that) const {
  if (getType() == FCString::kCStringType) {
      if (that.getType() == FCString::kCStringType) {
        return compareString(getCString(), that.getCString());
      } else {
        return compareString(getCString(), that.getFString());
      }
  } else {
      if (that.getType() == FCString::kCStringType) {
        return compareString(getFString(), that.getCString());
      } else {
        return compareString(getFString(), that.getFString());
      }
  }
}

int FCString::compareToN(const char* that, size_t n) const {
  if (getType() == FCString::kCStringType) {
    return compareStringN(getCString(), that, n);
  } else {
    return compareStringN(getFString(), that, n);
  }
}

int FCString::compareToN(const __FlashStringHelper* that, size_t n) const {
  if (getType() == FCString::kCStringType) {
    return compareStringN(getCString(), that, n);
  } else {
    return compareStringN(getFString(), that, n);
  }
}

bool FCString::hasSubstring(const char* substring) const {
  if (getType() == FCString::kCStringType) {
    return compareSubstring(getCString(), substring);
  } else {
    return compareSubstring(getFString(), substring);
  }
}

}
}
