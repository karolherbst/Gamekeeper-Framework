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

#ifndef GAMEKEEPER_BACKEND_STORECONFIGURATION_H
#define GAMEKEEPER_BACKEND_STORECONFIGURATION_H 1

#include <gamekeeper/core/common.h>

#include <map>
#include <memory>

#include <gamekeeper/model/store.h>

GAMEKEEPER_NAMESPACE_START(backend)

class GameListParser;
class LoginHandler;

struct PUBLIC_API StoreConfiguration
{
	typedef std::map<std::string, std::shared_ptr<LoginHandler>> LoginHandlerMap;

	PUBLIC_API StoreConfiguration(std::shared_ptr<GameListParser>, const LoginHandlerMap &, std::shared_ptr<model::Store>);
	PUBLIC_API StoreConfiguration(const StoreConfiguration &);
	PUBLIC_API StoreConfiguration(StoreConfiguration &&);
	PUBLIC_API std::shared_ptr<model::Store> getStore();
	PUBLIC_API const LoginHandlerMap & getLoginHandlers();
	PUBLIC_API std::shared_ptr<GameListParser> getGameListParser();
private:
	class PRIVATE_API PImpl;
	std::shared_ptr<PImpl> data;
};

GAMEKEEPER_NAMESPACE_END(backend)

#endif //GAMEKEEPER_BACKEND_STORECONFIGURATION_H
