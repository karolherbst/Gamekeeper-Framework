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

#include <functional>
#include <istream>

#include <gamekeeper/core/interface.h>

GAMEKEEPER_NAMESPACE_START(core)

/**
 * @interface FileDownloader filedownloader.h <gamekeeper/core/filedownloader.h>
 *
 * This interface provides basic operations to download files over a network. It abstracts from protocols and doesn't
 * provide functionality special to a specific protocol. If you want to use a specific protocol you should use a more
 * specilized interface instead.
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
	 * checks if the implementation supports the givven protocol
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param[in] protocolName the name of the protocol
	 * @return true if the implementation supports @p protocolName
	 */
	PUBLIC_API virtual bool supportsProtocol(const char * const protocolName) = 0;

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
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_CORE_FILEDOWNLOADER_H
