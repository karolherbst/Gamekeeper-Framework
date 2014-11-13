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

#ifndef GAMEKEEPER_BACKEND_STORECONFIGDATA_H
#define GAMEKEEPER_BACKEND_STORECONFIGDATA_H 1

#include <gamekeeper/core/common.h>

#include <string>
#include <vector>

GAMEKEEPER_NAMESPACE_START(backend)

struct PUBLIC_API StoreConfigData
{
	struct Store
	{
		std::string homepage;
		std::string name;
		std::string format;
	};

	struct Auth
	{
		std::string method;
		std::string loginurl;
		std::string logouturl;
	};

	struct Authfield
	{
		std::string username;
		std::string password;
	};

	struct Authtoken
	{
		std::vector<std::string> keys;
	};

	struct Games
	{
		std::string url;
		std::string list;
	};

	struct Game
	{
		std::string id;
		std::string name;
		std::string description;
		std::string homepage;
		std::string platforms;
	};

	struct Platform
	{
		std::string id;
		std::string win32;
		std::string mac32;
		std::string lin32;
		std::string lin64;
	};

	Store store;
	Auth auth;
	Authfield authfield;
	Authtoken authtoken;
	Games games;
	Game game;
	Platform platform;
};

GAMEKEEPER_NAMESPACE_END(backend)

#endif //GAMEKEEPER_BACKEND_STORECONFIGDATA_H
