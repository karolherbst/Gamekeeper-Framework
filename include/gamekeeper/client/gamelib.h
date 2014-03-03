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

#ifndef GAMEKEEPER_CLIENT_GAMELIBUI_H
#define GAMEKEEPER_CLIENT_GAMELIBUI_H 1

#include <gamekeeper/core/common.h>

#include <gamekeeper/core/interface.h>

#include <Hypodermic/IContainer.h>

namespace boost
{
namespace program_options
{
class options_description_easy_init;
class variable_value;
}
}

GAMEKEEPER_NAMESPACE_START(core)
class Logger;
GAMEKEEPER_NAMESPACE_END(core)

GAMEKEEPER_NAMESPACE_START(client)

/**
 * @interface GameLibUI gamelib.h <gamekeeper/client/gamelib.h>
 *
 * Entry type for the gamelib library
 *
 * this interface has to be implemented by the specific Language Frontend. This Interface is not intented for client
 * usage at all. Refer to the specific language frontent API you want to use instead.
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API GameLibUI
{
	typedef std::map<std::string, boost::program_options::variable_value> ConfigMap;
	/**
	 * default destructor
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	PUBLIC_INLINE GAMEKEEPER_INTERFACE_DESTRUCTOR(GameLibUI)

	/**
	 * init method for GameLib
	 *
	 * this method should be used to initialize the GameLib client with default options and configurations. Keep sure,
	 * that after a call to this method, every other method must be ready to use.
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	PUBLIC_API GAMEKEEPER_INTERFACE_METHOD(void init(const ConfigMap & config));

	/**
	 * shutdown event handling method for GameLib
	 *
	 * clean up the GameLib client so that it can be destroyed or reused with a call to {@link #init} again.
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	PUBLIC_API GAMEKEEPER_INTERFACE_METHOD(void onShutdown());

	/**
	 * starts the implementation specific event loop
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @pre {@link #init} was called
	 * @post blocks the current thread of execution
	 */
	PUBLIC_API GAMEKEEPER_INTERFACE_METHOD(void startEventLoop());

	/**
	 * this method will be called to collect options used by the client
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param[in] oaCmd the OptionAdder object for command line options
	 * @param[in] oaFile the OptionAdder object for config file options
	 * @param[in] oaBoth the OptionAdder object for config config file and command line options
	 */
	PUBLIC_INLINE GAMEKEEPER_INTERFACE_METHOD_OPTIONAL(void addOptions(
	                                                boost::program_options::options_description_easy_init & oaCmd,
	                                                boost::program_options::options_description_easy_init & oaFile,
	                                                boost::program_options::options_description_easy_init & oaBoth));
};

/**
 * is used by {@link main()} to get a language specific implementation of {@link GameLib}
 *
 * This method is provided by the GUI client, not gamelib itself.
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param logger the logger provided by gamelib
 * @return a new created instance of gameLib
 */
extern "C" REVERSE_PUBLIC_API GameLibUI * newInstance(gamelib::core::Logger& logger);

/**
 * declares the Implementation of the {@link GameLibUI} interface
 *
 * use this by the client to provide all the needed things.
 */
#define GAMECLIENTUI_CLASS(class) \
extern "C" REVERSE_PUBLIC_API gamelib::client::GameLibUI * newInstance(gamelib::core::Logger& logger) \
{ \
	return new class(logger); \
}

GAMEKEEPER_NAMESPACE_END(client)

#endif //GAMEKEEPER_CLIENT_GAMELIBUI_H
