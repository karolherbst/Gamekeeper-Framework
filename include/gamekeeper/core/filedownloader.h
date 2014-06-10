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

#ifndef GAMEKEEPER_CORE_FILEDOWNLOADER_H
#define GAMEKEEPER_CORE_FILEDOWNLOADER_H 1

#include <gamekeeper/core/common.h>

#include <unordered_map>

#include <gamekeeper/core/interface.h>

GAMEKEEPER_NAMESPACE_START(core)

/**
 * @interface FileDownloader filedownloader.h <gamekeeper/core/filedownloader.h>
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

	/**
	 * the callback function signature primary for lambdas
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param[in] buffer the buffer with the raw data
	 * @param[in] bufferSize the size of @p buffer
	 * @return true if there was no error while handling the callback
	 */
	typedef std::function<bool (ByteIstream &)> DownloadCallback;

	/**
	 * A CookieBuket stores cookies as simple string string mappings
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	typedef std::unordered_map<std::string, std::string> CookieBuket;

	/**
	 * The signature of one Cookie
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	typedef std::pair<std::string, std::string> Cookie;

	/**
	 * A Form stores form data as simple string string mappings
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	typedef std::unordered_map<std::string, std::string> Form;

	/**
	 * The signature of one HTTP Form entry
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	typedef std::pair<std::string, std::string> FormField;

	/**
	 * downloads the file behind the given location
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param[in] url the url
	 * @param[in] callback the callback function
	 */
	PUBLIC_API virtual void downloadFile(const char * const url, DownloadCallback callback) = 0;

	/**
	 * downloads the file behind the given location
	 *
	 * the given cookies will be passed to the Http(s) request
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param[in] url the url
	 * @param[in] callback the callback function
	 * @param[in] cookies the cookies
	 */
	PUBLIC_API virtual void downloadFileWithCookies(const char * const url, DownloadCallback callback,
	                                                const CookieBuket& cookies) = 0;

	/**
	 * fetches cookies with the given POST form
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param[in] url the url
	 * @param[in] form the form
	 * @return the fetched cookies
	 */
	PUBLIC_API virtual CookieBuket doPostRequestForCookies(const char * const url, const Form& form = Form()) = 0;

	/**
	 * downloads the file with the given POST form
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param[in] url the url
	 * @param[in] callback the callback function
	 * @param[in] form the form
	 */
	PUBLIC_API virtual void downloadFileWithForm(const char * const url, DownloadCallback callback, const Form & form) = 0;

	/**
	 * downloads the file and fetch cookies with the given POST form
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param[in] url the url
	 * @param[in] callback the callback function
	 * @param[in] form the form
	 * @return the fetched cookies
	 */
	PUBLIC_API virtual CookieBuket downloadFileAndCookiesWithForm(const char * const url, DownloadCallback callback,
	                                                              const Form & form) = 0;
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_CORE_FILEDOWNLOADER_H
