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

#include <std_compat/thread>

#include <gamekeeper/core/curlfiledownloaderfactory.h>
#include <gamekeeper/core/filedownloader.h>
#include <gamekeeper/core/log4cpploggerFactory.h>
#include <gamekeeper/core/propertyresolver.h>
#include <gamekeeper/core/userpaths.h>
#include <gamekeeper/utils/stringutils.h>

#include "defaultfixture.h"
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
	this->fileDownloaderFactory->create()->getRequest(url, [CURL_DOWNLOAD_TEST_CAPTURE](FileDownloader::ByteIstream & is) -> bool \
	{ \
		try { body } catch (...) { ex = std::current_exception(); } \
	}); \
	EXPECT_TRUE(handled); \
	if(ex) std::rethrow_exception(ex); \
}

class CurlFileDownloaderTest : public gamekeeper::test::DefaultFicture
{
protected:
	FileDownloaderFactory * fileDownloaderFactory = nullptr;

	virtual void SetUp() override
	{
		this->fileDownloaderFactory = new CurlFileDownloaderFactory(this->container->resolve<LoggerFactory>(),
		                                                            this->container->resolve<PropertyResolver>(),
		                                                            this->container->resolve<UserPaths>());
	}

	virtual void TearDown() override
	{
		delete this->fileDownloaderFactory;
	}
};

class CurlFileDownloaderServerTest : public CurlFileDownloaderTest, public gamekeeper::test::WebServerFictureAspect
{
public:
	CurlFileDownloaderServerTest()
	:	CurlFileDownloaderTest(),
		WebServerFictureAspect(this->container){}
};

TEST_F(CurlFileDownloaderServerTest, loadEmptyFile)
{
	CURL_DOWNLOAD_TEST("http://localhost:8080/files/emptyfile",
	{
		const std::string data = String::createFromStream(is);
		EXPECT_EQ("", data);
		handled = true;
		return true;
	});
}

TEST_F(CurlFileDownloaderServerTest, servertest)
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

TEST_F(CurlFileDownloaderServerTest, cookieTest)
{
	bool handled = false;
	auto fd = this->fileDownloaderFactory->create();
	fd->postRequest("http://localhost:8080/cookies/type/value");
	FileDownloader::CookieBucket cb = fd->getCookies();
	EXPECT_EQ(1, cb.size());
	EXPECT_EQ("type", cb[0].name);
	EXPECT_EQ("value", cb[0].value);
}

TEST_F(CurlFileDownloaderServerTest, bigFile)
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

TEST_F(CurlFileDownloaderTest, cookieIntegrity)
{
	auto cfd = this->fileDownloaderFactory->create();
	FileDownloader::CookieBucket cb
	{
		{"key1", "value1", "localhost", "/", 0, true},
		{"key2", "value2", "localhost", "/", 0, true}
	};
	cfd->setCookies(cb);
	EXPECT_EQ(cb, cfd->getCookies());
}

TEST_F(CurlFileDownloaderTest, simpleCookieIntegrity)
{
	auto cfd = this->fileDownloaderFactory->create();
	FileDownloader::CookieBucket cb
	{
		{"key1", "value1", "localhost", "/"}
	};
	cfd->setCookies(cb);
	EXPECT_EQ(cb, cfd->getCookies());
}

TEST_F(CurlFileDownloaderTest, setCookieCrazyDomainLocalhostDot)
{
	auto cfd = this->fileDownloaderFactory->create();

	// curl is doing some weird stuff, check here if curl has changed
	// curl converts localhost to .localhost
	cfd->addCookie({"crazyCurl", "", "localhost"});

	auto cs = cfd->getCookies();
	ASSERT_EQ(1, cs.size());
	EXPECT_EQ("localhost", cs[0].domain);
}

TEST_F(CurlFileDownloaderTest, cookieExpired)
{
	auto cfd = this->fileDownloaderFactory->create();
	cfd->addCookie({"expired", "expired", "domain.org", "/", 10000});
	auto cs = cfd->getCookies();
	EXPECT_TRUE(cs.empty());
}

TEST_F(CurlFileDownloaderTest, sessionCookie)
{
	auto cfd = this->fileDownloaderFactory->create();
	cfd->addCookie({"expired", "expired", "domain.org", "/", 0});
	auto cs = cfd->getCookies();
	EXPECT_FALSE(cs.empty());
}

TEST_F(CurlFileDownloaderTest, cookieCheckExpiring)
{
	auto cfd = this->fileDownloaderFactory->create();
	cfd->addCookie({"expiring", "expiring", "domain.org", "/",
	                std::chrono::duration_cast<std::chrono::seconds>((std::chrono::system_clock::now() + std::chrono::seconds(1)).time_since_epoch()).count()});
	ASSERT_FALSE(cfd->getCookies().empty());
	std::this_thread::sleep_for(std::chrono::seconds(2));
	ASSERT_TRUE(cfd->getCookies().empty());
}
