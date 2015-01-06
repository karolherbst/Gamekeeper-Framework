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

	/**
	 * @interface Cookie filedownloader.h <gamekeeper/core/filedownloader.h>
	 *
	 * A Cookie is a small piece of data to store http data sent by the server. This class provides an easy way to create such.
	 * All fields are conforming to the HTTP State Management Mechanism standard (RFC 6265) and have the same meaning period.
	 *
	 * All created Cookie objects represent session cookies by default
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	struct PUBLIC_API Cookie
	{
		typedef std::chrono::system_clock::time_point TimePoint;
		PUBLIC_API Cookie(const std::string & name, const std::string & value, const std::string & domain, const std::string & path = "",
		                  const TimePoint & expiry = TimePoint(std::chrono::seconds(0)), bool secure = false);
		PUBLIC_API Cookie(const std::string & name, const std::string & value, const std::string & domain, const std::string & path,
		                  const TimePoint::duration &, bool secure = false);
		PUBLIC_API Cookie(const std::string & name, const std::string & value, const std::string & domain, const std::string & path,
		                  const TimePoint::rep & duration, bool secure = false);
		const std::string name;
		const std::string value;
		const std::string domain;
		const std::string path;
		const TimePoint expiry;
		const bool secure;
	};

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

bool PUBLIC_API operator==(const FileDownloader::Cookie & a, const FileDownloader::Cookie & b);

inline bool PUBLIC_INLINE operator!=(const FileDownloader::Cookie & a, const FileDownloader::Cookie & b)
{
	return !(a == b);
}

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_CORE_NETWORK_FILEDOWNLOADER_H
