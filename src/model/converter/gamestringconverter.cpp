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

#include <gamekeeper/model/converter/gamestringconverter.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>

#include <gamekeeper/model/genericgame.h>
#include <gamekeeper/utils/stringutils.h>

namespace balgo = boost::algorithm;

GAMEKEEPER_NAMESPACE_START(model)

std::unique_ptr<Game>
GameStringConverter::convert(const std::unordered_map<std::string,std::string>& map)
{
	std::set<model::Platform> ps;
	{
		std::set<std::string> pstmp;
		balgo::split(pstmp, map.find("game.platforms")->second, balgo::is_any_of(","), balgo::token_compress_on);
		for(const std::string & p : pstmp)
		{
			ps.insert(utils::String::toType<model::Platform>(p));
		}
	}

	GenericGame * game = new GenericGame();
	game->setId(map.at("game.id"));
	game->setName(map.at("game.name"));
	game->setDescription(map.find("game.description")->second);
	game->setHomepage(map.find("game.homepage")->second);
	game->setPlatforms(ps);
	return std::unique_ptr<Game>(game);
}

std::unordered_map<std::string,std::string>
GameStringConverter::convert(const Game & game)
{
	std::vector<std::string> ps;
	for(const model::Platform & p : game.getPlatforms())
	{
		ps.push_back(utils::String::toString(p));
	}
	return {
		{"game.id", game.getId()},
		{"game.name", game.getName()},
		{"game.description", game.getDescription()},
		{"game.homepage", game.getHomepage()},
		{"game.platforms", balgo::join(ps, ",")}
	};
}

std::unordered_map<std::string,std::string>
GameStringConverter::convert(const Game * const game)
{
	return convert(*game);
}

GAMEKEEPER_NAMESPACE_END(model)
