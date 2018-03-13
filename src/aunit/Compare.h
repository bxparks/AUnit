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

// Significant portions of the design and implementation of this file came from
// https://github.com/mmurdoch/arduinounit/blob/master/src/ArduinoUnitUtility/Compare.h

#ifndef AUNIT_COMPARE_H
#define AUNIT_COMPARE_H

class String;
class __FlashStringHelper;
class FCString;

namespace aunit {

// compareString()

int compareString(const char* a, const char* b);

int compareString(const char* a, const String& b);

int compareString(const char* a, const __FlashStringHelper* b);

int compareString(const String& a, const char* b);

int compareString(const String& a, const String& b);

int compareString(const String& a, const __FlashStringHelper* b);

int compareString(const __FlashStringHelper* a, const char* b);

int compareString(const __FlashStringHelper* a, const __FlashStringHelper* b);

int compareString(const __FlashStringHelper* a, const String& b);

int compareString(const FCString& a, const FCString& b);

// compareStringN()
//
// These methods are used to implement the TestRunner::exclude() and
// TestRunner::include() features.

/** Compare only the first n characters of 'a' or 'b'. */
int compareStringN(const char* a, const char* b, size_t n);

/** Compare only the first n characters of 'a' or 'b'. */
int compareStringN(const char* a, const __FlashStringHelper* b, size_t n);

/** Compare only the first n characters of 'a' or 'b'. */
int compareStringN(const __FlashStringHelper* a, const char* b, size_t n);

/** Compare only the first n characters of 'a' or 'b'. */
int compareStringN(const __FlashStringHelper* a, const __FlashStringHelper* b,
    size_t n);

/** Compare only the first n characters of 'a' or 'b'. */
int compareStringN(const FCString& a, const char* b, size_t n);

/** Compare only the first n characters of 'a' or 'b'. */
int compareStringN(const FCString& a, const __FlashStringHelper* b, size_t n);

// compareEqual()

bool compareEqual(bool a, bool b);

bool compareEqual(char a, char b);

bool compareEqual(int a, int b);

bool compareEqual(unsigned int a, unsigned int b);

bool compareEqual(long a, long b);

bool compareEqual(unsigned long a, unsigned long b);

bool compareEqual(double a, double b);

bool compareEqual(const char* a, const char* b);

bool compareEqual(const char* a, const String& b);

bool compareEqual(const char* a, const __FlashStringHelper* b);

bool compareEqual(const __FlashStringHelper* a, const char* b);

bool compareEqual( const __FlashStringHelper* a, const __FlashStringHelper* b);

bool compareEqual(const __FlashStringHelper* a, const String& b);

bool compareEqual(const String& a, const char* b);

bool compareEqual(const String& a, const String& b);

bool compareEqual(const String& a, const __FlashStringHelper* b);

// compareLess()

bool compareLess(bool a, bool b);

bool compareLess(char a, char b);

bool compareLess(int a, int b);

bool compareLess(unsigned int a, unsigned int b);

bool compareLess(long a, long b);

bool compareLess(unsigned long a, unsigned long b);

bool compareLess(double a, double b);

bool compareLess(const char* a, const char* b);

bool compareLess(const char* a, const String& b);

bool compareLess(const char* a, const __FlashStringHelper* b);

bool compareLess(const __FlashStringHelper* a, const char* b);

bool compareLess(const __FlashStringHelper* a, const __FlashStringHelper* b);

bool compareLess(const __FlashStringHelper* a, const String& b);

bool compareLess(const String& a, const char* b);

bool compareLess(const String& a, const String& b);

bool compareLess(const String& a, const __FlashStringHelper* b);

// compareMore()

bool compareMore(bool a, bool b);

bool compareMore(char a, char b);

bool compareMore(int a, int b);

bool compareMore(unsigned int a, unsigned int b);

bool compareMore(long a, long b);

bool compareMore(unsigned long a, unsigned long b);

bool compareMore(double a, double b);

bool compareMore(const char* a, const char* b);

bool compareMore(const char* a, const String& b);

bool compareMore(const char* a, const __FlashStringHelper* b);

bool compareMore(const __FlashStringHelper* a, const char* b);

bool compareMore(const __FlashStringHelper* a, const __FlashStringHelper* b);

bool compareMore(const __FlashStringHelper* a, const String& b);

bool compareMore(const String& a, const char* b);

bool compareMore(const String& a, const String& b);

bool compareMore(const String& a, const __FlashStringHelper* b);

// compareLessOrEqual

bool compareLessOrEqual(bool a, bool b);

bool compareLessOrEqual(char a, char b);

bool compareLessOrEqual(int a, int b);

bool compareLessOrEqual(unsigned int a, unsigned int b);

bool compareLessOrEqual(long a, long b);

bool compareLessOrEqual(unsigned long a, unsigned long b);

bool compareLessOrEqual(double a, double b);

bool compareLessOrEqual(const char* a, const char* b);

bool compareLessOrEqual(const char* a, const String& b);

bool compareLessOrEqual(const char* a, const __FlashStringHelper* b);

bool compareLessOrEqual(const __FlashStringHelper* a, const char* b);

bool compareLessOrEqual(
    const __FlashStringHelper* a, const __FlashStringHelper* b);

bool compareLessOrEqual(const __FlashStringHelper* a, const String& b);

bool compareLessOrEqual(const String& a, const char* b);

bool compareLessOrEqual(const String& a, const String& b);

bool compareLessOrEqual(const String& a, const __FlashStringHelper* b);

// compareMoreOrEqual

bool compareMoreOrEqual(bool a, bool b);

bool compareMoreOrEqual(char a, char b);

bool compareMoreOrEqual(int a, int b);

bool compareMoreOrEqual(unsigned int a, unsigned int b);

bool compareMoreOrEqual(long a, long b);

bool compareMoreOrEqual(unsigned long a, unsigned long b);

bool compareMoreOrEqual(double a, double b);

bool compareMoreOrEqual(const char* a, const char* b);

bool compareMoreOrEqual(const char* a, const String& b);

bool compareMoreOrEqual(const char* a, const __FlashStringHelper* b);

bool compareMoreOrEqual(const __FlashStringHelper* a, const char* b);

bool compareMoreOrEqual(
    const __FlashStringHelper* a, const __FlashStringHelper* b);

bool compareMoreOrEqual(const __FlashStringHelper* a, const String& b);

bool compareMoreOrEqual(const String& a, const char* b);

bool compareMoreOrEqual(const String& a, const String& b);

bool compareMoreOrEqual(const String& a, const __FlashStringHelper* b);

// compareNotEqual

bool compareNotEqual(bool a, bool b);

bool compareNotEqual(char a, char b);

bool compareNotEqual(int a, int b);

bool compareNotEqual(unsigned int a, unsigned int b);

bool compareNotEqual(long a, long b);

bool compareNotEqual(unsigned long a, unsigned long b);

bool compareNotEqual(double a, double b);

bool compareNotEqual(const char* a, const char* b);

bool compareNotEqual(const char* a, const String& b);

bool compareNotEqual(const char* a, const __FlashStringHelper* b);

bool compareNotEqual(const __FlashStringHelper* a, const char* b);

bool compareNotEqual(
    const __FlashStringHelper* a, const __FlashStringHelper* b);

bool compareNotEqual(const __FlashStringHelper* a, const String& b);

bool compareNotEqual(const String& a, const char* b);

bool compareNotEqual(const String& a, const String& b);

bool compareNotEqual(const String& a, const __FlashStringHelper* b);

}

#endif
