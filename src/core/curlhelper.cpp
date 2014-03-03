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

#include <algorithm>
#include <iostream>
#include <sstream>

#include <gamelib/core/curlhelper.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <curl/curl.h>

GAMELIB_NAMESPACE_START(core)

struct CURLPrivateData
{
	std::string postData;
};

CurlHelper::CurlHelper(std::string newUserAgent)
:	userAgent(newUserAgent + " libcurl/" + curl_version_info(CURLVERSION_NOW)->version){
}

int
CurlHelper::curlFileDownloadCallback(void * const buffer, size_t bufferSize, size_t dataLength,
                                     HttpFileDownloader::DownloadCallback * func)
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
CurlHelper::addCookiesToCurl(const HttpFileDownloader::CookieBuket& cookies, CURL * curl)
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

HttpFileDownloader::CookieBuket
CurlHelper::getCookies(CURL * curl)
{
	struct curl_slist * list;
	HttpFileDownloader::CookieBuket result;
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

void
CurlHelper::addFormToCurl(const HttpFileDownloader::Form& form, CURL * curl)
{
	if(!form.empty())
	{
		std::ostringstream cookieLineBuilder;
		for (const HttpFileDownloader::FormField formField : form)
		{
			cookieLineBuilder << formField.first << '=' << formField.second << '&';
		}
		curlData[curl]->postData = cookieLineBuilder.str();
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, curlData[curl]->postData.c_str());
	}
}

CURL *
CurlHelper::createCURL(const char * const url)
{
	CURL * curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str());
	curlData[curl] = new CURLPrivateData();
	return curl;
}

void
CurlHelper::deleteCURL(CURL * curl)
{
	delete curlData[curl];
	curlData.erase(curl);
	curl_easy_cleanup(curl);
}

GAMELIB_NAMESPACE_END(core)
