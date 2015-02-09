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

#include <gamekeeper/backend/xmlgamelistparser.h>

#include <pugixml.hpp>

#include <gamekeeper/model/genericgame.h>

GAMEKEEPER_NAMESPACE_START(backend)

class PRIVATE_API XpathQuery
{
public:
	XpathQuery(const std::string &);
	const std::unique_ptr<pugi::xpath_query> & operator->();
	operator bool();
private:
	std::unique_ptr<pugi::xpath_query> query;
};

XpathQuery::XpathQuery(const std::string & _query)
{
	if(!_query.empty())
	{
		query = std::make_unique<pugi::xpath_query>(_query.c_str());
	}
}

const std::unique_ptr<pugi::xpath_query> &
XpathQuery::operator->()
{
	return this->query;
}

XpathQuery::operator bool()
{
	return static_cast<bool>(this->query);
}

class XMLGameListParser::PImpl
{
public:
	PImpl(std::map<std::string, std::string> & config);
	XpathQuery gamesListQuery;
	XpathQuery gameIdQuery;
	XpathQuery gameNameQuery;
	XpathQuery gameDescriptionQuery;
	XpathQuery gameHomepageQuery;
	XpathQuery platformIdQuery;

	std::string gamePlatformIdsPath;

	std::string platformWin32Id;
	std::string platformMac32Id;
	std::string platformLin32Id;
	std::string platformLin64Id;
};

XMLGameListParser::PImpl::PImpl(std::map<std::string, std::string> & config)
:	gamesListQuery(config["games.list"].c_str()),
	gameIdQuery(config["game.id"].c_str()),
	gameNameQuery(config["game.name"].c_str()),
	gameDescriptionQuery(config["game.description"]),
	gameHomepageQuery(config["game.homepage"]),
	platformIdQuery(config["platform.id"].c_str()),
	gamePlatformIdsPath(config["game.platforms"]),
	platformWin32Id(config["platform.win32"]),
	platformMac32Id(config["platform.mac32"]),
	platformLin32Id(config["platform.lin32"]),
	platformLin64Id(config["platform.lin64"]){}

XMLGameListParser::XMLGameListParser(std::map<std::string, std::string> & config)
:	data(std::make_unique<PImpl>(config)){}

XMLGameListParser::~XMLGameListParser(){}

std::vector<std::unique_ptr<model::Game>>
XMLGameListParser::parseGameList(std::basic_istream<gkbyte_t> & is)
{
	std::vector<std::unique_ptr<model::Game>> games;
	pugi::xml_document doc;
	if(doc.load(is))
	{
		pugi::xpath_node_set result = this->data->gamesListQuery->evaluate_node_set(doc.document_element());
		for(const pugi::xpath_node & node : result)
		{
			std::unique_ptr<model::GenericGame> game = std::make_unique<model::GenericGame>();
			game->setId(this->data->gameIdQuery->evaluate_string(node));
			game->setName(this->data->gameNameQuery->evaluate_string(node));
			if(this->data->gameDescriptionQuery)
			{
				game->setDescription(this->data->gameDescriptionQuery->evaluate_string(node));
			}
			if(this->data->gameHomepageQuery)
			{
				game->setHomepage(this->data->gameHomepageQuery->evaluate_string(node));
			}

			// after trivial stuff was parsed, create out variables
			pugi::xpath_variable_set variables;

			variables.add("game.id", pugi::xpath_type_string);
			variables.add("game.name", pugi::xpath_type_string);
			variables.add("game.description", pugi::xpath_type_string);
			variables.add("game.homepage", pugi::xpath_type_string);

			pugi::xpath_query gamePlatformIdsQuery(this->data->gamePlatformIdsPath.c_str(), &variables);

			variables.set("game.id", game->getId().c_str());
			variables.set("game.name", game->getName().c_str());
			variables.set("game.description", game->getName().c_str());
			variables.set("game.homepage", game->getName().c_str());

			// platform parsing is a little bit more tricky, because we get an array of names or ids
			std::set<model::Platform> platforms;
			for(const pugi::xpath_node & p : gamePlatformIdsQuery.evaluate_node_set(node))
			{
				std::string platform = this->data->platformIdQuery->evaluate_string(p);
				if(platform == this->data->platformWin32Id)
				{
					platforms.insert(model::Platform::WIN_32);
				}
				else if(platform == this->data->platformMac32Id)
				{
					platforms.insert(model::Platform::MAC_32);
				}
				else if(platform == this->data->platformLin32Id)
				{
					platforms.insert(model::Platform::LIN_32);
				}
				else if(platform == this->data->platformLin64Id)
				{
					platforms.insert(model::Platform::LIN_64);
				}
			}
			game->setPlatforms(std::move(platforms));

			games.push_back(std::move(game));
		}
	}
	return games;
}

GAMEKEEPER_NAMESPACE_END(backend)
