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

#ifndef GAMELIB_CORE_CURLFILEDOWNLOADER_H
#define GAMELIB_CORE_CURLFILEDOWNLOADER_H 1

#include <gamelib/core/common.h>

#include <memory>

#include <gamelib/core/curlhelper.h>
#include <gamelib/core/httpfiledownloader.h>

GAMEKEEPER_NAMESPACE_START(core)

class Logger;
class LoggerFactory;

class PUBLIC_API CurlFileDownloader : public HttpFileDownloader
{
public:
	PUBLIC_API CurlFileDownloader(std::shared_ptr<LoggerFactory>);
	PRIVATE_API ~CurlFileDownloader();
	PRIVATE_API GAMELIB_IMPLEMENTATION_OVERRIDE(bool supportsProtocol(const char * const protocolName, size_t nameSize));
	PRIVATE_API GAMELIB_IMPLEMENTATION_OVERRIDE(void downloadFile(const char * const url, DownloadCallback callback));
	PRIVATE_API GAMELIB_IMPLEMENTATION_OVERRIDE(void downloadFileWithCookies(const char * const url,
	                                                                         DownloadCallback callback,
	                                                                         const CookieBuket& cookies));
	PRIVATE_API GAMELIB_IMPLEMENTATION_OVERRIDE(CookieBuket doPostRequestForCookies(const char * const url,
	                                                                                const Form& form));
private:
	Logger & logger;
	CurlHelper curlHelper;

	PRIVATE_API void handleCurlError(int code);
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMELIB_CORE_CURLFILEDOWNLOADER_H
