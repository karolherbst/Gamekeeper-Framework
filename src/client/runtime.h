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

#ifndef GAMELIB_CLIENT_RUNTIME_H
#define GAMELIB_CLIENT_RUNTIME_H 1

#include <gamelib/core/common.h>

#include <memory>

GAMEKEEPER_NAMESPACE_START(core)

class Logger;

GAMEKEEPER_NAMESPACE_END(core)

GAMEKEEPER_NAMESPACE_START(client)

class GameLibUI;

class PUBLIC_API GameLibRuntime
{
public:
	PUBLIC_API GameLibRuntime();
	PUBLIC_API ~GameLibRuntime();
	PUBLIC_API gamelib::core::Logger& getUILogger();
	PUBLIC_API int main(int argc, const char* argv[], GameLibUI *);
private:
	GameLibUI * gameLibUI;
};

GAMEKEEPER_NAMESPACE_END(client)

#endif //GAMELIB_CLIENT_RUNTIME_H
