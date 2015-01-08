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

#ifndef GAMEKEEPER_MODEL_GENERICGAME_H
#define GAMEKEEPER_MODEL_GENERICGAME_H 1

#include <gamekeeper/core/common.h>

#include <memory>
#include <set>

#include <gamekeeper/model/game.h>
#include <gamekeeper/model/macros.h>

GAMEKEEPER_NAMESPACE_START(model)

class PUBLIC_API GenericGame : public Game
{
public:
	PUBLIC_API GenericGame();
	PUBLIC_API ~GenericGame();
	GK_BUILD_GET_INTERFACE(GAME_MODEL, PUBLIC_API virtual, override)
	GK_BUILD_SET_INTERFACE(GAME_MODEL, PUBLIC_API,)
private:
	class PRIVATE_API PImpl;
	std::unique_ptr<PImpl> data;
};

GAMEKEEPER_NAMESPACE_END(model)

#endif //GAMEKEEPER_MODEL_GAME_H
