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

#ifndef GAMEKEEPER_BACKEND_LOGINHANDLER_H
#define GAMEKEEPER_BACKEND_LOGINHANDLER_H 1

#include <gamekeeper/core/common.h>

#include <gamekeeper/core/interface.h>
#include <gamekeeper/core/network/filedownloader.h>

GAMEKEEPER_NAMESPACE_START(backend)

interface PUBLIC_API LoginHandler
{
	GAMEKEEPER_INTERFACE_METHODS(LoginHandler);

	PUBLIC_API virtual bool login(const std::string & username, const std::string & password) = 0;
	PUBLIC_API virtual void logout() = 0;
	PUBLIC_API virtual bool isLoggedIn() const = 0;
	PUBLIC_API virtual void downloadFile(const std::string & url, core::network::FileDownloader::DownloadCallback) = 0;
};

GAMEKEEPER_NAMESPACE_END(backend)

#endif //GAMEKEEPER_BACKEND_GAMELISTPARSER_H
