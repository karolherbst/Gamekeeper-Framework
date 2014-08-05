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

#ifndef GAMEKEEPER_BACKEND_XMLGAMELISTPARSER_H
#define GAMEKEEPER_BACKEND_XMLGAMELISTPARSER_H 1

#include <gamekeeper/core/common.h>

#include <map>

#include <gamekeeper/backend/gamelistparser.h>

GAMEKEEPER_NAMESPACE_START(backend)

class PUBLIC_API XMLGameListParser : public GameListParser
{
public:
	PUBLIC_API XMLGameListParser(std::map<std::string, std::string> & config);
	PUBLIC_API ~XMLGameListParser();
	PRIVATE_API virtual std::vector<std::unique_ptr<model::Game>> parseGameList(std::basic_istream<gkbyte_t> &) override;
private:
	class PRIVATE_API PImpl;

	std::unique_ptr<XMLGameListParser::PImpl> data;
};

GAMEKEEPER_NAMESPACE_END(backend)

#endif //GAMEKEEPER_BACKEND_XMLGAMELISTPARSER_H
