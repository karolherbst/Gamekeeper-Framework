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

#include <gamekeeper/core/boostpopropertyresolver.h>

#include <boost/program_options/variables_map.hpp>

GAMEKEEPER_NAMESPACE_START(core)

BoostPOPropertyResolver::BoostPOPropertyResolver(const boost::program_options::variables_map & map)
{
	for(auto p : map)
	{
		this->values.insert(this->values.end(), std::make_pair(p.first, p.second.value()));
	}
}

boost::any
BoostPOPropertyResolver::get(const std::string& key)
{
	auto it = this->values.find(key);
	if(it == this->values.end())
	{
		return boost::any();
	}
	return (*it).second;
}

GAMEKEEPER_NAMESPACE_END(core)
