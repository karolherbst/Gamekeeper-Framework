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

class JSONGameListParser::PImpl
{
public:
	PImpl(std::map<std::string, std::string> & config);
	std::string gamesListPath;
	std::string gameIdPath;
	std::string gameNamePath;
	std::string gamePlatformsPath;
	std::string platformId;
	std::string platformWin32Id;
	std::string platformMac32Id;
	std::string platformLin32Id;
	std::string platformLin64Id;
};

JSONGameListParser::PImpl::PImpl(std::map<std::string, std::string> & config)
:	gamesListPath(config.at("games.list")),
	gameIdPath(config.at("game.id")),
	gameNamePath(config.at("game.name")),
	gamePlatformsPath(config.at("game.platforms")),
	platformId(config.at("platform.id")),
	platformWin32Id(config.at("platform.win32")),
	platformMac32Id(config.at("platform.mac32")),
	platformLin32Id(config.at("platform.lin32")),
	platformLin64Id(config["platform.lin64"]){}

JSONGameListParser::JSONGameListParser(std::map<std::string, std::string> & config)
:	data(new JSONGameListParser::PImpl(config)){}

std::vector<std::unique_ptr<model::Game>>
JSONGameListParser::parseGameList(std::basic_istream<gkbyte_t> & is)
{
	json_t * root = nullptr;
	json_error_t error;
	std::vector<std::unique_ptr<model::Game>> games;

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

				json_t * gamePlatforms = json_path_get(gameNode, this->data->gamePlatformsPath.c_str());

				for(size_t p = 0; p < json_array_size(gamePlatforms); p++)
				{
					std::string platform = json_string_value(json_path_get(json_array_get(gamePlatforms, p), "$"));
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

				games.push_back(std::move(std::unique_ptr<model::Game>(game)));
			}
		}
	}
	json_decref(root);
	return games;
}

GAMEKEEPER_NAMESPACE_END(backend)
