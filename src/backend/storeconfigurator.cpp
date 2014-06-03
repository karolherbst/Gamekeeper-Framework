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

#include "pch.h"

#include <gamekeeper/backend/storeconfigurator.h>

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <gamekeeper/backend/httppostloginhandler.h>
#include <gamekeeper/backend/jsongamelistparser.h>
#include <gamekeeper/backend/storeconfiguration.h>
#include <gamekeeper/backend/xmlgamelistparser.h>
#include <gamekeeper/model/store.h>

namespace prop = boost::property_tree;


GAMEKEEPER_NAMESPACE_START(backend)

class PRIVATE_API StoreProps : public model::Store
{
public:
	PRIVATE_API StoreProps(std::map<std::string, std::string> props);

private:
	PRIVATE_API virtual const std::string & getName() const override;
	PRIVATE_API virtual const std::map<std::string, std::string> & getConfig() const override;

	const std::string name;
	const std::map<std::string, std::string> config;
};

StoreProps::StoreProps(std::map<std::string, std::string> props)
:	name(props.at("store.name")),
	config(props){}

const std::string &
StoreProps::getName() const
{
	return this->name;
}

const std::map<std::string, std::string> &
StoreProps::getConfig() const
{
	return this->config;
}

static void loadIniFileIntoMap(const prop::ptree & tree, std::map<std::string, std::string> & map)
{
	for(const auto & s : tree)
	{
		for(const auto & k : s.second)
		{
			map.insert(std::make_pair(s.first + '.' + k.first, k.second.get_value<std::string>()));
		}
	}
}

StoreConfigurator::StoreConfigurator(std::shared_ptr<core::HttpFileDownloader> _hfd)
:	hfd(_hfd){}

StoreConfiguration
StoreConfigurator::configure(const boost::filesystem::path & configFile)
{
	std::map<std::string, std::string> props;
	{
		prop::ptree config;
		prop::read_ini(configFile.string(), config);
		loadIniFileIntoMap(config, props);
	}

	std::string storeFormat = props.at("store.format");
	std::string authMethod = props.at("auth.method");

	GameListParser * glp = nullptr;
	if(storeFormat == "xml")
	{
		glp = new XMLGameListParser(props);
	}
	else if(storeFormat == "json")
	{
		glp = new JSONGameListParser(props);
	}

	LoginHandler * lh = nullptr;
	if(authMethod == "http_post")
	{
		lh = new HTTPPostLoginHandler(props, this->hfd);
	}

	return StoreConfiguration(glp, lh, new StoreProps(props));
}

GAMEKEEPER_NAMESPACE_END(backend)
