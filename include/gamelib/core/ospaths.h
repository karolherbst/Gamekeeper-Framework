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

#ifndef GAMELIB_CORE_OSPATHS_H
#define GAMELIB_CORE_OSPATHS_H 1

#include <gamelib/core/common.h>

#include <gamelib/core/interface.h>

#include <boost/filesystem/path.hpp>

GAMELIB_NAMESPACE_START(core)

/**
 * @interface OSPaths ospaths.h <gamelib/core/ospaths.h>
 *
 * this interface provides all methods to get OS specific paths like where should config files be stored
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API OSPaths
{
	PUBLIC_API GAMELIB_INTERFACE_DESTRUCTOR(OSPaths)

	/**
	 * returns the file with the given name from the config paths
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param name the file name
	 * @return the resolved file
	 */
	PUBLIC_API GAMELIB_INTERFACE_METHOD(boost::filesystem::path getConfigFile(std::string name));
};

GAMELIB_NAMESPACE_END(core)

#endif //GAMELIB_CORE_OSPATHS_H