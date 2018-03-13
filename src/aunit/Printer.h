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
