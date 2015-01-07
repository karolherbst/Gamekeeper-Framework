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

#include <gamekeeper/core/network/cookie.h>
#include <gamekeeper/core/network/filedownloader.h>

#include "defaultfixture.h"

using namespace gamekeeper::core;

class FileDownloaderTest : public testing::Test{};

static Cookie DEFAULT_COOKIE("", "", "");

TEST_F(FileDownloaderTest, cookieEqualAssign)
{
	Cookie c = DEFAULT_COOKIE;
	ASSERT_EQ(DEFAULT_COOKIE, c);
}

TEST_F(FileDownloaderTest, cookieEqualConstruct)
{
	Cookie c1("", "", "");
	Cookie c2("", "", "");
	ASSERT_EQ(c1, c2);
}

TEST_F(FileDownloaderTest, cookieEqualFullConstructed)
{
	Cookie c1("cName", "cValue", "c.value.org", "/test", 500, true);
	Cookie c2("cName", "cValue", "c.value.org", "/test", 500, true);
	ASSERT_EQ(c1, c2);
}

TEST_F(FileDownloaderTest, cookieEqualDomainDot)
{
	Cookie c1("cName", "cValue", ".domain");
	Cookie c2("cName", "cValue", "domain");
	ASSERT_EQ(c1, c2);
	ASSERT_EQ(c2, c1);
}

TEST_F(FileDownloaderTest, cookieNotEqual)
{
	Cookie c1("", "", "");
	Cookie c2("hi", "", "");
	ASSERT_NE(c1, c2);
}

TEST_F(FileDownloaderTest, cookieRFC6256_5_2_L1_4_AND_5_2_L2_5)
{
	Cookie c("   testName   ", "   testValue   ", "");
	ASSERT_EQ("testName", c.name);
	ASSERT_EQ("testValue", c.value);
}

TEST_F(FileDownloaderTest, cookieRFC6256_5_2_3)
{
	Cookie c("", "", ".doMaIn");
	ASSERT_EQ("domain", c.domain);
}

TEST_F(FileDownloaderTest, cookieRFC6256_5_2_4)
{
	Cookie c1("", "", "", "");
	ASSERT_EQ("/", c1.path);

	Cookie c2("", "", "", "hsd");
	ASSERT_EQ("/", c2.path);

	Cookie c3("", "", "", "/DjhkdasDTest");
	ASSERT_EQ("/DjhkdasDTest", c3.path);
}
