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

#include <gamekeeper/core/common.h>
#include <gamekeeper/core/linuxinformation.h>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <gtest/gtest.h>

using namespace gamelib::core;
using namespace boost::filesystem;

class OSINFORMATIONCLASSTEST : public testing::Test
{
protected:
	virtual void SetUp() override
	{
		this->osInfo = new OSINFORMATIONCLASS();
	}

	virtual void TearDown() override
	{
		delete this->osInfo;
	}

	OSInformation * osInfo = nullptr;
};

TEST_F(OSINFORMATIONCLASSTEST, getEnvNotEmpty)
{
	std::string home = osInfo->getEnv("HOME");
	EXPECT_FALSE(home.empty());
}

TEST_F(OSINFORMATIONCLASSTEST, getEnvHomeReturnsFile)
{
	path homePath(osInfo->getEnv("HOME"));
	EXPECT_TRUE(exists(homePath));
	EXPECT_TRUE(is_directory(homePath));
}

TEST_F(OSINFORMATIONCLASSTEST, getEnvConsistency)
{
	// test that the stored char is not the same
	std::string home = osInfo->getEnv("HOME");
	std::string path = osInfo->getEnv("PATH");

	EXPECT_NE(home, path);
}

TEST_F(OSINFORMATIONCLASSTEST, setEnv)
{
	constexpr const char * testValue = "dfjakldj";
	// test doesn't make sense, if the stored variable actually equals our test
	EXPECT_NE(osInfo->getEnv(testValue), testValue);
	osInfo->setEnv(testValue, testValue);
	EXPECT_EQ(osInfo->getEnv(testValue), testValue);
}
