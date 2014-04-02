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
  #include <gamekeeper/core/windowsinformation.h>
  #define OSINFORMATIONCLASS WindowsInformation
#else
  #include <gamekeeper/core/linuxinformation.h>
  #define OSINFORMATIONCLASS LinuxInformation
#endif
#include <gamekeeper/core/propertyresolver.h>

#include <Hypodermic/ContainerBuilder.h>
#include <Hypodermic/Helpers.h>

GAMEKEEPER_NAMESPACE_START(test)

class TestPropertyResolver : public gamekeeper::core::PropertyResolver
{
public:
	TestPropertyResolver(std::map<std::string, boost::any> & _props);
	GAMEKEEPER_IMPLEMENTATION_OVERRIDE(boost::any get(const std::string& key));
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
		containerBuilder.registerType<Log4cppLoggerFactory>()->
		        as<LoggerFactory>()->
		        singleInstance();
		containerBuilder.registerType<OSINFORMATIONCLASS>()->
		        as<OSInformation>()->
		        singleInstance();
		containerBuilder.registerType<TestPropertyResolver>(CREATE_CAPTURED([this], new TestPropertyResolver(this->props)))->
			as<PropertyResolver>()->
			singleInstance();
	}
	
	// left out not implemented stuff yet
	this->container = containerBuilder.build();
}

void
DefaultFicture::setProperty(const std::string & key, boost::any value)
{
	this->props[key] = value;
}

GAMEKEEPER_NAMESPACE_END(test)
