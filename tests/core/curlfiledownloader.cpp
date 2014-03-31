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
#include <gamekeeper/core/curlfiledownloader.h>
#include <gamekeeper/core/log4cpploggerFactory.h>

#include "webserverfixture.h"

using namespace gamekeeper::core;

class CurlFiledownloaderTest : public gamekeeper::test::WebServerFicture
{
public:
	WEBSERVERFICTUREMETHODSDEF
protected:
	HttpFileDownloader * fileDownloader = nullptr;

	virtual void SetUp() override
	{
		this->fileDownloader = new CurlFileDownloader(this->container->resolve<LoggerFactory>());
	}

	virtual void TearDown() override
	{
		delete this->fileDownloader;
	}
};

WEBSERVERFICTUREMETHODSIMPL(CurlFiledownloaderTest)

TEST_F(CurlFiledownloaderTest, loadEmptyFile)
{
	bool handled = false;
	this->fileDownloader->downloadFile("http://localhost:8080/files/emptyfile",
	                                   [&](void * const buffer, size_t bufferSize) -> bool
	{
		EXPECT_STREQ("", static_cast<const char*>(buffer));
		handled = true;
		return true;
	});
	EXPECT_TRUE(handled);
}

TEST_F(CurlFiledownloaderTest, servertest)
{
	bool handled = false;
	this->fileDownloader->downloadFile("http://localhost:8080/files/fileWithContentHAHa",
	                                   [&](void * const buffer, size_t bufferSize) -> bool
	{
		EXPECT_LT(4, bufferSize);
		// cut line ending
		std::string value(static_cast<const char*>(buffer), 4);
		EXPECT_EQ("HAHa", value);
		handled = true;
		return true;
	});
	EXPECT_TRUE(handled);
}

TEST_F(CurlFiledownloaderTest, cookieTest)
{
	bool handled = false;
	HttpFileDownloader::CookieBuket cb = this->fileDownloader->doPostRequestForCookies("http://localhost:8080/cookies/type/value");
	EXPECT_EQ("value", cb["type"]);
}
