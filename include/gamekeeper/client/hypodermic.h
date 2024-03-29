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

#include <Hypodermic/IContainer.h>

GAMEKEEPER_NAMESPACE_START(client)

/**
 * @class HypodermicUtil hypodermic.h <gamekeeper/client/hypodermic.h>
 *
 * class to optiain a hypodermic container instance
 *
 * @author Karol Herbst
 * @since 0
 */
class PUBLIC_API HypodermicUtil
{
public:
	/**
	 * use this method to get the globale hypodermic container
	 *
	 * this method can be only called after the GameKeeperUI.init method was called
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @return the current hypodermic container instance
	 */
	PUBLIC_API static Hypodermic::IContainer& getContainer();
};

GAMEKEEPER_NAMESPACE_END(client)
