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

#ifndef GAMEKEEPER_BACKEND_PLATFORM_H
#define GAMEKEEPER_BACKEND_PLATFORM_H 1

#include <gamekeeper/core/common.h>

#include <functional>

GAMEKEEPER_NAMESPACE_START(model)

enum class Platform : uint16_t
{
	WIN32 = 0,
	WIN64 = 1,
	MAC32 = 2,
	MAC64 = 3,
	LIN32 = 4,
	LIN64 = 5,
};

GAMEKEEPER_NAMESPACE_END(model)

#endif //GAMEKEEPER_BACKEND_PLATFORM_H
