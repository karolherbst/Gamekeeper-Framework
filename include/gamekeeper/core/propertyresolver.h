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

#ifndef GAMEKEEPER_CORE_PROPERTYRESOLVER_H
#define GAMEKEEPER_CORE_PROPERTYRESOLVER_H 1

#include <gamekeeper/core/common.h>

#include <gamekeeper/core/interface.h>

#include <boost/any.hpp>

GAMEKEEPER_NAMESPACE_START(core)

/**
 * @interface PropertyResolver <gamekeeper/core/propertyresolver.h>
 *
 * this interface provides methods to resolve properties from a global source
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API PropertyResolver
{
	PUBLIC_INLINE GAMEKEEPER_INTERFACE_DESTRUCTOR(PropertyResolver)

	/**
	 * returns the typed property of the given key
	 *
	 * the type of the property is specified in the documentory
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param key the key of the property
	 * @return a boost::any object with the typed value
	 */
	PUBLIC_API GAMEKEEPER_INTERFACE_METHOD(boost::any get(const std::string& key));

	/**
	 * templated wrapper method around the get method
	 *
	 * casts the return boost::any object of get to the given type
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param key the key of the property
	 * @return the casted value of the property of type T
	 */
	template <typename T>
	PUBLIC_INLINE T get(const std::string& key)
	{
		return boost::any_cast<T>(this->get(key));
	}
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_CORE_PROPERTYRESOLVER_H
