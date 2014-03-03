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

#include "pch.h"

#include "windowsinformation.h"

#include <cstdlib>
#include <windows.h>
#include <Shlobj.h>

#include <boost/locale/encoding_utf.hpp>

GAMELIB_NAMESPACE_START(core)

using boost::locale::conv::utf_to_utf;

std::string
WindowsInformation::getEnv(const char * name)
{
	wchar_t buffer[32767];
	DWORD size = GetEnvironmentVariableW(utf_to_utf<wchar_t>(name).c_str(), buffer, 32767);
	return utf_to_utf<char>(buffer, &buffer[size]);
}

void
WindowsInformation::setEnv(const char * name, const char * value)
{
	SetEnvironmentVariableW(utf_to_utf<wchar_t>(name).c_str(), utf_to_utf<wchar_t>(value).c_str());
}

std::string
WindowsInformation::getEnvSeperator()
{
	return ";";
}

boost::filesystem::path
WindowsInformation::getSystemRoot()
{
	return "C:\\";
}

boost::filesystem::path
WindowsInformation::getUserPath()
{
	TCHAR szPath[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, szPath);
	return szPath;
}

GAMELIB_NAMESPACE_END(core)
