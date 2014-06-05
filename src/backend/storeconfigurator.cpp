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

#include <gamekeeper/backend/storeconfigurator.h>

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <gamekeeper/backend/httppostloginhandler.h>
#include <gamekeeper/backend/jsongamelistparser.h>
#include <gamekeeper/backend/oauthloginhandler.h>
#include <gamekeeper/backend/storeconfiguration.h>
#include <gamekeeper/backend/xmlgamelistparser.h>
#include <gamekeeper/core/filedownloaderfactory.h>
#include <gamekeeper/model/store.h>
#include <gamekeeper/utils/containerutils.h>

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
:	name(props["store.name"]),
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
		// insert an empty string with the section, so that we can check for them
		map.insert(std::make_pair(s.first, ""));
		for(const auto & k : s.second)
		{
			map.insert(std::make_pair(s.first + '.' + k.first, k.second.get_value<std::string>()));
		}
	}
}

StoreConfigurator::StoreConfigurator(std::shared_ptr<core::FileDownloaderFactory> _fdf, std::shared_ptr<AuthManager> _am)
:	fdf(_fdf),
	am(_am){}

StoreConfiguration
StoreConfigurator::configure(const boost::filesystem::path & configFile)
{
	std::map<std::string, std::string> props;
	{
		prop::ptree config;
		prop::read_ini(configFile.string(), config);
		loadIniFileIntoMap(config, props);
	}

	try
	{
		// here we check for properties every config file must have. It may be that some will move in different places
		// after more implementations came up
		auto missing = utils::Containers::checkMissing(props,
		{
			"games.url",
			"games.list",
			"game.id",
			"game.name",
			"store.format",
			"store.name"
		});

		if(!missing.empty())
		{
			throw core::PropertiesMissingException(missing);
		}

		const std::string & storeFormat = props["store.format"];
		const std::string & authMethod = props["auth.method"];

		std::shared_ptr<GameListParser> glp;
		if(storeFormat == "xml")
		{
			glp = std::make_shared<XMLGameListParser>(props);
		}
		else if(storeFormat == "json")
		{
			glp = std::make_shared<JSONGameListParser>(props);
		}

		StoreConfiguration::LoginHandlerMap lhm;
		if(props.find("auth_http_post") != props.end())
		{
			lhm.insert(std::make_pair("http_post", std::make_shared<HTTPPostLoginHandler>(props, this->fdf->create(), this->am)));
		}

		if(props.find("auth_oauth") != props.end())
		{
			lhm.insert(std::make_pair("oauth", std::make_shared<OAuthLoginHandler>(props, this->fdf->create(), this->am)));
		}

		if(lhm.empty())
		{
			throw StoreConfiguratorException("no login mechanism found in config");
		}

		return StoreConfiguration(glp, lhm, std::make_shared<StoreProps>(props));
	}
	catch(const std::exception & ex)
	{
		throw StoreConfiguratorException(std::string("error[") + ex.what() + "] while parsing store config file: " + configFile.string());
	}
	catch(...)
	{
		throw StoreConfiguratorException("unhandled error while parsing store config file: " + configFile.string());
	}
}

GAMEKEEPER_NAMESPACE_END(backend)
