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

#include <gamekeeper/backend/storeconfiguration.h>

#include <gamekeeper/backend/gamelistparser.h>
#include <gamekeeper/backend/loginhandler.h>

GAMEKEEPER_NAMESPACE_START(backend)

class StoreConfiguration::PImpl
{
public:
	PImpl(std::shared_ptr<GameListParser>, std::shared_ptr<LoginHandler>, std::shared_ptr<model::Store>);
	std::shared_ptr<GameListParser> glp;
	std::shared_ptr<model::Store> store;
	std::shared_ptr<LoginHandler> lh;
};

StoreConfiguration::PImpl::PImpl(std::shared_ptr<GameListParser> _glp, std::shared_ptr<LoginHandler> _lh, std::shared_ptr<model::Store> _store)
:	glp(std::move(_glp)),
	store(std::move(_store)),
	lh(std::move(_lh)){}

StoreConfiguration::StoreConfiguration(std::shared_ptr<GameListParser> glp, std::shared_ptr<LoginHandler> lh, std::shared_ptr<model::Store> store)
:	data(std::make_shared<PImpl>(glp, lh, store)){}

StoreConfiguration::StoreConfiguration(const StoreConfiguration & sc)
:	data(sc.data){}

StoreConfiguration::StoreConfiguration(StoreConfiguration && sc)
:	data(std::move(sc.data)){}

std::shared_ptr<GameListParser>
StoreConfiguration::getGameListParser()
{
	return this->data->glp;
}

std::shared_ptr<LoginHandler>
StoreConfiguration::getLoginHandler()
{
	return this->data->lh;
}

std::shared_ptr<model::Store>
StoreConfiguration::getStore()
{
	return this->data->store;
}

GAMEKEEPER_NAMESPACE_END(backend)
