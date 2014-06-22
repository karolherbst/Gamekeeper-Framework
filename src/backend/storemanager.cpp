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

#include <gamekeeper/backend/storemanager.h>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem/operations.hpp>

#include <gamekeeper/backend/storeconfiguration.h>
#include <gamekeeper/backend/storeconfigurator.h>
#include <gamekeeper/core/bundlepaths.h>
#include <gamekeeper/core/logger.h>
#include <gamekeeper/core/loggerFactory.h>
#include <gamekeeper/core/loggerStream.h>
#include <gamekeeper/utils/stringutils.h>

namespace balgo = boost::algorithm;
namespace bfs = boost::filesystem;

GAMEKEEPER_NAMESPACE_START(backend)

using core::endl;
using core::LogLevel;

StoreManager::StoreManager(std::shared_ptr<core::LoggerFactory> lf, std::shared_ptr<core::BundlePaths> bp, std::shared_ptr<core::FileDownloaderFactory> fdf)
:	logger(lf->getComponentLogger("backend.StoreManager"))
{
	StoreConfigurator sc(fdf);
	// load all store config files
	for(bfs::directory_iterator pIt(bp->getDataPath() / "stores"); pIt != bfs::directory_iterator(); pIt++)
	{
		bfs::path file((*pIt).path());
		this->logger << LogLevel::Debug << "parsing file: " << file.string() << endl;
		StoreConfiguration sConfig = sc.configure(file);
		this->storeConfigs.insert(std::make_pair(balgo::to_lower_copy(sConfig.getStore()->getName()), sConfig));
	}
}

std::vector<StoreConfiguration>
StoreManager::getAllStores()
{
	std::vector<StoreConfiguration> result;
	for(auto & p : this->storeConfigs)
	{
		result.push_back(p.second);
	}
	return result;
}

StoreConfiguration
StoreManager::getStore(const std::string & name)
{
	return this->storeConfigs.at(balgo::to_lower_copy(name));
}

GAMEKEEPER_NAMESPACE_END(backend)
