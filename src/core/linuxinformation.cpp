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

#include "pch.h"

#include "linuxinformation.h"

#include <cstdlib>

GAMELIB_NAMESPACE_START(core)

void
LinuxInformation::setEnv(const char * name, const char * value)
{
	setenv(name, value, 1);
}

std::string
LinuxInformation::getEnvSeperator()
{
	return ":";
}

boost::filesystem::path
LinuxInformation::getSystemRoot()
{
	return "/";
}

boost::filesystem::path
LinuxInformation::getUserPath()
{
	return getEnv("HOME");
}

GAMELIB_NAMESPACE_END(core)
