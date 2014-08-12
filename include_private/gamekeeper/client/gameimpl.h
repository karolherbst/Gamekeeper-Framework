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

#ifndef GAMEKEEPER_CLIENT_GAMEIMPL_H
#define GAMEKEEPER_CLIENT_GAMEIMPL_H 1

#include <gamekeeper/core/common.h>

#include <memory>

#include <gamekeeper/client/game.h>

GAMEKEEPER_NAMESPACE_START(client)

class PRIVATE_API GameImpl : public Game
{
public:
	PRIVATE_API GameImpl(std::unique_ptr<model::Game> &&);
	PRIVATE_API ~GameImpl();
	GK_BUILD_GET_INTERFACE(GAME_MODEL, PRIVATE_API virtual, override)
private:
	class PRIVATE_API PImpl;
	std::unique_ptr<PImpl> data;
};

GAMEKEEPER_NAMESPACE_END(client)

#endif //GAMEKEEPER_CLIENT_GAME_H
