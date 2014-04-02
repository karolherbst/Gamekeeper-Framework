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

#ifndef TEST_GAMEKEEPER_CORE_WEBSERVERFICTURE
#define TEST_GAMEKEEPER_CORE_WEBSERVERFICTURE 1

#include <gamekeeper/core/common.h>

#include "defaultfixture.h"

#define WEBSERVERFICTUREMETHODSDEF \
	static void SetUpTestCase(); \
	static void TearDownTestCase();
#define WEBSERVERFICTUREMETHODSIMPL(clazz) \
void clazz::SetUpTestCase() { gamekeeper::test::WebServerFicture::SetUpTestCase();} \
void clazz::TearDownTestCase() { gamekeeper::test::WebServerFicture::TearDownTestCase();} \

struct mg_server;

GAMEKEEPER_NAMESPACE_START(test)

class WebServerFicture : public DefaultFicture
{
public:
	static void SetUpTestCase();
	static void TearDownTestCase();
private:
	static mg_server * server;
};

GAMEKEEPER_NAMESPACE_END(test)

#endif //TEST_GAMEKEEPER_CORE_WEBSERVERFICTURE