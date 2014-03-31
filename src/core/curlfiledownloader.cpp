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

#include "pch.h"

#include <gamekeeper/core/curlfiledownloader.h>
#include <gamekeeper/core/curlhelper.h>
#include <gamekeeper/core/logger.h>
#include <gamekeeper/core/loggerFactory.h>
#include <gamekeeper/core/loggerStream.h>

#include <boost/algorithm/string/replace.hpp>

#include <curl/curl.h>

namespace algo = boost::algorithm;

GAMEKEEPER_NAMESPACE_START(core)

CurlFileDownloader::CurlFileDownloader(std::shared_ptr<LoggerFactory> loggerFactory,
                                       std::shared_ptr<PropertyResolver> _propertyResolver,
                                       std::shared_ptr<OSPaths> _ospaths)
:	propertyResolver(_propertyResolver),
	ospaths(_ospaths),
	logger(loggerFactory->getComponentLogger("IO.curl")),
	curlHelper("GameKeeper/0.1")
{
	logger << LogLevel::Debug << "init curl" << endl;
	curl_global_init(CURL_GLOBAL_SSL);
}

CurlFileDownloader::~CurlFileDownloader()
{
	curl_global_cleanup();
}

bool
CurlFileDownloader::supportsProtocol(const char * const protocolName, size_t nameSize)
{
	return true;
}

void
CurlFileDownloader::downloadFile(const char * const url, DownloadCallback callback)
{
	boost::filesystem::path downloadPath = this->resolveDownloadPath(url);
	logger << LogLevel::Debug << "try to download file from: " << url << " at: " << downloadPath.string() << endl;
	CURL * curl = this->curlHelper.createCURL(url);
	this->curlHelper.setUpFileDownload(curl, &callback,
	                                   this->propertyResolver->get<uint32_t>("network.download.max_buffer_size"),
	                                   resolveDownloadPath(url));
	handleCurlError(curl_easy_perform(curl));
	this->curlHelper.handleFileDownloadResult(curl);
	this->curlHelper.deleteCURL(curl);
}

void
CurlFileDownloader::downloadFileWithCookies(const char * const url, DownloadCallback callback,
                                            const CookieBuket& cookies)
{
	boost::filesystem::path downloadPath = this->resolveDownloadPath(url);
	logger << LogLevel::Debug << "try to download file from: " << url << " at: " << downloadPath.string() << endl;
	CURL * curl = this->curlHelper.createCURL(url);
	this->curlHelper.setUpFileDownload(curl, &callback,
	                                   this->propertyResolver->get<uint32_t>("network.download.max_buffer_size"),
	                                   resolveDownloadPath(url));

	this->curlHelper.addCookiesToCurl(cookies, curl);

	handleCurlError(curl_easy_perform(curl));
	this->curlHelper.handleFileDownloadResult(curl);
	this->curlHelper.deleteCURL(curl);
}

CurlFileDownloader::CookieBuket
CurlFileDownloader::doPostRequestForCookies(const char * const url, const Form& form)
{
	logger << LogLevel::Debug << "try to fetch cookies at: " << url << endl;
	CURL * curl = this->curlHelper.createCURL(url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlHelper::emptyCurlFileDownloadCallback);
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, nullptr);

	this->curlHelper.addFormToCurl(form, curl);

	handleCurlError(curl_easy_perform(curl));

	CurlFileDownloader::CookieBuket result = this->curlHelper.getCookies(curl);
	this->curlHelper.deleteCURL(curl);

	return result;
}

void
CurlFileDownloader::handleCurlError(int code)
{
	switch (code)
	{
		case CURLE_OK:
			// everything okay
			logger << LogLevel::Trace << "CURL returned with CURLE_OK" << endl;
			break;
		default:
			// unhandled error
			logger << LogLevel::Fatal << "CURL return code " << code << " unhandled, please report a bug" << endl;
			break;
	}
}

boost::filesystem::path
CurlFileDownloader::resolveDownloadPath(const char * const url)
{
	std::string uri = url;
	// frist cut the protocoll
	size_t pos = uri.find("://");
	uri.erase(0, pos + 3);
	// now replace some unsupported characters
	algo::replace_all(uri, ":", "_");
	return this->ospaths->getCacheFile(std::string("downloads/" + uri));
}

GAMEKEEPER_NAMESPACE_END(core)
