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

#ifndef GAMELIB_GAMELIB_H
#define GAMELIB_GAMELIB_H 1

#include <gamelib/core/common.h>

#include <gamelib/core/interface.h>

#include <boost/property_tree/ptree_fwd.hpp>

GAMELIB_NAMESPACE_START(client)

/**
 * @interface GameLib gamelib.h <gamelib/client/gamelib.h>
 *
 * Entry type for the gamelib library
 *
 * this interface has to be implemented by the specific Language Frontend. This Interface is not intented for client
 * usage at all. Refer to the specific language frontent API you want to use instead.
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API GameLib
{
	/**
	 * default destructor
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	PUBLIC_API GAMELIB_INTERFACE_DESTRUCTOR(GameLib)
	
	/**
	 * init method for GameLib
	 *
	 * this method should be used to initialize GameLib with default options and configurations. Keep sure, that after 
	 * a call to this method, every other method must be ready to use.
	 *
	 * {@link #setConfig} doesn't have to be called before.
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	PUBLIC_API GAMELIB_INTERFACE_METHOD(void init())
	
	/**
	 * shutdown method for GameLib
	 *
	 * clean up GameLib so that it can be reused with a call to {@link #init} again.
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	PUBLIC_API GAMELIB_INTERFACE_METHOD(void shutdown())
	
	/**
	 * sets the config of the current GameLib instance
	 *
	 * this method is called usually after it was constructed. It may be called after a shutdown call or to reconfigure
	 * GameLib at runtime.
	 *
	 * It is recommend to store a copy of the passed object to create diff objects, so you can trigger updates only
	 * for changed properties.
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param[in] config the property tree with all the configuration properties.
	 */
	PUBLIC_API GAMELIB_INTERFACE_METHOD(void setConfig(boost::property_tree::ptree& config))
	
	/**
	 * starts the implementation specific event loop
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @pre {@link #init} was called
	 * @post blocks the current thread of execution
	 *
	 * @param[in] config the property tree with all the configuration properties.
	 */
	PUBLIC_API GAMELIB_INTERFACE_METHOD(void startEventLoop())
}

/**
 * is used by {@link main()} to get a language specific implementation of {@link GameLib}
 *
 * This method is provided by the language frontend library, not gamelib itself.
 *
 * @author Karol Herbst
 * @since 0
 *
 * @return a new created instance of gameLib
 */
GameLib * newInstance();

GAMELIB_NAMESPACE_END(client)

#endif //GAMELIB_GAMELIB_H
