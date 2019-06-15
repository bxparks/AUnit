#include <unistd.h> // usleep()
#include <inttypes.h>
#include <time.h> // clock_gettime()

#include "Arduino.h"

void delay(unsigned long ms) {
  usleep(ms * 1000);
}

void yield() {}

unsigned long millis() {
  struct timespec spec;
  clock_gettime(CLOCK_MONOTONIC, &spec);
  unsigned long ms = spec.tv_sec * 1000 + spec.tv_nsec / 1000000;
  return ms;
}

void digitalWrite(uint8_t pin, uint8_t val) {}

void pinMode(uint8_t pin, uint8_t mode) {}

int main(int argc, char** argv) {
  setup();
  while (true) {
    loop();
  }
}
