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

#ifndef GAMELIB_CORE_FILEDOWNLOADER_H
#define GAMELIB_CORE_FILEDOWNLOADER_H 1

#include <gamelib/core/common.h>
#include <gamelib/core/interface.h>

GAMELIB_NAMESPACE_START(core)

/**
 * @interface FileDownloader filedownloader.h <gamelib/core/filedownloader.h>
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
	PUBLIC_API GAMELIB_INTERFACE_DESTRUCTOR(FileDownloader)
	
	/**
	 * checks if the implementation supports the givven protocol
	 * 
	 * @author Karol Herbst
	 * @since 0
	 * 
	 * @param[in] protocolName the name of the protocol
	 * @param[in] nameSize the size of @p protocolName
	 * @return true if the implementation supports @p protocolName
	 */
	PUBLIC_API GAMELIB_INTERFACE_METHOD(bool supportsProtocol(const char * protocolName, size_t nameSize))
};

GAMELIB_NAMESPACE_END(core)

#endif //GAMELIB_CORE_FILEDOWNLOADER_H
