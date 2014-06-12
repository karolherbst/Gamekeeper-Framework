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

#ifndef GAMEKEEPER_BACKEND_STOREMANAGER_H
#define GAMEKEEPER_BACKEND_STOREMANAGER_H 1

#include <gamekeeper/core/common.h>

#include <map>
#include <memory>
#include <vector>

#include <gamekeeper/backend/storeconfiguration.h>

GAMEKEEPER_NAMESPACE_START(core)

class BundlePaths;
class FileDownloaderFactory;
class Logger;
class LoggerFactory;

GAMEKEEPER_NAMESPACE_END(core)

GAMEKEEPER_NAMESPACE_START(backend)

/**
 * @class StoreManager storemanager <gamekeeper/backend/storemanager.h>
 *
 * @author Karol Herbst
 * @since 0
 */
class PUBLIC_API StoreManager
{
public:
	PUBLIC_API StoreManager(std::shared_ptr<core::LoggerFactory>, std::shared_ptr<core::BundlePaths>, std::shared_ptr<core::FileDownloaderFactory>);
	PUBLIC_API std::vector<StoreConfiguration> getAllStores();
	PUBLIC_API StoreConfiguration getStore(const std::string &);
private:
	core::Logger & logger;
	std::map<std::string, StoreConfiguration> storeConfigs;
};

GAMEKEEPER_NAMESPACE_END(backend)

#endif //GAMEKEEPER_BACKEND_STORECONFIGURATOR_H
