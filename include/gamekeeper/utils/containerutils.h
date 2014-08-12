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

#ifndef GAMEKEEPER_UTILS_CONTAINERUTILS_H
#define GAMEKEEPER_UTILS_CONTAINERUTILS_H 1

#include <gamekeeper/core/common.h>

#include <vector>

GAMEKEEPER_NAMESPACE_START(utils)

namespace Containers
{
	/**
	 * returns a list of keys not found in @a container
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param[in] container the container to be searched through
	 * @param[in] keys a list of keys to be searched
	 */
	template <class Container>
	std::vector<typename Container::key_type> checkMissing(const Container & container, const std::vector<typename Container::key_type> & keys);
}

template <class Container>
std::vector<typename Container::key_type>
Containers::checkMissing(const Container & container, const std::vector<typename Container::key_type> & keys)
{
	std::vector<typename Container::key_type> missing;
	for(const typename Container::key_type & key : keys)
	{
		if(container.find(key) == container.end())
		{
			missing.push_back(key);
		}
	}
	return missing;
}

GAMEKEEPER_NAMESPACE_END(utils)

#endif //GAMEKEEPER_UTILS_STRINGUTILS_H
