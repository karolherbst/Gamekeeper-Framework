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

#ifndef GAMEKEEPER_CORE_EXCEPTION_H
#define GAMEKEEPER_CORE_EXCEPTION_H 1

#include <gamekeeper/core/common.h>

#include <exception>
#include <string>

GAMEKEEPER_NAMESPACE_START(core)

/**
 * Basic Exception type used in GameKeeper
 *
 * Every thrown exception should be wraped by this class (or any subclasses), so that unhandled exceptionc can be
 * easily catched globally
 *
 * @author Karol Herbst
 * @since 0
 */
class PUBLIC_API GameKeeperException : public std::exception{};

/**
 * simple message based Exception class
 *
 * @author Karol Herbst
 * @since 0
 */
class PUBLIC_API GameKeeperMessageException : public GameKeeperException
{
public:
	GameKeeperMessageException(const std::string& message);
	GameKeeperMessageException(std::string&& message) noexcept;
	virtual const char * what() const noexcept override;
	GameKeeperMessageException& operator=(const exception&);
private:
	std::string errorMessage;
};

#define GAMEKEEPER_EXCEPTION(name) \
name : public gamekeeper::core::GameKeeperMessageException \
{ \
public: \
	name(const std::string & message) : gamekeeper::core::GameKeeperMessageException(message){} \
	name(std::string&& message) noexcept : gamekeeper::core::GameKeeperMessageException(message){} \
}

#define GAMEKEEPER_EXCEPTION_MESSAGE_CONCAT(name, message) \
name : public gamekeeper::core::GameKeeperMessageException \
{ \
public: \
	name(const std::string & info) : gamekeeper::core::GameKeeperMessageException(std::string(message) + info){} \
}

// some global exceptions
class PUBLIC_API GAMEKEEPER_EXCEPTION_MESSAGE_CONCAT(ResourceNotFound, "The following resource cannot be found: ");

// some non trivial global exceptions
class PUBLIC_API PropertiesMissingException : public gamekeeper::core::GameKeeperException
{
public:
	PropertiesMissingException(const std::vector<std::string> &);
	PropertiesMissingException(std::vector<std::string> &&);
	virtual const char * what() const noexcept override;
	const std::vector<std::string> getMissingProperties() const;
private:
	const std::vector<std::string> props;
	const std::string msg;
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_CORE_EXCEPTION_H
