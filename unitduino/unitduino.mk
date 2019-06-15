# Include this Makefile to compile an Arduino *.ino file on Linux or MacOS.
#
# Create a 'Makefile' in the sketch folder. For example, for the
# Blink/Blink.ino program, the makefile will be 'Blink/Makefile'.
# The content will look like this:
#
#      APP_NAME := {name of *.ino file}
#      ARDUINO_LIBS := AceTime {... additional Arduino libraries}
#      include ../../../AUnit/unitduino/unitduino.mk
#
# The 2 parameters are:
#   * 'APP_NAME': base name of the Arduino sketch file,
#     e.g. 'Blink' not 'Blink.ino'
#   * 'ARDUINO_LIBS': list of dependent Arduino libraries.
#     The unitduino directory and the AUnit library are automatically
#     included.
#
# Type 'make -n' to verify.
#
# Type 'make' to create the $(APP_NAME).out program.

# Detect Linux or MacOS
UNAME := $(shell uname)

# Unitduino module directory.
UNITDUINO_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))

# If ARDUINO_LIB_DIR is not defined, assume that it's 2 directories
# above the unitduino/ directory.
ARDUINO_LIB_DIR ?= $(realpath $(UNITDUINO_DIR)/../..)

# Default modules which are automatically linked in: AUnit and AUnit/unitduino.
DEFAULT_MODULES := $(UNITDUINO_DIR) ${ARDUINO_LIB_DIR}/AUnit

# Application Modules as specified by the application's ARDUINO_LIBS variable.
APP_MODULES := $(foreach lib,$(ARDUINO_LIBS),${ARDUINO_LIB_DIR}/${lib})

# All dependent modules.
ALL_MODULES := $(DEFAULT_MODULES) $(APP_MODULES)

# Compiler and settings
ifeq ($(UNAME), Linux)
CXX ?= g++
CXXFLAGS ?= -Wall -std=gnu++11 -fno-exceptions -fno-threadsafe-statics -flto
else ifeq ($(UNAME), Darwin)
CXX ?= clang++
CXXFLAGS ?= -std=c++11 -stdlib=libc++ # -Weverything
endif

# pre-processor (-I, -D, etc)
CPPFLAGS_EXPANSION = -I$(module) -I$(module)/src
CPPFLAGS ?=
CPPFLAGS += $(foreach module,$(ALL_MODULES),$(CPPFLAGS_EXPANSION))

# linker settings (e.g. -lm)
LDFLAGS ?=

# C++ srcs. Support subdirectory expansions up to 3 levels below 'src/'.
# (There might be a better way to do this using GNU Make but I can't find a
# mechanism that doesn't barf when the 'src/' directory doesn't exist.)
SRCS_EXPANSION = $(wildcard $(module)/*.cpp) \
	$(wildcard $(module)/src/*.cpp) \
	$(wildcard $(module)/src/*/*.cpp) \
	$(wildcard $(module)/src/*/*/*.cpp) \
	$(wildcard $(module)/src/*/*/*/*.cpp)
SRCS := $(foreach module,$(ALL_MODULES),$(SRCS_EXPANSION))
SRCS := ${SRCS} $(wildcard *.cpp) $(wildcard */*.cpp)

# Objects including *.o from *.ino
OBJS := $(SRCS:%.cpp=%.o) $(APP_NAME).o

$(APP_NAME).out: $(OBJS)
	$(CXX) $(LDFLAGS) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^

$(APP_NAME).o: $(APP_NAME).ino
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -x c++ -c $<

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# This simple rule does not capture all header dependencies of a given cpp
# file. Maybe it's better to make each cpp to depend on all headers of a given
# module, and force a recompilation of all cpp files. As far as I understand,
# this is what the Arduino IDE does upon each compile iteration.
%.cpp: %.h

all: $(APP_NAME).out

.PHONY: clean

clean:
	rm -f $(OBJS) $(APP_NAME).out
