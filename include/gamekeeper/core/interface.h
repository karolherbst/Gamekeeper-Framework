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

/**
 * this method auto creates all needed methods for interfaces
 *
 * @author Karol Herbst
 * @since 0
 * @param type the name of the interface
 */
#define GAMEKEEPER_INTERFACE_METHODS(type) \
	PUBLIC_INLINE virtual ~type() = default; \
	PUBLIC_INLINE type() = default; \
	PRIVATE_API type(const type&) = delete; \
	PRIVATE_API type & operator=(const type &) = delete
