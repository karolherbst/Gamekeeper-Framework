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

#include <memory>
#include <vector>

#include <gamekeeper/client/store.h>
#include <gamekeeper/core/interface.h>

GAMEKEEPER_NAMESPACE_START(client)

/**
 * @interface StoreController storecontroller.h <gamekeeper/client/storecontroller.h>
 *
 * This interface provides an easy way to access stores
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API StoreController
{
public:
	GAMEKEEPER_INTERFACE_METHODS(StoreController);

	/**
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @return list of all stores
	 */
	PUBLIC_API virtual std::vector<std::unique_ptr<Store>> getAll() = 0;

	/**
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @parem[in] name the name of the store
	 * @return the store with the given name
	 */
	PUBLIC_API virtual std::unique_ptr<Store> get(const std::string & name) = 0;
};

GAMEKEEPER_NAMESPACE_END(client)
