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

#include <gamekeeper/core/exception.h>

GAMEKEEPER_NAMESPACE_START(core)

GameKeeperMessageException::GameKeeperMessageException(const std::string& message)
:	errorMessage(message){}

GameKeeperMessageException::GameKeeperMessageException(std::string&& message) noexcept
:	errorMessage(message){}

const char *
GameKeeperMessageException::what() const noexcept
{
	return this->errorMessage.c_str();
}

GameKeeperMessageException&
GameKeeperMessageException::operator=(const std::exception& ex)
{
	this->errorMessage = ex.what();
	return *this;
}

GAMEKEEPER_NAMESPACE_END(core)
