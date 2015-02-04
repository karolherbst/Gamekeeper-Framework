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

#pragma once

#include <gamekeeper/core/common.h>

#include <memory>

#include <boost/filesystem/path.hpp>

#include <gamekeeper/core/network/filedownloader.h>

GAMEKEEPER_NAMESPACE_START(core)

interface Logger;

GAMEKEEPER_NAMESPACE_END(core)

GAMEKEEPER_NAMESPACE_START(core, network)

class PUBLIC_API CurlFileDownloader : public FileDownloader
{
public:
	PUBLIC_API CurlFileDownloader(Logger & logger, const std::string & userAgent, const boost::filesystem::path & cacheDir,
	                              uint16_t connectionTimeout, uint16_t retryPause, uint16_t maxResolveRetries, uint16_t maxConnectRetries,
	                              uint32_t maxBufferSize);
	PRIVATE_API virtual ~CurlFileDownloader();

	PRIVATE_API virtual void getRequest(const std::string & url, const DownloadCallback & callback) override;
	PRIVATE_API virtual void postRequest(const std::string & url, const Form & form) override;

	PRIVATE_API virtual void addCookie(const Cookie & cookie) override;
	PRIVATE_API virtual void addCookies(const CookieBucket & cookies) override;
	PRIVATE_API virtual CookieBucket getCookies() override;

	PRIVATE_API virtual void setCookies(const CookieBucket & cookies) override;
	PRIVATE_API virtual void clearCookies() override;
private:
	class PRIVATE_API PImpl;
	std::unique_ptr<CurlFileDownloader::PImpl> data;
};

GAMEKEEPER_NAMESPACE_END(core, network)
