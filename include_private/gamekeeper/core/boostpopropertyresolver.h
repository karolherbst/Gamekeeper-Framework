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

#ifndef GAMEKEEPER_CORE_BOOSTPOPROPERTYRESOLVER_H
#define GAMEKEEPER_CORE_BOOSTPOPROPERTYRESOLVER_H 1

#include <gamekeeper/core/common.h>

#include <map>
#include <string>

#include <gamekeeper/core/propertyresolver.h>

namespace boost
{
namespace program_options
{
	class variables_map;
}
}

GAMEKEEPER_NAMESPACE_START(core)

class PUBLIC_API BoostPOPropertyResolver : public PropertyResolver
{
public:
	PUBLIC_API BoostPOPropertyResolver(const boost::program_options::variables_map &);
	PRIVATE_API GAMEKEEPER_IMPLEMENTATION_OVERRIDE(boost::any get(const std::string& key));
private:
	std::map<std::string, boost::any> values;
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_CORE_BOOSTPOPROPERTYRESOLVER_H
