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

#include <map>

#include <gtest/gtest.h>

class ContainerUtilsTest : public testing::Test{};

using namespace gamekeeper::utils;

TEST_F(ContainerUtilsTest, checkMissing_emptyContainer)
{
	std::map<std::string,std::string> emptyMap;
	std::vector<std::string> missingKeys{"missingKey"};
	EXPECT_EQ(missingKeys, Containers::checkMissing(emptyMap, missingKeys));
}

TEST_F(ContainerUtilsTest, checkMissing_mixed)
{
	std::map<std::string,std::string> emptyMap{{"existingKey", "string"}};
	std::vector<std::string> keysToSearch{"existingKey", "missingKey"};
	std::vector<std::string> missingKeys{"missingKey"};
	EXPECT_EQ(missingKeys, Containers::checkMissing(emptyMap, keysToSearch));
}
