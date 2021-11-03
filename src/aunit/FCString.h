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

#include <stddef.h> // size_t

class Print;
class __FlashStringHelper;

namespace aunit {
namespace internal {

/**
 * A union of (const char*) and (const __FlashStringHelper*) with a
 * discriminator. This allows us to treat these 2 strings like a single object.
 * The major reason this class is needed is because the F() cannot be used
 * outside a function, it can only be used inside a function, so we are forced
 * to use normal c-strings instead of F() strings when manually creating Test or
 * TestOnce instances.
 *
 * I deliberately decided not to inherit from Printable. While it is convenient
 * to be able to call Print::print() with an instance of this class, the cost
 * is 2 bytes (8-bit processors) or 4 bytes (32-bit processors) of extra static
 * memory for the v-table pointer for each instance. But each instance is only
 * 3 bytes (8-bits) or 5 bytes (32-bits) big, so the cost of 50-100 bytes of
 * static memory for a large suite of 25 unit tests does not seem worth the
 * minor convenience.
 *
 * Use the print() and println() methods to print to the given 'Print'. In
 * hindsight, with more Arduino programming under my belt, I think these
 * functions should accept a reference `Print&` instead of a pointer `Print*`.
 * That is how I implemented this in a subsequent version of this class in the
 * AceCommon library.
 */
class FCString {
  public:
    static const uint8_t kCStringType = 0;
    static const uint8_t kFStringType = 1;

    /** Default constructor initializes to a nullptr of kCStringType. */
    FCString() {}

    /** Construct with a c-string. */
    explicit FCString(const char* s):
        mStringType(kCStringType) {
      mString.cstring = s;
    }

    /** Construct with a flash string. */
    explicit FCString(const __FlashStringHelper* s):
        mStringType(kFStringType) {
      mString.fstring = s;
    }

    /** Get the internal type of string. */
    uint8_t getType() const { return mStringType; }

    /** Get the c-string pointer. */
    const char* getCString() const { return mString.cstring; }

    /** Get the flash string pointer. */
    const __FlashStringHelper* getFString() const { return mString.fstring; }

    /** Convenience method for printing an FCString. */
    void print(Print* printer) const;
  
    /** Convenience method for printing an FCString. */
    void println(Print* printer) const;

    /** Compare to another FCString. */
    int compareTo(const FCString& that) const;

    /**
     * Compare to C-string using the first n characters.
     * This is expected to be used only for TestRunner::exclude() and
     * TestRunner::include().
     */
    int compareToN(const char* that, size_t n) const;

    /**
     * Compare to a flash string using the first n characters.
     * This is expected to be used only for TestRunner::exclude() and
     * TestRunner::include().
     */
    int compareToN(const __FlashStringHelper* that, size_t n) const;

    /** Determine if given substring exists. */
    bool hasSubstring(const char* substring) const;

  private:
    // NOTE: It might be possible just use a (void *) instead of a union.
    union {
      const char* cstring;
      const __FlashStringHelper* fstring;
    } mString = { nullptr };

    uint8_t mStringType = kCStringType;
};

}
}

#endif
