PASSING_TESTS := AUnitMetaTest \
AUnitMoreTest \
AUnitTest \
FilterTest \
Print64Test

FAILING_TESTS := FailingTest \
SetupAndTeardownTest

tests:
	set -e; \
	for dir in $(PASSING_TESTS) $(FAILING_TESTS); do \
		$(MAKE) -C $$dir; \
	done

# $ make runtests | grep failed
runtests:
	set -e; \
	for dir in $(PASSING_TESTS); do \
		echo '==== Running:' $$dir; \
		$(MAKE) -C $$dir run; \
	done

# These contain purposefully failing tests, so must be manually verified to run
# and fail in the expected way.
# $ make run_manual_tests
runfailingtests:
	for dir in $(FAILING_TESTS); do \
		echo '==== Running:' $$dir; \
		$(MAKE) -C $$dir run; \
	done

clean:
	set -e; \
	for dir in $(PASSING_TESTS) $(FAILING_TESTS); do \
		echo $$dir; \
		$(MAKE) -C $$dir clean; \
	done
