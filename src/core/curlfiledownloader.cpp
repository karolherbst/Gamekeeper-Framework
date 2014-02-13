/*
 * libgamelib
 *
 * Copyright (C) 2013 Karol Herbst <gamelib@karolherbst.de>
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

#include <gamelib/core/curlfiledownloader.h>
#include <gamelib/core/curlhelper.h>
#include <gamelib/core/logger.h>
#include <gamelib/core/loggerFactory.h>
#include <gamelib/core/loggerStream.h>

#include <curl/curl.h>

GAMELIB_NAMESPACE_START(core)

CurlFileDownloader::CurlFileDownloader(std::shared_ptr<LoggerFactory> loggerFactory)
:	logger(loggerFactory->getComponentLogger("IO.curl"))
{
	logger << LogLevel::Debug << "init curl" << endl;
	curl_global_init(CURL_GLOBAL_SSL);
	CurlHelper::setUserAgent("GameLib/0.1");
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
	logger << LogLevel::Debug << "try to download file at: " << url << endl;
	CURL * curl = CurlHelper::createCURL(url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlHelper::curlFileDownloadCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &callback);
	handleCurlError(curl_easy_perform(curl));
	CurlHelper::deleteCURL(curl);
}

void
CurlFileDownloader::downloadFileWithCookies(const char * const url, DownloadCallback callback,
                                            const CookieBuket& cookies)
{
	logger << LogLevel::Debug << "try to download file at: " << url << endl;
	CURL * curl = CurlHelper::createCURL(url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlHelper::curlFileDownloadCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &callback);
	
	CurlHelper::addCookiesToCurl(cookies, curl);
	
	handleCurlError(curl_easy_perform(curl));
	CurlHelper::deleteCURL(curl);
}

CurlFileDownloader::CookieBuket
CurlFileDownloader::doPostRequestForCookies(const char * const url, const Form& form)
{
	logger << LogLevel::Debug << "try to fetch cookies at: " << url << endl;
	CURL * curl = CurlHelper::createCURL(url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlHelper::emptyCurlFileDownloadCallback);
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, nullptr);

	CurlHelper::addFormToCurl(form, curl);

	handleCurlError(curl_easy_perform(curl));

	CurlFileDownloader::CookieBuket result = CurlHelper::getCookies(curl);
	CurlHelper::deleteCURL(curl);

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

GAMELIB_NAMESPACE_END(core)
