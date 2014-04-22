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

#ifndef TEST_GAMEKEEPER_CORE_DEFAULTFICTURE
#define TEST_GAMEKEEPER_CORE_DEFAULTFICTURE 1

#include <gamekeeper/core/common.h>

#include <boost/any.hpp>

#include <gtest/gtest.h>

#include <Hypodermic/IContainer.h>

GAMEKEEPER_NAMESPACE_START(test)

class DefaultFicture : public testing::Test
{
public:
	DefaultFicture();
	~DefaultFicture();
protected:
	std::shared_ptr<Hypodermic::IContainer> container;

	void setProperty(const std::string & key, boost::any value);
private:
	std::map<std::string, boost::any> props;
};

GAMEKEEPER_NAMESPACE_END(test)

#endif //TEST_GAMEKEEPER_CORE_DEFAULTFICTURE
