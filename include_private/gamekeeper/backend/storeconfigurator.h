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

#include <boost/filesystem/path.hpp>

#include <gamekeeper/backend/storeconfiguration.h>
#include <gamekeeper/core/exception.h>

GAMEKEEPER_NAMESPACE_START(core, network)

interface FileDownloaderFactory;

GAMEKEEPER_NAMESPACE_END(core, network)

GAMEKEEPER_NAMESPACE_START(backend, security)

interface AuthManager;

GAMEKEEPER_NAMESPACE_END(backend, security)

GAMEKEEPER_NAMESPACE_START(backend)

/**
 * @class StoreConfigurator storeconfigurator <gamekeeper/backend/storconfigurator.h>
 *
 * @author Karol Herbst
 * @since 0
 */
class PUBLIC_API StoreConfigurator
{
public:
	PUBLIC_API StoreConfigurator(std::shared_ptr<core::network::FileDownloaderFactory>, std::shared_ptr<security::AuthManager>);
	PUBLIC_API StoreConfiguration configure(const boost::filesystem::path & configFile);

	class PUBLIC_API GAMEKEEPER_EXCEPTION(StoreConfiguratorException);
private:
	std::shared_ptr<core::network::FileDownloaderFactory> fdf;
	std::shared_ptr<security::AuthManager> am;
};

GAMEKEEPER_NAMESPACE_END(backend)
