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

#include "curlFileDownloader.h"
#include "curlHelper.h"

#include <curl/curl.h>

GAMELIB_NAMESPACE_START(core)

CurlFileDownloader::CurlFileDownloader()
{
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
	CURL * curl = CurlHelper::createCURL();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlHelper::curlFileDownloadCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &callback);
	curl_easy_perform(curl);
	CurlHelper::deleteCURL(curl);
}

void
CurlFileDownloader::downloadFileWithCookies(const char * const url, DownloadCallback callback,
                                            const CookieBuket& cookies)
{
	CURL * curl = CurlHelper::createCURL();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlHelper::curlFileDownloadCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &callback);
	
	CurlHelper::addCookiesToCurl(cookies, curl);
	
	curl_easy_perform(curl);
	CurlHelper::deleteCURL(curl);
}

CurlFileDownloader::CookieBuket
CurlFileDownloader::getAllCookies(const char * const url, const CookieBuket& cookies)
{
	CURL * curl = CurlHelper::createCURL();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlHelper::emptyCurlFileDownloadCallback);
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, nullptr);
	
	CurlHelper::addCookiesToCurl(cookies, curl);
	
	curl_easy_perform(curl);
	
	CurlFileDownloader::CookieBuket result = CurlHelper::getCookies(curl);
	CurlHelper::deleteCURL(curl);
	
	return result;
}

CurlFileDownloader::CookieBuket
CurlFileDownloader::doPostRequestForCookies(const char * const url, const Form& form)
{
	CURL * curl = CurlHelper::createCURL();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlHelper::emptyCurlFileDownloadCallback);
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, nullptr);

	CurlHelper::addFormToCurl(form, curl);

	curl_easy_perform(curl);

	CurlFileDownloader::CookieBuket result = CurlHelper::getCookies(curl);
	CurlHelper::deleteCURL(curl);

	return result;
}

GAMELIB_NAMESPACE_END(core)
