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

#ifndef AUNIT_PRINT_64_H
#define AUNIT_PRINT_64_H

#include <stddef.h> // size_t
#include <Print.h>

/**
 * @file print64.h
 *
 * Helper routines to print 'long long' and 'unsigned long long' because
 * the Print::print() methods in Print.h do not suport 64-bit integers.
 *
 * The division and mod operations for 'long long' types are CPU intensive and
 * consume significant amounts of flash memory (100s of bytes?) on an 8-bit
 * processors. I assume that these methods will not be used very often on 8-bit
 * processors.
 */

class Print;

namespace aunit {
namespace internal {

size_t printNumber64(Print& printer, unsigned long long n, int base = 10);

size_t print64(Print& printer, unsigned long long n, int base = 10);

size_t print64(Print& printer, long long n, int base = 10);

size_t println64(Print& printer, unsigned long long n, int base = 10);

size_t println64(Print& printer, long long n, int base = 10);

}
}

#endif
