/*
 * libgamelib
 *
 * Copyright (C) 2013 Karol Herbst <gamelib@karolherbst.de>
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

#ifndef GAMELIB_CLIENT_HYPODERMIC_H
#define GAMELIB_CLIENT_HYPODERMIC_H 1

#include <gamelib/core/common.h>

#include <Hypodermic/IContainer.h>

GAMELIB_NAMESPACE_START(client)

/**
 * @class HypodermicUtil hypodermic.h <gamelib/client/hypodermic.h>
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
	 * this method can be only called after the GamelibUI.init method was called
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @return the current hypodermic container instance
	 */
	PUBLIC_API static Hypodermic::IContainer& getContainer();
};

GAMELIB_NAMESPACE_END(client)

#endif //GAMELIB_CLIENT_HYPODERMIC_H
