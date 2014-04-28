/*
 * GameKeeper Framework
 *
 * Copyright (C) 2013 Karol Herbst <gamekeeper@karolherbst.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <gamekeeper/utils/stringutils.h>

#include <cstdint>

#include <gtest/gtest.h>

class StringUtilsTest : public testing::Test{};

enum TEST_ENUM
{
	TEST_ENUM_TEST = 4
};

enum class TEST_ENUM_INT : int
{
	TEST = 5
};

enum class TEST_ENUM_CHAR : char
{
	TEST = 'E'
};

#define TO_STRING_TEST(name, type, value, exp_value) \
TEST_F(StringUtilsTest, toString_##name) \
{ \
        const type C = value; \
        EXPECT_EQ(exp_value, gamekeeper::utils::String::toString(C)); \
}

static constexpr char TEST_STRING[] = "test string";
static const std::string TEST_STRING_STRING = "test string";

TO_STRING_TEST(char, char, 'c', "c");
TO_STRING_TEST(uint8_t, uint8_t, 5, "5");
TO_STRING_TEST(int8_t, int8_t, 5, "5");
TO_STRING_TEST(uint16_t, uint16_t, 5, "5");
TO_STRING_TEST(int16_t, int16_t, 5, "5");
TO_STRING_TEST(uint32_t, uint32_t, 5, "5");
TO_STRING_TEST(int32_t, int32_t, 5, "5");
TO_STRING_TEST(uint64_t, uint64_t, 5, "5");
TO_STRING_TEST(int64_t, int64_t, 5, "5");
TO_STRING_TEST(float, float, 5.0f, "5");
TO_STRING_TEST(double, double, 5.0, "5");
TO_STRING_TEST(long_double, long double, 5.0, "5");
TO_STRING_TEST(string, std::string, TEST_STRING, TEST_STRING);
TO_STRING_TEST(string_from_string, std::string &, TEST_STRING_STRING, TEST_STRING_STRING);
TO_STRING_TEST(char_ptr, char * , TEST_STRING, TEST_STRING);
TO_STRING_TEST(ptr, void *, (void *)5, "0x5");
TO_STRING_TEST(enum, TEST_ENUM, TEST_ENUM_TEST, "4");
TO_STRING_TEST(enum_class_int, TEST_ENUM_INT, TEST_ENUM_INT::TEST, "5");
TO_STRING_TEST(enum_class_char, TEST_ENUM_CHAR, TEST_ENUM_CHAR::TEST, "E");
