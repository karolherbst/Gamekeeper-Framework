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

#ifndef GAMEKEEPER_CORE_NETWORK_FILEDOWNLOADER_H
#define GAMEKEEPER_CORE_NETWORK_FILEDOWNLOADER_H 1

#include <gamekeeper/core/common.h>

#include <chrono>
#include <functional>
#include <iostream>
#include <vector>
#include <unordered_map>

#include <gamekeeper/core/exception.h>
#include <gamekeeper/core/interface.h>

GAMEKEEPER_NAMESPACE_START(core)

struct Cookie;

/**
 * @interface FileDownloader filedownloader.h <gamekeeper/core/network/filedownloader.h>
 *
 * This interface provides basic operations to download files over a network. An Implementation shall support
 * HTTP(S) and (S)FTP(S).
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API FileDownloader
{
	GAMEKEEPER_INTERFACE_METHODS(FileDownloader);

	typedef std::basic_istream<gkbyte_t> ByteIstream;

	typedef std::function<bool (ByteIstream &)> DownloadCallback;

	typedef std::vector<Cookie> CookieBucket;

	typedef std::unordered_map<std::string, std::string> Form;

	typedef std::pair<std::string, std::string> FormField;

	PUBLIC_API virtual void getRequest(const std::string & url, const DownloadCallback & callback) = 0;

	PUBLIC_API virtual void postRequest(const std::string & url, const Form & form = Form()) = 0;

	PUBLIC_API virtual void addCookie(const Cookie & cookie) = 0;
	PUBLIC_API virtual void addCookies(const CookieBucket & cookies) = 0;
	PUBLIC_API virtual CookieBucket getCookies() = 0;

	PUBLIC_API virtual void setCookies(const CookieBucket & cookies) = 0;
	PUBLIC_API virtual void clearCookies() = 0;

	class PUBLIC_API GAMEKEEPER_EXCEPTION(FileDownloaderException);
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_CORE_NETWORK_FILEDOWNLOADER_H
