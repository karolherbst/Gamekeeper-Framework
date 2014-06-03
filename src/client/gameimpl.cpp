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

#include <gamekeeper/client/gameimpl.h>

GAMEKEEPER_NAMESPACE_START(client)

class GameImpl::PImpl
{
public:
	PImpl(std::unique_ptr<model::Game> &&);
	std::unique_ptr<model::Game> game;
};

GameImpl::PImpl::PImpl(std::unique_ptr<model::Game> && model)
:	game(std::move(model)){}

GameImpl::GameImpl(std::unique_ptr<model::Game> && model)
:	data(new PImpl(std::move(model))){

}

GameImpl::~GameImpl(){}

const std::string &
GameImpl::getId() const
{
	return this->data->game->getId();
}

const std::string &
GameImpl::getName() const
{
	return this->data->game->getName();
}

const std::set<model::Platform> &
GameImpl::getPlatforms() const
{
	return this->data->game->getPlatforms();
}

GAMEKEEPER_NAMESPACE_END(client)
