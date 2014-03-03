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

#ifndef GAMEKEEPER_BINDINGS_MODEL_GAME_H
#define GAMEKEEPER_BINDINGS_MODEL_GAME_H 1

#include <gamelib/core/common.h>

#include <string>

#include <gamelib/model/game.h>

GAMEKEEPER_NAMESPACE_START(bindings)
namespace model
{

class PUBLIC_API Game : public gamelib::model::Game
{
private:
	std::string id;
	std::string name;
	
public:
	PRIVATE_API GAMELIB_IMPLEMENTATION_OVERRIDE(const char* getId() const);
	PRIVATE_API GAMELIB_IMPLEMENTATION_OVERRIDE(const char* getName() const);
	
	PUBLIC_API void setId(const char*);
	PUBLIC_API void setName(const char*);
};

}
GAMEKEEPER_NAMESPACE_END(bindings)

#endif //GAMEKEEPER_BINDINGS_MODEL_GAME_H
