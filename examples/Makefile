all:
	set -e; \
	for i in */Makefile; do \
		echo '==== Making:' $$(dirname $$i); \
		$(MAKE) -C $$(dirname $$i); \
	done

# Don't 'set -e' because some of these tests are expected to fail
runtests:
	for i in */Makefile; do \
		echo '==== Running:' $$(dirname $$i); \
		$(MAKE) -C $$dir run; \
	done

clean:
	set -e; \
	for i in */Makefile; do \
		echo '==== Cleaning:' $$(dirname $$i); \
		$(MAKE) -C $$(dirname $$i) clean; \
	done
