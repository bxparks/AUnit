#include <Print.h>
#include "print64.h"

// Copied and modified from Arduino AVR Core Print.cpp file.

namespace aunit {
namespace internal {

size_t printNumber64(Print& printer, unsigned long long n, int base) {
  if ((n & 0xFFFFFFFF00000000) == 0) {
    return printer.print((unsigned long) n, base);
  }

  char buf[8 * sizeof(unsigned long long) + 1];
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  return printer.write(str);
}

size_t print64(Print& printer, unsigned long long n, int base) {
  if (base == 0) {
    base = 10;
  }
  return printNumber64(printer, n, base);
}

size_t print64(Print& printer, long long n, int base) {
  if (base == 0) {
    base = 10;
  }
  if (base == 10) {
    if (n < 0) {
      size_t t = printer.write('-');
      return printNumber64(printer, -n, base) + t;
    } else {
      return printNumber64(printer, n, base);
    }
  } else {
    return printNumber64(printer, n, base);
  }
}

size_t println64(Print& printer, unsigned long long n, int base) {
  size_t len = print64(printer, n, base);
  return len + printer.println();
}

size_t println64(Print& printer, long long n, int base) {
  size_t len = print64(printer, n, base);
  return len + printer.println();
}

}
}

