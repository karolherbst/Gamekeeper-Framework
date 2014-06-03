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

#ifndef GAMEKEEPER_CORE_BUNDLEPATHS_H
#define GAMEKEEPER_CORE_BUNDKEPATHS_H 1

#include <gamekeeper/core/common.h>

#include <gamekeeper/core/interface.h>

#include <boost/filesystem/path.hpp>

GAMEKEEPER_NAMESPACE_START(core)

/**
 * @interface BundlePaths bundlepaths.h <gamekeeper/core/bundlepaths.h>
 *
 * this interface provides all methods to get Bundle specific paths
 *
 * It is guaranteed, that root access is needed to modify or create files under resolved system wide paths.
 *
 * All methods of this interface, which returns a single path, can throw the following exceptions:
 *  * ResourceNotFound
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API BundlePaths
{
	GAMEKEEPER_INTERFACE_METHODS(BundlePaths);

	/**
	 * returns the path to the data path directory
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @return the resolved file
	 * @throws ResourceNotFound
	 */
	PUBLIC_API virtual const boost::filesystem::path & getDataPath() = 0;
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_CORE_BUNDLEPATHS_H
