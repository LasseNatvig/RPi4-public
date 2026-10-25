// unity.h - Unity test framework header - Example code for TDT4258 - 2026 edition
// Initially written by Cline and MistralMedium

#ifndef UNITY_H
#define UNITY_H

#include <stdint.h>

void unityTestStart(const char* file, uint32_t line);
void unityTestEnd(void);
void unityFail(const char* msg);
void unityIgnore(void);
int unityGetFailCount(void);
int unityGetTestCount(void);
int unityGetIgnoreCount(void);

#define TEST_ASSERT_EQUAL(a, b) if ((a) != (b)) { unityFail("Assert Equal Failed"); return; }
#define TEST_ASSERT_NOT_EQUAL(a, b) if ((a) == (b)) { unityFail("Assert Not Equal Failed"); return; }
#define TEST_ASSERT_EQUAL_INT(a, b) TEST_ASSERT_EQUAL((int)(a), (int)(b))
#define TEST_ASSERT_TRUE(a) if (!(a)) { unityFail("Assert True Failed"); return; }
#define TEST_ASSERT_FALSE(a) if ((a)) { unityFail("Assert False Failed"); return; }
#define TEST_ASSERT_EQUAL_STRING(a, b) if (strcmp((a),(b)) != 0) { unityFail("Assert String Equal Failed"); return; }

#define TEST_CASE(name) void name(void)
#define RUN_TEST(func) unityTestStart(__FILE__, __LINE__); func(); unityTestEnd()

#endif