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
#include <boost/property_tree/detail/file_parser_error.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <gamekeeper/backend/httppostloginhandler.h>
#include <gamekeeper/backend/jsongamelistparser.h>
#include <gamekeeper/backend/storeconfiguration.h>
#include <gamekeeper/backend/xmlgamelistparser.h>
#include <gamekeeper/core/filedownloaderfactory.h>
#include <gamekeeper/model/store.h>
#include <gamekeeper/utils/containerutils.h>
#include <gamekeeper/utils/stringutils.h>

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

static void loadFileIntoMap(const prop::ptree & tree, std::map<std::string, std::string> & map)
{
	for(const auto & s : tree)
	{
		for(const auto & k : s.second)
		{
			map.insert(std::make_pair(s.first + '.' + k.first, k.second.get_value<std::string>()));
		}
	}
}

StoreConfigurator::StoreConfigurator(std::shared_ptr<core::FileDownloaderFactory> _fdf, std::shared_ptr<security::AuthManager> _am)
:	fdf(_fdf),
	am(_am){}

StoreConfiguration
StoreConfigurator::configure(const boost::filesystem::path & configFile)
{
	if(!configFile.has_extension())
	{
		throw StoreConfiguratorException("Config file \"" + configFile.string() + "\" has no suffix, can't determine file format");
	}

	try
	{
		std::map<std::string, std::string> props;
		prop::ptree config;
		std::string ext(configFile.extension().string(), 1);

		if(ext == "inf" || ext == "info")
		{
			prop::read_info(configFile.string(), config);
		}
		else if(ext == "ini")
		{
			prop::read_ini(configFile.string(), config);
		}
		else if(ext == "js" || ext == "json")
		{
			prop::read_json(configFile.string(), config);
		}
		else if(ext == "xml")
		{
			prop::read_xml(configFile.string(), config);
		}
		else
		{
			throw StoreConfiguratorException("Config file \"" + configFile.string() + "\" has no usable file format");
		}

		loadFileIntoMap(config, props);

		// here we check for properties every config file must have. It may be that some will move in different places
		// after more implementations came up
		auto missing = utils::Containers::checkMissing(props,
		{
			"auth.loginurl",
			"auth.logouturl",
			"auth.method",
			"authfield.username",
			"authfield.password",
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

		std::shared_ptr<LoginHandler> lh;
		if(authMethod == "http_post")
		{
			lh = std::make_shared<HTTPPostLoginHandler>(props, this->fdf->create(), this->am);
		}

		return StoreConfiguration(glp, lh, std::make_shared<StoreProps>(props));
	}
	// indicates parsing error in the read_* calls
	catch(const prop::file_parser_error & fpe)
	{
		throw StoreConfiguratorException(std::string("parsing error[") + fpe.message() + "] in file: " + configFile.string() + ":" + utils::String::toString(fpe.line()));
	}
	// just rethrow them
	catch(const StoreConfiguratorException &)
	{
		throw;
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
