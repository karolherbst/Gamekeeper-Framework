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

#ifndef GAMEKEEPER_MODEL_STORE_H
#define GAMEKEEPER_MODEL_STORE_H 1

#include <gamekeeper/core/interface.h>

#include <map>

#include <gamekeeper/model/macros.h>

GAMEKEEPER_NAMESPACE_START(model)

#define STORE_MODEL ( \
	(std::string, name, Name), \
	(gamekeeper::model::Store::ConfigMap, config, Config))

interface PUBLIC_API Store
{
	typedef std::map<std::string,std::string> ConfigMap;
	GAMEKEEPER_INTERFACE_METHODS(Store);
	GK_BUILD_GET_INTERFACE(STORE_MODEL, PUBLIC_API virtual, = 0)
};

GAMEKEEPER_NAMESPACE_END(model)

#endif //GAMEKEEPER_MODEL_GAME_H
