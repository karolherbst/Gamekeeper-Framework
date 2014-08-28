/*
 * GameKeeper Framework
 *
 * Copyright (C) 2013 - 2014 Karol Herbst <gamekeeper@karolherbst.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Linking against the GameKeeper Framework statically or dynamically with
 * other modules is making a combined work based on the GameKeeper Framework.
 * Thus, the terms and conditions of the GNU General Public License cover the
 * whole combination.
 *
 * In addition, as a special exception, the copyright holders of the GameKeeper
 * Framework give you permission to copy and distribute a work based on the
 * GameKeeper Framework in object code or executable form without the
 * requirements to accompany it with the source code, provided that you meet
 * all of these conditions:
 *
 *   a) The dervied work links with the GameKeeper Framework solely through
 *      symbols defined in any file in the /include directory and its
 *      subdirectories.
 *
 *   b) The dervied work does not link against symbols of the GameKeeper
 *      Framework outside the gamekeeper::client, gamekeeper::core and
 *      gamekeeper::model C++ namespaces.
 *
 * You may copy and distribute such a system
 * following the terms of the GNU GPL for the GameKeeper Framework and the licenses
 * of the other code concerned, provided that you include the source code of that other
 * code when and as the GNU GPL requires distribution of source code.
 *
 * Note that people who make modified versions of the GameKeeper Framework are
 * not obligated to grant this special exception for their modified versions;
 * it is their choice whether to do so.  The GNU General Public License gives
 * permission to release a modified version without this exception; this
 * exception also makes it possible to release a modified version which
 * carries forward this exception.
 */

#ifndef GAMEKEEPER_CLIENT_GAMEKEEPER_H
#define GAMEKEEPER_CLIENT_GAMEKEEPER_H 1

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
interface Logger;
GAMEKEEPER_NAMESPACE_END(core)

GAMEKEEPER_NAMESPACE_START(client)

/**
 * @interface GameKeeperUI gamekeeper.h <gamekeeper/client/gamekeeper.h>
 *
 * Entry type for the GameKeeper Framework
 *
 * this interface has to be implemented by the specific Language Frontend. This Interface is not intented for client
 * usage at all. Refer to the specific language frontent API you want to use instead.
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API GameKeeperUI
{
	GAMEKEEPER_INTERFACE_METHODS(GameKeeperUI);

	typedef std::map<std::string, boost::program_options::variable_value> ConfigMap;

	/**
	 * init method for GameKeeper
	 *
	 * this method should be used to initialize the GameKeeper client with default options and configurations. Keep sure,
	 * that after a call to this method, every other method must be ready to use.
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	PUBLIC_API virtual void init(const ConfigMap & config) = 0;

	/**
	 * shutdown event handling method for GameKeeper
	 *
	 * clean up the GameKeeper client so that it can be destroyed or reused with a call to {@link #init} again.
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	PUBLIC_API virtual void onShutdown() = 0;

	/**
	 * starts the implementation specific event loop
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @pre {@link #init} was called
	 * @post blocks the current thread of execution
	 */
	PUBLIC_API virtual void startEventLoop() = 0;
};

/**
 * is used by {@link main()} to get a language specific implementation of {@link GameKeeper}
 *
 * This method is provided by the GUI client, not GameKeeper itself.
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param logger the logger provided by GameKeeper
 * @return a new created instance of GameKeeper
 */
extern "C" REVERSE_PUBLIC_API GameKeeperUI * newInstance(gamekeeper::core::Logger& logger);

/**
 * declares the Implementation of the {@link GameKeeperUI} interface
 *
 * use this by the client to provide all the needed things.
 */
#define GAMECLIENTUI_CLASS(class) \
extern "C" REVERSE_PUBLIC_API gamekeeper::client::GameKeeperUI * newInstance(gamekeeper::core::Logger& logger) \
{ \
	return new class(logger); \
}

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
extern "C" REVERSE_PUBLIC_API void addOptions(boost::program_options::options_description_easy_init & oaCmd,
                                              boost::program_options::options_description_easy_init & oaFile,
                                              boost::program_options::options_description_easy_init & oaBoth);

#define GAMECLIENT_ADD_OPTIONS(body) \
extern "C" REVERSE_PUBLIC_API void \
addOptions(boost::program_options::options_description_easy_init & cmd, \
           boost::program_options::options_description_easy_init & file, \
           boost::program_options::options_description_easy_init & both) body

GAMEKEEPER_NAMESPACE_END(client)

#endif //GAMEKEEPER_CLIENT_GAMEKEEPER_H
