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

#ifndef AUNIT_FSTRING_H
#define AUNIT_FSTRING_H

#include <stdint.h>

class __FlashStringHelper;

namespace aunit {

/**
 * A union of (const char*) and (const __FlashStringHelper*) with a
 * discriminator. This allows us to treat these 2 strings like a single object.
 * The major reason this class is needed is because the F() cannot be used
 * outside a function, it can only be used inside a function, so we are forced
 * to use normal c-strings instead of F() strings when manually creating Test or
 * TestOnce instances.
 */
class FCString {
  public:
    static const uint8_t kCStringType = 0;
    static const uint8_t kFStringType = 1;

    explicit FCString(const char* s):
        mStringType(kCStringType) {
      mString.cstring = s;
    }

    explicit FCString(const __FlashStringHelper* s):
        mStringType(kFStringType) {
      mString.fstring = s;
    }

    uint8_t getType() const { return mStringType; }

    const char* getCString() const { return mString.cstring; }

    const __FlashStringHelper* getFString() const { return mString.fstring; }
  
  private:
    union {
      const char* cstring;
      const __FlashStringHelper* fstring;
    } mString;

    const uint8_t mStringType;
};

}

#endif
