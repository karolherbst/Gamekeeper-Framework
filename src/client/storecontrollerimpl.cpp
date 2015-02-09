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

#include <gamekeeper/backend/storemanager.h>
#include <gamekeeper/client/storeimpl.h>
#include <gamekeeper/client/storecontrollerimpl.h>

GAMEKEEPER_NAMESPACE_START(client)

StoreControllerImpl::StoreControllerImpl(std::shared_ptr<gamekeeper::backend::StoreManager> _sm)
:	sm(std::move(_sm)){}

std::vector<std::unique_ptr<Store>>
StoreControllerImpl::getAll()
{
	std::vector<std::unique_ptr<Store>> result;
	for(auto & s : this->sm->getAllStores())
	{
		result.push_back(std::make_unique<StoreImpl>(s));
	}
	return result;
}

std::unique_ptr<Store>
StoreControllerImpl::get(const std::string & name)
{
	return std::make_unique<StoreImpl>(this->sm->getStore(name));
}

GAMEKEEPER_NAMESPACE_END(client)
