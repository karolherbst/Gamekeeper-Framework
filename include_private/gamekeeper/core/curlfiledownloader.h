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

#ifndef GAMEKEEPER_CORE_CURLFILEDOWNLOADER_H
#define GAMEKEEPER_CORE_CURLFILEDOWNLOADER_H 1

#include <gamekeeper/core/common.h>

#include <memory>

#include <gamekeeper/core/curlhelper.h>
#include <gamekeeper/core/httpfiledownloader.h>
#include <gamekeeper/core/ospaths.h>
#include <gamekeeper/core/propertyresolver.h>

#include <boost/filesystem/path.hpp>

GAMEKEEPER_NAMESPACE_START(core)

class Logger;
class LoggerFactory;

class PUBLIC_API CurlFileDownloader : public HttpFileDownloader
{
public:
	PUBLIC_API CurlFileDownloader(std::shared_ptr<LoggerFactory>, std::shared_ptr<PropertyResolver>, std::shared_ptr<OSPaths>);
	PRIVATE_API ~CurlFileDownloader();
	PRIVATE_API GAMEKEEPER_IMPLEMENTATION_OVERRIDE(bool supportsProtocol(const char * const protocolName, size_t nameSize));
	PRIVATE_API GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void downloadFile(const char * const url, DownloadCallback callback));
	PRIVATE_API GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void downloadFileWithCookies(const char * const url,
	                                                                         DownloadCallback callback,
	                                                                         const CookieBuket& cookies));
	PRIVATE_API GAMEKEEPER_IMPLEMENTATION_OVERRIDE(CookieBuket doPostRequestForCookies(const char * const url,
	                                                                                const Form& form));
private:
	std::shared_ptr<PropertyResolver> propertyResolver;
	std::shared_ptr<OSPaths> ospaths;
	Logger & logger;
	CurlHelper curlHelper;

	PRIVATE_API void handleCurlError(int code);
	PRIVATE_API boost::filesystem::path resolveDownloadPath(const char * const url);
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_CORE_CURLFILEDOWNLOADER_H
