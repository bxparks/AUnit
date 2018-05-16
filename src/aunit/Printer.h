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

#ifndef AUNIT_PRINTER_H
#define AUNIT_PRINTER_H

class Print;

namespace aunit {

/**
 * Utility class that provides a level of indirection to the Print class where
 * test results can be sent. By default, the Print object will be the Serial
 * object. This can be changed using the setPrinter() method.
 *
 * This class assumes that it will be used after all static initializations have
 * finished. Because static initialization ordering is undefined, if this
 * utility is used during static initialization, the behaviour is undefined.
 */
class Printer {
  public:
    /**
     * Get the output printer used by the various assertion() methods and the
     * TestRunner. The default is the predefined Serial object. Can be changed
     * using the setPrinter() method.
     */
    static Print* getPrinter() { return sPrinter; }

    /** Set the printer. */
    static void setPrinter(Print* printer) { sPrinter = printer; }

  private:
    // Disable copy-constructor and assignment operator
    Printer(const Printer&) = delete;
    Printer& operator=(const Printer&) = delete;

    static Print* sPrinter;
};

}

#endif
