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

#include <memory>
#include <vector>

#include <gamekeeper/core/interface.h>

GAMEKEEPER_NAMESPACE_START(backend, security)

struct Token;

interface PUBLIC_API AuthManager
{
	typedef std::vector<std::unique_ptr<Token>> Tokens;

	GAMEKEEPER_INTERFACE_METHODS(AuthManager);
	PUBLIC_API virtual void saveToken(const Token & token) = 0;
	PUBLIC_API virtual void removeToken(const Token & token) = 0;
	PUBLIC_API virtual Tokens readAllTokens(const std::string & group) = 0;
};

GAMEKEEPER_NAMESPACE_END(backend, security)
