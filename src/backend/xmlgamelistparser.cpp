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
public:
	const char * getId() const
	{
		return this->id.c_str();
	}

	const char * getName() const
	{
		return this->name.c_str();
	}
};

class XMLGameListParser::PImpl
{
public:
	PImpl(std::map<std::string, std::string> & config);
	pugi::xpath_query gamesListQuery;
	pugi::xpath_query gameIdQuery;
	pugi::xpath_query gameNameQuery;
};

XMLGameListParser::PImpl::PImpl(std::map<std::string, std::string> & config)
:	gamesListQuery(config.at("games.list").c_str()),
	gameIdQuery(config.at("game.id").c_str()),
	gameNameQuery(config.at("game.name").c_str()){}

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
			games.insert(std::move(std::unique_ptr<model::Game>(game)));
		}
	}
	return games;
}

GAMEKEEPER_NAMESPACE_END(backend)
