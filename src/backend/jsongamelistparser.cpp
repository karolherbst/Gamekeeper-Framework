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

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <json/reader.h>
#include <json/value.h>

#include <gamekeeper/model/game.h>

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

static std::string
convertJsonPathToJsonCppPath(std::string path)
{
	// we handle root as current
	boost::replace_all(path, "$", "@");
	boost::replace_all(path, "@.", ".");

	// we don't have any @. anymore
	boost::replace_all(path, "@", ".");

	// if we have [*] at the end, it indicates an array only, but we don't use it
	if(boost::ends_with(path, "[*]"))
	{
		boost::replace_all(path, "[*]", "");
	}
	return path;
}

class JSONGameListParser::PImpl
{
public:
	PImpl(std::map<std::string, std::string> & config);
	Json::Path gamesListPath;
	Json::Path gameIdPath;
	Json::Path gameNamePath;
};

JSONGameListParser::PImpl::PImpl(std::map<std::string, std::string> & config)
:	gamesListPath(convertJsonPathToJsonCppPath(config.at("games.list"))),
	gameIdPath(convertJsonPathToJsonCppPath(config.at("game.id"))),
	gameNamePath(convertJsonPathToJsonCppPath(config.at("game.name"))){}

JSONGameListParser::JSONGameListParser(std::map<std::string, std::string> & config)
:	data(new JSONGameListParser::PImpl(config)){}

std::unordered_set<std::unique_ptr<model::Game>>
JSONGameListParser::parseGameList(std::basic_istream<gkbyte_t> & is)
{
	Json::Value root;
	Json::Reader reader;
	std::unordered_set<std::unique_ptr<model::Game>> games;

	if(reader.parse(is, root, false))
	{
		Json::Value gamesList = this->data->gamesListPath.resolve(root);
		for(const Json::Value & gameNode : gamesList)
		{
			GameJSON * game = new GameJSON();
			game->id = this->data->gameIdPath.resolve(gameNode).asString();
			game->name = this->data->gameNamePath.resolve(gameNode).asString();
			games.insert(std::move(std::unique_ptr<model::Game>(game)));
		}
	}
	return games;
}

GAMEKEEPER_NAMESPACE_END(backend)
