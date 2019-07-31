/*
Copyright (c) 2019 Brian T. Park

Parts derived from the Arduino SDK
Copyright (c) 2005-2013 Arduino Team

Parts from [Entering raw
mode](https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html).

The 'Serial' object sends output to STDOUT, and receives input from STDIN in
'raw' mode. The main() loop checks the STDIN and if it finds a character,
inserts it into the Serial buffer.
*/

#include <inttypes.h>
#include <unistd.h> // usleep()
#include <signal.h> // SIGINT
#include <termios.h>
#include <time.h> // clock_gettime()
#include <stdlib.h> // exit()
#include <stdio.h> // perror()
#include "Arduino.h"

// -----------------------------------------------------------------------
// Arduino methods emulated in Unix
// -----------------------------------------------------------------------

void delay(unsigned long ms) {
  usleep(ms * 1000);
}

void yield() {
  usleep(1000);
}

unsigned long millis() {
  struct timespec spec;
  clock_gettime(CLOCK_MONOTONIC, &spec);
  unsigned long ms = spec.tv_sec * 1000U + spec.tv_nsec / 1000000UL;
  return ms;
}

unsigned long micros() {
  struct timespec spec;
  clock_gettime(CLOCK_MONOTONIC, &spec);
  unsigned long us = spec.tv_sec * 1000000UL + spec.tv_nsec / 1000U;
  return us;
}

void digitalWrite(uint8_t pin, uint8_t val) {}

int digitalRead(uint8_t pin) { return 0; }

void pinMode(uint8_t pin, uint8_t mode) {}

// -----------------------------------------------------------------------
// Additional Unix stuff to put STDIN into raw mode, and trap Ctrl-C
// -----------------------------------------------------------------------

static struct termios orig_termios;
static bool inRawMode = false;

void die(const char* s) {
	perror(s);
  exit(1);
}

void disableRawMode() {
  if (!inRawMode) return;
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
    inRawMode = false; // prevent exit(1) from being called twice
		die("disableRawMode(): tcsetattr() failure");
	}
}

void enableRawMode() {
  if (!isatty(STDIN_FILENO)) {
    die("enableRawMode(): redirection on STDIN not supported");
  }
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
    die("enableRawMode(): tcgetattr() failure");
	}

  struct termios raw = orig_termios;
  // The 'Enter' key in raw mode is ^M (\r, CR). But internally, we want this
  // to be ^J (\n, NL), so ICRNL and INLCR causes the ^M to become a \n.
  raw.c_iflag &= ~(/*ICRNL | INLCR |*/ INPCK | ISTRIP | IXON);
  // Set the output into cooked mode, to handle NL and CR properly.
  // Print.println() sends CR-NL (\r\n). But some code will send just \n. The
  // ONLCR translates \n into \r\n. So '\r\n' will become \r\r\n, which is just
  // fine.
  raw.c_oflag |= (OPOST | ONLCR);
  raw.c_cflag |= (CS8);
  // Enable ISIG to allow Ctrl-C to kill the program.
  raw.c_lflag &= ~(/*ECHO | ISIG |*/ ICANON | IEXTEN);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 0;
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
    die("enableRawMode(): tcsetattr() failure");
  }
  inRawMode = true;
}

void handleControlC(int /*sig*/) {
  if (inRawMode) {
    // If this returns an error, don't call die() because it will call exit(),
    // which may call this again, causing an infinite recursion.
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
      perror("handleControlC(): tcsetattr() failure");
    }
    inRawMode = false;
  }
  exit(1);
}


// -----------------------------------------------------------------------
// Main loop
// -----------------------------------------------------------------------

int main(int argc, char** argv) {
  signal(SIGINT, handleControlC);
  atexit(disableRawMode);
	enableRawMode();

  setup();
  while (true) {
		char c = '\0';
    read(STDIN_FILENO, &c, 1);
		if (c) Serial.insertChar(c);
    loop();
    yield();
  }
}
