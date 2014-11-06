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

#ifndef GAMEKEEPER_CLIENT_STORE_H
#define GAMEKEEPER_CLIENT_STORE_H 1

#include <gamekeeper/core/common.h>

#include <memory>
#include <vector>

#include <gamekeeper/client/game.h>
#include <gamekeeper/model/store.h>

GAMEKEEPER_NAMESPACE_START(client)

interface PUBLIC_API Store : public model::Store
{
public:
	PUBLIC_API virtual bool login(const std::string & method, const std::string & username, const std::string & password /*, TwoFactorAuthCallback for later usage */) = 0;
	PUBLIC_API virtual bool logout() = 0;
	PUBLIC_API virtual bool isLoggedIn() const = 0;
	PUBLIC_API virtual std::vector<std::string> supportedLoginMethods() = 0;
	PUBLIC_API virtual std::vector<std::unique_ptr<Game>> getAllGames() = 0;
};

GAMEKEEPER_NAMESPACE_END(client)

#endif //GAMEKEEPER_CLIENT_STORE_H
