#line 2 "SlowTests.cpp"

#include "AUnitTest.h"

testing(slow_pass) { if (millis() > 1000) pass(); }

testing(slow_fail) { if (millis() > 1000) fail(); }

testing(slow_skip) { if (millis() > 1000) skip(); }

#if USE_AUNIT == 1
testing(slow_expire) { if (millis() > 1000) expire(); }
#endif

