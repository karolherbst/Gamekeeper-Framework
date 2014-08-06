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

#include <boost/algorithm/string/join.hpp>

namespace balgo = boost::algorithm;

GAMEKEEPER_NAMESPACE_START(core)

static const std::string PME_MESSAGE{"following proeprties not found: "};

PropertiesMissingException::PropertiesMissingException(const std::vector<std::string> & m)
:	props(m),
	msg(PME_MESSAGE + balgo::join(props, ", ")){}

PropertiesMissingException::PropertiesMissingException(std::vector<std::string> && m)
:	props(m),
	msg(PME_MESSAGE + balgo::join(props, ", ")){}

const char *
PropertiesMissingException::what() const noexcept
{
	return this->msg.c_str();
}

const std::vector<std::string>
PropertiesMissingException::getMissingProperties() const
{
	return this->props;
}

GAMEKEEPER_NAMESPACE_END(core)
