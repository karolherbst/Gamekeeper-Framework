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

#include <sstream>

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
	CURL * curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curlFileDownloadCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &callback);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
}

void
CurlFileDownloader::downloadFileWithCookies(const char * const url, DownloadCallback callback,
                                            const CookieBuket& cookies)
{
	CURL * curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curlFileDownloadCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &callback);
	
	if(!cookies.empty())
	{
		std::ostringstream cookieLineBuilder;
		for (const std::pair<std::string, std::string> pair : cookies)
		{
			cookieLineBuilder << pair.first << '=' << pair.second << ";";
		}
		std::string cookieLine = cookieLineBuilder.str();
		curl_easy_setopt(curl, CURLOPT_COOKIE, cookieLine.c_str());
	}
	
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
}

int
CurlFileDownloader::curlFileDownloadCallback(void * const buffer, size_t bufferSize, size_t dataLength,
                                             DownloadCallback * func)
{
	if (func->operator()(buffer, bufferSize, dataLength))
	{
		return dataLength;
	}
	else
	{
		return -1;
	}
}

GAMELIB_NAMESPACE_END(core)
