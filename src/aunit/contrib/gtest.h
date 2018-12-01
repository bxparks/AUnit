/*
MIT License

Copyright (c) 2018 Chris Johnson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/**
 * @file gtest.h
 *
 * A simple adapter which allows basic unit tests written using Google Test API
 * (https://github.com/google/googletest) to run on Arduino platforms using
 * AUnit.
 *
 * This is not a comprehensive mapping layer. Only the TEST()
 * macro is supported, TEST_F() is not supported. Many of the ASSERT_Xxx()
 * macros are missing. Over time, more mapping macros may be added.
 *
 * Usage:
 *
 * @code
 * #include <AUnit.h>
 * #include <aunit/contrib/gtest.h>
 * @endcode
 *
 * or
 *
 * @code
 * #include <AUnitVerbose.h>
 * #include <aunit/contrib/gtest.h>
 * @endcode
 */

#ifndef AUNIT_CONTRIB_GTEST_H
#define AUNIT_CONTRIB_GTEST_H

#define TEST(category, name) test(category##_##name)

#define ASSERT_EQ(e, a) assertEqual(static_cast<decltype(a)>(e), a)
#define ASSERT_NE(e, a) assertNotEqual(static_cast<decltype(a)>(e), a)
#define ASSERT_LT(e, a) assertLess(static_cast<decltype(a)>(e), a)
#define ASSERT_GT(e, a) assertMore(static_cast<decltype(a)>(e), a)
#define ASSERT_LE(e, a) assertLessOrEqual(static_cast<decltype(a)>(e), a)
#define ASSERT_GE(e, a) assertMoreOrEqual(static_cast<decltype(a)>(e), a)

#define ASSERT_STREQ(e, a) assertEqual(static_cast<decltype(a)>(e), a)
#define ASSERT_STRNE(e, a) assertNotEqual(static_cast<decltype(a)>(e), a)
#define ASSERT_STRCASEEQ(e, a) \
    assertStringCaseEqual(static_cast<decltype(a)>(e), a)
#define ASSERT_STRCASENE(e, a) \
    assertStringCaseNotEqual(static_cast<decltype(a)>(e), a)

#define ASSERT_TRUE(x) assertTrue(x)
#define ASSERT_FALSE(x) assertFalse(x)

#define ASSERT_NEAR(e, a, error) assertNear(static_cast<decltype(a)>(e), a, static_cast<decltype(a)>(error))

#endif
