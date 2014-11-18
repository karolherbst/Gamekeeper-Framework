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

#include <gamekeeper/utils/containerutils.h>

#include <deque>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <gtest/gtest.h>

class ContainerUtilsTest : public testing::Test{};

using namespace gamekeeper::utils;

using std::deque;
using std::list;
using std::map;
using std::multimap;
using std::multiset;
using std::set;
using std::unordered_map;
using std::unordered_multimap;
using std::unordered_multiset;
using std::unordered_set;
using std::vector;

#define TEST_MACRO_MAP(type, mapType) \
TEST_F(ContainerUtilsTest, checkMissing_emptyContainer_##type##_##mapType) \
{ \
	mapType<std::string,std::string> emptyMap; \
	type<std::string> missingKeys{"missingKey"}; \
	EXPECT_EQ(missingKeys, Containers::checkMissing(emptyMap, missingKeys)); \
} \
\
TEST_F(ContainerUtilsTest, checkMissing_mixed_##type##_##mapType) \
{ \
	mapType<std::string,std::string> emptyMap{{"existingKey", "string"}}; \
	type<std::string> keysToSearch{"existingKey", "missingKey"}; \
	type<std::string> missingKeys{"missingKey"}; \
	EXPECT_EQ(missingKeys, Containers::checkMissing(emptyMap, keysToSearch)); \
}

#define TEST_MACRO_SET(type, setType) \
TEST_F(ContainerUtilsTest, checkMissing_emptyContainer_##type##_##setType) \
{ \
	setType<std::string> emptyMap; \
	type<std::string> missingKeys{"missingKey"}; \
	EXPECT_EQ(missingKeys, Containers::checkMissing(emptyMap, missingKeys)); \
} \
\
TEST_F(ContainerUtilsTest, checkMissing_mixed_##type##_##setType) \
{ \
	setType<std::string> emptyMap{"existingKey"}; \
	type<std::string> keysToSearch{"existingKey", "missingKey"}; \
	type<std::string> missingKeys{"missingKey"}; \
	EXPECT_EQ(missingKeys, Containers::checkMissing(emptyMap, keysToSearch)); \
}

#define TEST_MACRO_EXEC(type) \
TEST_MACRO_MAP(type, map) \
TEST_MACRO_MAP(type, multimap) \
TEST_MACRO_MAP(type, unordered_map) \
TEST_MACRO_MAP(type, unordered_multimap) \
TEST_MACRO_SET(type, set) \
TEST_MACRO_SET(type, multiset) \
TEST_MACRO_SET(type, unordered_set) \
TEST_MACRO_SET(type, unordered_multiset) \

TEST_MACRO_EXEC(deque)
TEST_MACRO_EXEC(list)
TEST_MACRO_EXEC(vector)
