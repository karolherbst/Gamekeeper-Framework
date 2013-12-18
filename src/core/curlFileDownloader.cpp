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
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
}

GAMELIB_NAMESPACE_END(core)
