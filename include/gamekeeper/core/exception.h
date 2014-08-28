/*
 * GameKeeper Framework
 *
 * Copyright (C) 2013 - 2014 Karol Herbst <gamekeeper@karolherbst.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Linking against the GameKeeper Framework statically or dynamically with
 * other modules is making a combined work based on the GameKeeper Framework.
 * Thus, the terms and conditions of the GNU General Public License cover the
 * whole combination.
 *
 * In addition, as a special exception, the copyright holders of the GameKeeper
 * Framework give you permission to copy and distribute a work based on the
 * GameKeeper Framework in object code or executable form without the
 * requirements to accompany it with the source code, provided that you meet
 * all of these conditions:
 *
 *   a) The dervied work links with the GameKeeper Framework solely through
 *      symbols defined in any file in the /include directory and its
 *      subdirectories.
 *
 *   b) The dervied work does not link against symbols of the GameKeeper
 *      Framework outside the gamekeeper::client, gamekeeper::core and
 *      gamekeeper::model C++ namespaces.
 *
 * You may copy and distribute such a system
 * following the terms of the GNU GPL for the GameKeeper Framework and the licenses
 * of the other code concerned, provided that you include the source code of that other
 * code when and as the GNU GPL requires distribution of source code.
 *
 * Note that people who make modified versions of the GameKeeper Framework are
 * not obligated to grant this special exception for their modified versions;
 * it is their choice whether to do so.  The GNU General Public License gives
 * permission to release a modified version without this exception; this
 * exception also makes it possible to release a modified version which
 * carries forward this exception.
 */

#ifndef GAMEKEEPER_CORE_EXCEPTION_H
#define GAMEKEEPER_CORE_EXCEPTION_H 1

#include <gamekeeper/core/common.h>

#include <exception>
#include <string>
#include <vector>

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
