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
#include <unordered_set>

#include <gamekeeper/core/httpfiledownloader.h>
#include <gamekeeper/core/propertyresolver.h>
#include <gamekeeper/core/userpaths.h>

#include <boost/filesystem/path.hpp>

GAMEKEEPER_NAMESPACE_START(core)

class CURLPrivateData;
interface Logger;
interface LoggerFactory;

class PUBLIC_API CurlFileDownloader : public HttpFileDownloader
{
public:
	PUBLIC_API CurlFileDownloader(std::shared_ptr<LoggerFactory>, std::shared_ptr<PropertyResolver>, std::shared_ptr<UserPaths>);
	PRIVATE_API virtual ~CurlFileDownloader();
	PRIVATE_API virtual bool supportsProtocol(const char * const protocolName) override;
	PRIVATE_API virtual void downloadFile(const char * const url, DownloadCallback callback) override;
	PRIVATE_API virtual void downloadFileWithCookies(const char * const url, DownloadCallback callback,
	                                                 const CookieBuket& cookies) override;
	PRIVATE_API virtual CookieBuket doPostRequestForCookies(const char * const url, const Form& form) override;
	PRIVATE_API virtual void downloadFileWithForm(const char * const url, DownloadCallback callback, const Form & form) override;
	PRIVATE_API virtual CookieBuket downloadFileAndCookiesWithForm(const char * const url, DownloadCallback callback,
                                                                       const Form & form) override;
private:
	std::shared_ptr<PropertyResolver> propertyResolver;
	std::shared_ptr<UserPaths> userpaths;
	Logger & logger;
	const std::string userAgent;

	PRIVATE_API void performCurl(CURLPrivateData & curl, uint32_t timeout = 0);
	PRIVATE_API void handleFileDownload(CURLPrivateData & curl, FileDownloader::DownloadCallback * func,
	                                    const char * const url);
	PRIVATE_API boost::filesystem::path resolveDownloadPath(const char * const url);

	static const std::unordered_set<std::string> supportedProtocols;
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_CORE_CURLFILEDOWNLOADER_H
