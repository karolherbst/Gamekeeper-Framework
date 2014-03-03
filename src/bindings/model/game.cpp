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

#include "game.h"

GAMEKEEPER_NAMESPACE_START(bindings)
namespace model
{

const char*
Game::getId() const
{
	return this->id.c_str();
}

const char*
Game::getName() const
{
	return this->name.c_str();
}

void
Game::setId(const char* id)
{
	this->id = id;
}

void
Game::setName(const char* n)
{
	this->name = n;
}

}
GAMEKEEPER_NAMESPACE_END(bindings)
