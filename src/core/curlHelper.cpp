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

#include "curlHelper.h"

#include <algorithm>
#include <sstream>
#include <iostream>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

GAMELIB_NAMESPACE_START(core)

int
CurlHelper::curlFileDownloadCallback(void * const buffer, size_t bufferSize, size_t dataLength,
                                     CurlFileDownloader::DownloadCallback * func)
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

int
CurlHelper::emptyCurlFileDownloadCallback(void * const buffer, size_t bufferSize, size_t dataLength, void * func)
{
	return dataLength;
}

void
CurlHelper::addCookiesToCurl(const CurlFileDownloader::CookieBuket& cookies, CURL * curl)
{
	if(!cookies.empty())
	{
		std::ostringstream cookieLineBuilder;
		for (const HttpFileDownloader::Cookie cookie : cookies)
		{
			cookieLineBuilder << cookie.first << '=' << cookie.second << ";";
		}
		std::string cookieLine = cookieLineBuilder.str();
		curl_easy_setopt(curl, CURLOPT_COOKIE, cookieLine.c_str());
	}
}

CurlFileDownloader::CookieBuket
CurlHelper::getCookies(CURL * curl)
{
	struct curl_slist * list;
	CurlFileDownloader::CookieBuket result;
	curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &list);
	
	while(list != nullptr)
	{
		std::vector<std::string> strings;
		boost::split(strings, list->data, boost::is_any_of("\t"));
		result[strings[5]] = strings[6];
		list = list->next;
	}
	
	curl_slist_free_all(list);
	return result;
}

CURL *
CurlHelper::createCURL()
{
	CURL * curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Gamelib/0.1");
	return curl;
}


void
CurlHelper::deleteCURL(CURL * curl)
{
	curl_easy_cleanup(curl);
}

GAMELIB_NAMESPACE_END(core)
