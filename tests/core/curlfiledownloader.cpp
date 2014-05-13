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

#include <exception>
#include <sstream>

#include <gamekeeper/core/curlfiledownloader.h>
#include <gamekeeper/core/log4cpploggerFactory.h>
#include <gamekeeper/utils/stringutils.h>

#include "webserverfixture.h"

using namespace gamekeeper::core;
using namespace gamekeeper::utils;

#if defined(GCC_VERSION) && (GCC_VERSION < 40704 || GCC_VERSION == 40800)
# define CURL_DOWNLOAD_TEST_CAPTURE &,&ex
#else
# define CURL_DOWNLOAD_TEST_CAPTURE &
#endif

#define CURL_DOWNLOAD_TEST(url, body) \
{ \
	bool handled = false; \
	std::exception_ptr ex; \
	this->fileDownloader->downloadFile(url, [CURL_DOWNLOAD_TEST_CAPTURE](FileDownloader::ByteIstream & is) -> bool \
	{ \
		try { body } catch (...) { ex = std::current_exception(); } \
	}); \
	EXPECT_TRUE(handled); \
	if(ex) std::rethrow_exception(ex); \
}


class CurlFiledownloaderTest : public gamekeeper::test::WebServerFicture
{
protected:
	HttpFileDownloader * fileDownloader = nullptr;

	virtual void SetUp() override
	{
		this->fileDownloader = new CurlFileDownloader(this->container->resolve<LoggerFactory>(),
		                                              this->container->resolve<PropertyResolver>(),
		                                              this->container->resolve<OSPaths>());
	}

	virtual void TearDown() override
	{
		delete this->fileDownloader;
	}
};

TEST_F(CurlFiledownloaderTest, loadEmptyFile)
{
	CURL_DOWNLOAD_TEST("http://localhost:8080/files/emptyfile",
	{
		const std::string data = String::createFromStream(is);
		EXPECT_EQ("", data);
		handled = true;
		return true;
	});
}

TEST_F(CurlFiledownloaderTest, servertest)
{
	CURL_DOWNLOAD_TEST("http://localhost:8080/files/fileWithContentHAHa",
	{
		const std::string data = String::createFromStream(is);
		EXPECT_LT(4, data.size());
		// cut line ending
		EXPECT_EQ("HAHa", data.substr(0, 4));
		handled = true;
		return true;
	});
}

TEST_F(CurlFiledownloaderTest, cookieTest)
{
	bool handled = false;
	HttpFileDownloader::CookieBuket cb = this->fileDownloader->doPostRequestForCookies("http://localhost:8080/cookies/type/value");
	EXPECT_EQ("value", cb["type"]);
}

TEST_F(CurlFiledownloaderTest, bigFile)
{
	// set buffer size to 0 to disable buffering at all
	setProperty("network.download", (uint64_t)0);
	CURL_DOWNLOAD_TEST("http://localhost:8080/bigfile/5000",
	{
		std::string data = String::createFromStream(is);
		EXPECT_EQ(5000, data.size());
		handled = true;
		return true;
	});
}
