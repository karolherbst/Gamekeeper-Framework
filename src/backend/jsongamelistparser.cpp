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

#include <gamekeeper/backend/jsongamelistparser.h>

#include <jansson.h>
#include <jansson_path.h>

#include <gamekeeper/model/game.h>
#include <gamekeeper/utils/stringutils.h>

GAMEKEEPER_NAMESPACE_START(backend)

class GameJSON : public gamekeeper::model::Game
{
	friend class JSONGameListParser;
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

class JSONGameListParser::PImpl
{
public:
	PImpl(std::map<std::string, std::string> & config);
	std::string gamesListPath;
	std::string gameIdPath;
	std::string gameNamePath;
};

JSONGameListParser::PImpl::PImpl(std::map<std::string, std::string> & config)
:	gamesListPath(config.at("games.list")),
	gameIdPath(config.at("game.id")),
	gameNamePath(config.at("game.name")){}

JSONGameListParser::JSONGameListParser(std::map<std::string, std::string> & config)
:	data(new JSONGameListParser::PImpl(config)){}

std::unordered_set<std::unique_ptr<model::Game>>
JSONGameListParser::parseGameList(std::basic_istream<gkbyte_t> & is)
{
	json_t * root = nullptr;
	json_error_t error;
	std::unordered_set<std::unique_ptr<model::Game>> games;

	{
		std::string jsonTree = utils::String::createFromStream(is);
		root = json_loads(jsonTree.c_str(), 0, &error);
	}

	if(root)
	{
		json_t * gamesList = json_path_get(root, this->data->gamesListPath.c_str());
		if(json_is_array(gamesList))
		{
			for(size_t i = 0; i < json_array_size(gamesList); i++)
			{
				json_t * gameNode = json_array_get(gamesList, i);
				GameJSON * game = new GameJSON();
				game->id = json_string_value(json_path_get(gameNode, this->data->gameIdPath.c_str()));
				game->name = json_string_value(json_path_get(gameNode, this->data->gameNamePath.c_str()));
				games.insert(std::move(std::unique_ptr<model::Game>(game)));
			}
		}
	}
	json_decref(root);
	return games;
}

GAMEKEEPER_NAMESPACE_END(backend)
