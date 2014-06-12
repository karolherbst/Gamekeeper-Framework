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

#ifndef GAMEKEEPER_BACKEND_STORECONFIGURATOR_H
#define GAMEKEEPER_BACKEND_STORECONFIGURATOR_H 1

#include <gamekeeper/core/common.h>

#include <boost/filesystem/path.hpp>

#include <gamekeeper/backend/storeconfiguration.h>

GAMEKEEPER_NAMESPACE_START(core)

class FileDownloaderFactory;

GAMEKEEPER_NAMESPACE_END(core)

GAMEKEEPER_NAMESPACE_START(backend)

class AuthManager;

/**
 * @class StoreConfigurator storeconfigurator <gamekeeper/backend/storconfigurator.h>
 *
 * @author Karol Herbst
 * @since 0
 */
class PUBLIC_API StoreConfigurator
{
public:
	PUBLIC_API StoreConfigurator(std::shared_ptr<core::FileDownloaderFactory>, std::shared_ptr<AuthManager>);
	PUBLIC_API StoreConfiguration configure(const boost::filesystem::path & configFile);
private:
	std::shared_ptr<core::FileDownloaderFactory> fdf;
	std::shared_ptr<AuthManager> am;
};

GAMEKEEPER_NAMESPACE_END(backend)

#endif //GAMEKEEPER_BACKEND_STORECONFIGURATOR_H
