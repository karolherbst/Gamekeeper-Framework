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

#ifndef GAMELIB_CORE_CURLHELPER_H
#define GAMELIB_CORE_CURLHELPER_H 1

#include <gamelib/core/common.h>

#include <map>

#include "curlFileDownloader.h"

#include "curl/curl.h"

GAMELIB_NAMESPACE_START(core)

class CURLPrivateData;

class PRIVATE_API CurlHelper
{
public:
	PRIVATE_API static int curlFileDownloadCallback(void * const buffer, size_t bufferSize, size_t dataLength,
	                                                CurlFileDownloader::DownloadCallback * func);
	PRIVATE_API static int emptyCurlFileDownloadCallback(void * const buffer, size_t bufferSize, size_t dataLength,
	                                                void * func);
	PRIVATE_API static void addCookiesToCurl(const CurlFileDownloader::CookieBuket& cookies, CURL * curl);
	PRIVATE_API static CurlFileDownloader::CookieBuket getCookies(CURL * curl);
	PRIVATE_API static void addFormToCurl(const CurlFileDownloader::Form& form, CURL * curl);
	PRIVATE_API static CURL * createCURL(const char * const url);
	PRIVATE_API static void deleteCURL(CURL * curl);
	PRIVATE_API static void setUserAgent(std::string userAgent);
private:
	CurlHelper();

	static std::string userAgent;

	PRIVATE_API static std::map<CURL *, CURLPrivateData *> curlData;
	PRIVATE_API static std::string& createUserAgentString();
};

GAMELIB_NAMESPACE_END(core)

#endif //GAMELIB_CORE_CURLHELPER_H
