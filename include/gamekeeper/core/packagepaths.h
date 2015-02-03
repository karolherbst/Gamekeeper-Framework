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

#include <gamekeeper/core/interface.h>

namespace boost
{
namespace filesystem
{
	class path;
}
}

GAMEKEEPER_NAMESPACE_START(core)

/**
 * @interface PackagePaths packagepaths.h <gamekeeper/core/packagepaths.h>
 *
 * this interface provides all methods to access files part of the gamekeeper installation.
 *
 * It isn't guarented that any paths or files are writeable
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API PackagePaths
{
	GAMEKEEPER_INTERFACE_METHODS(PackagePaths);

	/**
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @return the path to all installed data files
	 */
	virtual const boost::filesystem::path & getDataPath() = 0;
};

GAMEKEEPER_NAMESPACE_END(core)
