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

#include <gamekeeper/backend/xmlgamelistparser.h>

#include <pugixml.hpp>

#include <gamekeeper/model/game.h>

GAMEKEEPER_NAMESPACE_START(backend)

class GameXML : public gamekeeper::model::Game
{
	friend class XMLGameListParser;
private:
	std::string id;
	std::string name;
	std::set<model::Platform> platforms;
public:
	virtual const std::string & getId() const override
	{
		return this->id;
	}

	virtual const std::string & getName() const override
	{
		return this->name;
	}

	virtual const std::set<model::Platform> & getPlatforms() const override
	{
		return this->platforms;
	}
};

class XMLGameListParser::PImpl
{
public:
	PImpl(std::map<std::string, std::string> & config);
	pugi::xpath_query gamesListQuery;
	pugi::xpath_query gameIdQuery;
	pugi::xpath_query gameNameQuery;
	pugi::xpath_query platformIdQuery;

	std::string gamePlatformIdsPath;

	std::string platformWin32Id;
	std::string platformMac32Id;
	std::string platformLin32Id;
	std::string platformLin64Id;
};

XMLGameListParser::PImpl::PImpl(std::map<std::string, std::string> & config)
:	gamesListQuery(config.at("games.list").c_str()),
	gameIdQuery(config.at("game.id").c_str()),
	gameNameQuery(config.at("game.name").c_str()),
	platformIdQuery(config.at("platform.id").c_str()),
	gamePlatformIdsPath(config.at("game.platforms")),
	platformWin32Id(config.at("platform.win32")),
	platformMac32Id(config.at("platform.mac32")),
	platformLin32Id(config.at("platform.lin32")),
	platformLin64Id(config.at("platform.lin64")){}

XMLGameListParser::XMLGameListParser(std::map<std::string, std::string> & config)
:	data(new XMLGameListParser::PImpl(config)){}

std::unordered_set<std::unique_ptr<model::Game>>
XMLGameListParser::parseGameList(std::basic_istream<gkbyte_t> & is)
{
	std::unordered_set<std::unique_ptr<model::Game>> games;
	pugi::xml_document doc;
	if(doc.load(is))
	{
		pugi::xpath_node_set result = this->data->gamesListQuery.evaluate_node_set(doc.document_element());
		for(const pugi::xpath_node & node : result)
		{
			GameXML * game = new GameXML();
			game->id = this->data->gameIdQuery.evaluate_string(node);
			game->name = this->data->gameNameQuery.evaluate_string(node);

			// after trivial stuff was parsed, create out variables
			pugi::xpath_variable_set variables;

			variables.add("game.id", pugi::xpath_type_string);
			variables.add("gamen.ame", pugi::xpath_type_string);

			pugi::xpath_query gamePlatformIdsQuery(this->data->gamePlatformIdsPath.c_str(), &variables);

			variables.set("game.id", game->getId().c_str());
			variables.set("game.name", game->getName().c_str());

			// platform parsing is a little bit more tricky, because we get an array of names or ids
			for(const pugi::xpath_node & p : gamePlatformIdsQuery.evaluate_node_set(node))
			{
				std::string platform = this->data->platformIdQuery.evaluate_string(p);
				if(platform == this->data->platformWin32Id)
				{
					game->platforms.insert(model::Platform::WIN_32);
				}
				else if(platform == this->data->platformMac32Id)
				{
					game->platforms.insert(model::Platform::MAC_32);
				}
				else if(platform == this->data->platformLin32Id)
				{
					game->platforms.insert(model::Platform::LIN_32);
				}
				else if(platform == this->data->platformLin64Id)
				{
					game->platforms.insert(model::Platform::LIN_64);
				}
			}

			games.insert(std::move(std::unique_ptr<model::Game>(game)));
		}
	}
	return games;
}

GAMEKEEPER_NAMESPACE_END(backend)
