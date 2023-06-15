# See https://github.com/bxparks/EpoxyDuino for documentation about using
# EpoxyDuino to compile and run AUnit tests natively on Linux or MacOS.

APP_NAME := MemoryBenchmark
ARDUINO_LIBS := AUnit
MORE_CLEAN := more_clean
include ../../../EpoxyDuino/EpoxyDuino.mk

.PHONY: benchmarks

TARGETS := nano.txt micro.txt samd21.txt stm32.txt samd51.txt \
	esp8266.txt esp32.txt

README.md: $(TARGETS) generate_readme.py generate_table.awk
	./generate_readme.py > $@

benchmarks: $(TARGETS)

nano.txt:
	./collect.sh --cli nano $@

micro.txt:
	./collect.sh --cli micro $@

samd21.txt:
	./collect.sh --cli xiao $@

stm32.txt:
	./collect.sh --cli stm32 $@

samd51.txt:
	./collect.sh --cli itsym4 $@

esp8266.txt:
	./collect.sh --cli nodemcu $@

esp32.txt:
	./collect.sh --cli esp32 $@

epoxy:
	./validate_using_epoxy_duino.sh

more_clean:
	echo "Use 'make clean_benchmarks' to remove *.txt files"

clean_benchmarks:
	rm -f $(TARGETS)
