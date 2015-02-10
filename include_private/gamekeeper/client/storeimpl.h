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

#include <vector>

#include <gamekeeper/client/store.h>

GAMEKEEPER_NAMESPACE_START(backend)

class StoreConfiguration;

GAMEKEEPER_NAMESPACE_END(backend)

GAMEKEEPER_NAMESPACE_START(client)

interface PRIVATE_API StoreImpl : public client::Store
{
public:
	PRIVATE_API StoreImpl(const backend::StoreConfiguration &);
	PRIVATE_API ~StoreImpl();
	GK_BUILD_GET_INTERFACE(STORE_MODEL, PRIVATE_API virtual, override)
	PRIVATE_API virtual bool login(const std::string & username, const std::string & password /*, TwoFactorAuthCallback for later usage */) override;
	PRIVATE_API virtual bool logout() override;
	PRIVATE_API virtual bool isLoggedIn() const override;
	PRIVATE_API virtual std::vector<std::unique_ptr<Game>> getAllGames() override;
private:
	class PRIVATE_API PImpl;
	std::unique_ptr<StoreImpl::PImpl> data;
};

GAMEKEEPER_NAMESPACE_END(client)
