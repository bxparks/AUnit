#include <Arduino.h>
#include "Printer.h"

namespace aunit {

Print* Printer::sPrinter = &Serial;

}
