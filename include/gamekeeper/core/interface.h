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

#ifndef GAMEKEEPER_CORE_INTERFACE_H
#define GAMEKEEPER_CORE_INTERFACE_H 1

#include <gamekeeper/core/common.h>

/**
 * this method auto creates all needed methods for interfaces
 *
 * @author Karol Herbst
 * @since 0
 * @param type the name of the interface
 */
#define GAMEKEEPER_INTERFACE_METHODS(type) \
	PUBLIC_INLINE virtual ~type(){} \
	PUBLIC_INLINE type() = default; \
	PRIVATE_API type(const type&) = delete; \
	PRIVATE_API type & operator=(const type &) = delete

/**
 * this method auto creates the pure virtual method given
 *
 * @author Karol Herbst
 * @since 0
 * @param method the method signature
 */
#define GAMEKEEPER_INTERFACE_METHOD(method) virtual method = 0

/**
 * this macro shall be used to override interface methods
 *
 * @author Karol Herbst
 * @since 0
 * @param method the method signature
 */
#define GAMEKEEPER_IMPLEMENTATION_OVERRIDE(method) virtual method override

#endif //GAMEKEEPER_CORE_INTERFACE_H
