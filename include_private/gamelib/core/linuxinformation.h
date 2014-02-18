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

#ifndef GAMELIB_CORE_LINUXINFORMATION_H
#define GAMELIB_CORE_LINUXINFORMATION_H 1

#include <gamelib/core/common.h>

#include "abstractinformation.h"

GAMELIB_NAMESPACE_START(core)

class PUBLIC_API LinuxInformation : public AbstractInformation
{
public:
	PRIVATE_API GAMELIB_IMPLEMENTATION_OVERRIDE(void setEnv(const char *, const char *));
	PRIVATE_API GAMELIB_IMPLEMENTATION_OVERRIDE(std::string getEnvSeperator());
	PRIVATE_API GAMELIB_IMPLEMENTATION_OVERRIDE(boost::filesystem::path getSystemRoot());
	PRIVATE_API GAMELIB_IMPLEMENTATION_OVERRIDE(boost::filesystem::path getUserPath());
};

GAMELIB_NAMESPACE_END(core)

#endif //GAMELIB_CORE_LINUXINFORMATION_H
