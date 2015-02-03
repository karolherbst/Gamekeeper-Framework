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

#pragma once

#include <gamekeeper/core/common.h>

#include <memory>
#include <vector>

#include <gamekeeper/core/exception.h>
#include <gamekeeper/core/interface.h>

GAMEKEEPER_NAMESPACE_START(model)

interface Game;

GAMEKEEPER_NAMESPACE_END(model)

GAMEKEEPER_NAMESPACE_START(backend)

/**
 * @interface GameListParser gamelistparser.h <gamekeeper/core/gamelistparser.h>
 *
 * This interfase should be used to implement GameListParser
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API GameListParser
{
	GAMEKEEPER_INTERFACE_METHODS(GameListParser);

	/**
	 * parses games from the input stream
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param[in] is the input stream
	 * @return unsorted list of games
	 */
	PUBLIC_API virtual std::vector<std::unique_ptr<model::Game>> parseGameList(std::basic_istream<gkbyte_t> & is) = 0;

	class PUBLIC_API GAMEKEEPER_EXCEPTION(GameListParserException);
};

GAMEKEEPER_NAMESPACE_END(backend)
