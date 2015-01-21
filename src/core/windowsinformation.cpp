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

#include <gamekeeper/core/windowsinformation.h>

#include <cstdlib>

#include <windows.h>
#include <lmcons.h>
#include <shlobj.h>

#include <boost/locale/encoding_utf.hpp>

namespace bfs = boost::filesystem;

GAMEKEEPER_NAMESPACE_START(core)

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

bfs::path
WindowsInformation::getSystemRoot()
{
	return "C:\\";
}

bfs::path
WindowsInformation::getUserPath()
{
	TCHAR szPath[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, szPath);
	return szPath;
}

std::string
WindowsInformation::getUserName()
{
	WCHAR buffer[UNLEN + 1];
	DWORD size = UNLEN + 1;
	GetUserNameW(buffer, &size);
	return utf_to_utf<char>(buffer, &buffer[size]);
}

bfs::path
WindowsInformation::getExecPath()
{
	wchar_t buffer[MAX_PATH];
	DWORD resSize = GetModuleFileNameW(nullptr, buffer, MAX_PATH);
	return bfs::path(buffer, &buffer[resSize]);
}

GAMEKEEPER_NAMESPACE_END(core)
