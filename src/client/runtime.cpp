/*
 * libgamelib
 *
 * Copyright (C) 2013 Karol Herbst <gamelib@karolherbst.de>
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

#include "runtime.h"

#include <gamelib/core/common.h>

#include <cstdlib>

#include <gamelib/core/logger.h>
#include <gamelib/core/loggerStream.h>
#include <gamelib/client/gamelib.h>
#include <gamelib/client/hypodermic.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <Hypodermic/ContainerBuilder.h>
#include <Hypodermic/Helpers.h>

#include <gamelib/core/curlfiledownloader.h>
#ifdef GAMELIB_OS_IS_WINDOWS
  #include <gamelib/core/windowsinformation.h>
  #define OSINFORMATIONCLASS WindowsInformation
#else
  #include <gamelib/core/linuxinformation.h>
  #define OSINFORMATIONCLASS LinuxInformation
#endif
#include <gamelib/core/log4cpploggerFactory.h>
#include <gamelib/core/xdgpaths.h>

namespace po = boost::program_options;

GAMELIB_NAMESPACE_START(client)

static std::shared_ptr<Hypodermic::IContainer> localContainer;
static std::shared_ptr<Hypodermic::IContainer> container;

GameLibRuntime::GameLibRuntime()
{
	using namespace gamelib::core;
	Hypodermic::ContainerBuilder containerBuilder;

	// set up IoC container
	containerBuilder.registerType<Log4cppLoggerFactory>()->
	        as<LoggerFactory>()->
	        singleInstance();
	localContainer = containerBuilder.build();
}

GameLibRuntime::~GameLibRuntime()
{
	delete this->gameLibUI;
}

gamelib::core::Logger&
GameLibRuntime::getUILogger()
{
	return localContainer->resolve<gamelib::core::LoggerFactory>()->getComponentLogger("UI.client");
}

int
GameLibRuntime::main(int argc, const char* argv[], GameLibUI * gameLibUI)
{
	this->gameLibUI = gameLibUI;

	po::options_description desc("Global options");
	desc.add_options()
		("help", "produce help message");
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if(vm.count("help") > 0)
	{
		std::cout << desc << std::endl;
		return EXIT_SUCCESS;
	}

	Hypodermic::ContainerBuilder containerBuilder;
	{
		using namespace gamelib::core;

		// set up IoC container
		containerBuilder.registerInstance<LoggerFactory>(
			localContainer->resolve<gamelib::core::LoggerFactory>())->
			as<LoggerFactory>()->
			singleInstance();
		containerBuilder.registerType<OSINFORMATIONCLASS>()->
		        as<OSInformation>()->
		        singleInstance();
		containerBuilder.registerType<XDGPaths>(CREATE(new XDGPaths(INJECT(OSInformation))))->
		        as<OSPaths>()->
		        singleInstance();
		containerBuilder.registerType<CurlFileDownloader>(CREATE(new CurlFileDownloader(INJECT(LoggerFactory))))->
		        as<FileDownloader>()->
		        as<HttpFileDownloader>()->
		        singleInstance();
	}
	container = containerBuilder.build();

	std::shared_ptr<gamelib::core::LoggerFactory> loggerFactory = container->resolve<gamelib::core::LoggerFactory>();
	loggerFactory->getComponentLogger("main") << gamelib::core::LogLevel::Debug << "firing up gamelib" << gamelib::core::endl;

	this->gameLibUI->init(argc, argv);
	this->gameLibUI->startEventLoop();
	this->gameLibUI->onShutdown();

	return EXIT_SUCCESS;
}

Hypodermic::IContainer&
HypodermicUtil::getContainer()
{
	return *container;
}

GAMELIB_NAMESPACE_END(client)
