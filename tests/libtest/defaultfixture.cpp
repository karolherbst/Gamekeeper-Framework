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

#include "defaultfixture.h"

#include <gamekeeper/core/log4cpploggerFactory.h>

// some platform dependent stuff
#ifdef GAMEKEEPER_OS_IS_WINDOWS
  #include <gamekeeper/core/win32threadhelper.h>
  #define THREADHELPERCLASS Win32ThreadHelper
#else
  #include <gamekeeper/core/pthreadhelper.h>
  #define THREADHELPERCLASS PthreadHelper
#endif

#include <gamekeeper/core/userpaths.h>
#include <gamekeeper/core/propertyresolver.h>
#include <gamekeeper/core/stdc++11threadmanager.h>

#include <boost/filesystem/operations.hpp>

#include <Hypodermic/ContainerBuilder.h>
#include <Hypodermic/Helpers.h>

GAMEKEEPER_NAMESPACE_START(test)

class TestUserPaths : public gamekeeper::core::UserPaths
{
public:
	TestUserPaths();
	virtual boost::filesystem::path getConfigFile(std::string name) override;
	virtual boost::filesystem::path getDataFile(std::string name) override;
	virtual boost::filesystem::path getCacheFile(std::string name) override;
	virtual boost::filesystem::path getRuntimeFile(std::string name) override;
private:
	static boost::filesystem::path testPath;
};

boost::filesystem::path
TestUserPaths::testPath = TESTPATH;

TestUserPaths::TestUserPaths()
{
	if(boost::filesystem::exists(testPath))
	{
		boost::filesystem::remove_all(testPath);
	}
	boost::filesystem::create_directories(testPath);
}

boost::filesystem::path
TestUserPaths::getConfigFile(std::string name)
{
	return testPath / "config" / name;
}

boost::filesystem::path
TestUserPaths::getDataFile(std::string name)
{
	return testPath / "data" / name;
}

boost::filesystem::path
TestUserPaths::getCacheFile(std::string name)
{
	return testPath / "cache" / name;
}

boost::filesystem::path
TestUserPaths::getRuntimeFile(std::string name)
{
	return testPath / "runtime" / name;
}

class TestPropertyResolver : public gamekeeper::core::PropertyResolver
{
public:
	TestPropertyResolver(std::map<std::string, boost::any> & _props);
	virtual boost::any get(const std::string& key) override;
private:
	std::map<std::string, boost::any> & props;
};

TestPropertyResolver::TestPropertyResolver(std::map<std::string, boost::any> & _props)
:	props(_props){}

boost::any
TestPropertyResolver::get(const std::string& key)
{
	return this->props[key];
}

DefaultFicture::DefaultFicture() {
	Hypodermic::ContainerBuilder containerBuilder;
	{
		using namespace gamekeeper::core;

		// set up IoC container
		containerBuilder.registerType<TestUserPaths>()->
			as<UserPaths>()->
			singleInstance();
		containerBuilder.registerType<TestPropertyResolver>(CREATE_CAPTURED([this], new TestPropertyResolver(this->props)))->
			as<PropertyResolver>()->
			singleInstance();
		containerBuilder.registerType<THREADHELPERCLASS>()->
			as<NativeThreadHelper>()->
			singleInstance();
		containerBuilder.registerType<StdCpp11ThreadManager>(CREATE(new StdCpp11ThreadManager(INJECT(NativeThreadHelper))))->
			as<ThreadManager>()->
			as<ThreadFactory>()->
			singleInstance();
	}

	// left out not implemented stuff yet
	this->container = containerBuilder.build();

	// set default properties
	this->setProperty("network.debug", false);
	this->setProperty("network.connection.retries", (uint16_t)3);
	this->setProperty("network.connection.timeout", (uint16_t)30000);
	this->setProperty("network.download.max_buffer_size", (uint32_t)2048);
	this->setProperty("network.resolve.retries", (uint16_t)3);
	this->setProperty("network.resolve.timeout", (uint16_t)5000);
	this->setProperty("network.time_between_retries", (uint16_t)300);
}

DefaultFicture::~DefaultFicture()
{
	using namespace gamekeeper::core;
	auto tm = this->container->resolve<ThreadManager>();
	tm->interruptAll();
	for(int8_t count = 0; count < 10 && !tm->tryJoinFor(250); count++){}
}

void
DefaultFicture::setProperty(const std::string & key, boost::any value)
{
	this->props[key] = value;
}

GAMEKEEPER_NAMESPACE_END(test)
