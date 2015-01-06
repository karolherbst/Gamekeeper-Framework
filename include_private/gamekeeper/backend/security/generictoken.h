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

#ifndef GAMEKEEPER_BACKEND_SECURITY_GENERICTOKEN_H
#define GAMEKEEPER_BACKEND_SECURITY_GENERICTOKEN_H 1

#include <gamekeeper/core/common.h>

#include <gamekeeper/backend/security/token.h>

GAMEKEEPER_NAMESPACE_START(backend, security)

class PRIVATE_API GenericToken : public Token
{
public:
	GenericToken(const std::string & key, const std::string & value, const std::string & group, const TimePoint & expiry = TimePoint(), Properties properties = Properties());
	GenericToken(const std::string & key, const std::string & value, const std::string & group, const TimePoint::duration  & duration, Properties properties = Properties());
	GenericToken(const std::string & key, const std::string & value, const std::string & group, const TimePoint::rep & duration, Properties properties = Properties());

	virtual const std::string & getKey() const override;
	virtual const std::string & getValue() const override;
	virtual const std::string & getGroup() const override;
	virtual const TimePoint & getExpiry() const override;
	virtual const Properties & getProperties() const override;
private:
	const std::string key;
	const std::string value;
	const std::string group;
	const TimePoint expiry;
	const Properties properties;
};

GAMEKEEPER_NAMESPACE_END(backend, security)

#endif //GAMEKEEPER_BACKEND_SECURITY_GENERICTOKEN_H
