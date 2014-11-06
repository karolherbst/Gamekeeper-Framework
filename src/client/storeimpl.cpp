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

#include <gamekeeper/client/storeimpl.h>

#include <gamekeeper/backend/gamelistparser.h>
#include <gamekeeper/backend/loginhandler.h>
#include <gamekeeper/backend/storeconfiguration.h>
#include <gamekeeper/client/gameimpl.h>

GAMEKEEPER_NAMESPACE_START(client)

class StoreImpl::PImpl
{
public:
	PImpl(backend::StoreConfiguration *);
	std::unique_ptr<backend::StoreConfiguration> config;
	std::shared_ptr<backend::LoginHandler> getLoggedInLoginHandler();
private:
	std::shared_ptr<backend::LoginHandler> currentLoginHandler;
};

StoreImpl::PImpl::PImpl(backend::StoreConfiguration * _config)
:	config(_config)
{
	// check if there is already a current loginHandler
	getLoggedInLoginHandler();
}

std::shared_ptr<backend::LoginHandler>
StoreImpl::PImpl::getLoggedInLoginHandler()
{
	if(this->currentLoginHandler && this->currentLoginHandler->isLoggedIn())
	{
		return this->currentLoginHandler;
	}

	for(const auto & lh : this->config->getLoginHandlers())
	{
		if(lh.second->isLoggedIn())
		{
			this->currentLoginHandler = lh.second;
			return lh.second;
		}
	}
	return nullptr;
}

StoreImpl::StoreImpl(const backend::StoreConfiguration & _config)
:	data(new StoreImpl::PImpl(new backend::StoreConfiguration(_config))){}

GK_BUILD_GET_IMPLEMENTATION_WRAPPER(STORE_MODEL, StoreImpl,, this->data->config->getStore())

bool
StoreImpl::login(const std::string & method, const std::string & username, const std::string & password /*, TwoFactorAuthCallback for later usage */)
{
	return this->data->config->getLoginHandlers().at(method)->login(username, password);
}

bool
StoreImpl::logout()
{
	this->data->getLoggedInLoginHandler()->logout();
	return true;
}

bool
StoreImpl::isLoggedIn() const
{
	// if there is a handler, we should be logged in
	return static_cast<bool>(this->data->getLoggedInLoginHandler());
}

std::vector<std::string>
StoreImpl::supportedLoginMethods()
{
	std::vector<std::string> result;
	for(const auto & p : this->data->config->getLoginHandlers())
	{
		result.push_back(p.first);
	}
	return result;
}

std::vector<std::unique_ptr<Game>>
StoreImpl::getAllGames()
{
	std::vector<std::unique_ptr<Game>> result;

	this->data->getLoggedInLoginHandler()->downloadFile(this->getConfig().at("games.url"),
	[this,&result](std::basic_istream<gkbyte_t> & is) -> bool
	{
		for(auto & g : this->data->config->getGameListParser()->parseGameList(is))
		{
			result.push_back(std::move(std::unique_ptr<Game>(new GameImpl(std::move(g)))));
		}
		return true;
	});

	return result;
}

GAMEKEEPER_NAMESPACE_END(client)
