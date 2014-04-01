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

#include "runtime.h"

#include <gamekeeper/core/common.h>

#include <cstdlib>

#include <gamekeeper/core/logger.h>
#include <gamekeeper/core/loggerStream.h>
#include <gamekeeper/client/gamekeeper.h>
#include <gamekeeper/client/hypodermic.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <Hypodermic/ContainerBuilder.h>
#include <Hypodermic/Helpers.h>

#include <gamekeeper/core/curlfiledownloader.h>
#ifdef GAMEKEEPER_OS_IS_WINDOWS
  #include <gamekeeper/core/windowsinformation.h>
  #define OSINFORMATIONCLASS WindowsInformation
#else
  #include <gamekeeper/core/linuxinformation.h>
  #define OSINFORMATIONCLASS LinuxInformation
#endif
#include <gamekeeper/core/log4cpploggerFactory.h>
#include <gamekeeper/core/xdgpaths.h>

namespace po = boost::program_options;

GAMEKEEPER_NAMESPACE_START(client)

static std::shared_ptr<Hypodermic::IContainer> localContainer;
static std::shared_ptr<Hypodermic::IContainer> container;

GameKeeperRuntime::GameKeeperRuntime()
{
	using namespace gamekeeper::core;
	Hypodermic::ContainerBuilder containerBuilder;

	// set up IoC container
	containerBuilder.registerType<Log4cppLoggerFactory>()->
	        as<LoggerFactory>()->
	        singleInstance();
	localContainer = containerBuilder.build();
}

GameKeeperRuntime::~GameKeeperRuntime()
{
	delete this->gameKeeperUI;
}

gamekeeper::core::Logger&
GameKeeperRuntime::getUILogger()
{
	return localContainer->resolve<gamekeeper::core::LoggerFactory>()->getComponentLogger("UI.client");
}

static void
fillProperties(po::options_description & cmd, po::options_description & file)
{
	po::options_description descGlobalCmd("Global options");

	descGlobalCmd.add_options()
		("help", "produce help message");

	cmd.add(descGlobalCmd);
}

int
GameKeeperRuntime::main(int argc, const char* argv[], GameKeeperUI * gameKeeperUI)
{
	this->gameKeeperUI = gameKeeperUI;

	po::options_description descCmd;
	po::options_description descFile;

	fillProperties(descCmd, descFile);

	po::options_description cmdClient("Client options");
	po::options_description fileClient;
	po::options_description bothClient;
	po::options_description_easy_init cmdClientEasy = cmdClient.add_options();
	po::options_description_easy_init fileClientEasy = fileClient.add_options();
	po::options_description_easy_init bothClientEasy = bothClient.add_options();

	this->gameKeeperUI->addOptions(cmdClientEasy, fileClientEasy, bothClientEasy);

	if(!bothClient.options().empty())
	{
		cmdClient.add(bothClient);
		fileClient.add(bothClient);
	}

	if(!cmdClient.options().empty())
	{
		descCmd.add(cmdClient);
	}

	if(!fileClient.options().empty())
	{
		descFile.add(fileClient);
	}

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, descCmd), vm);
	//po::store(po::parse_config_file("", descFile), vm);

	if(vm.count("help") > 0)
	{
		std::cout << descCmd << std::endl;
		return EXIT_SUCCESS;
	}

	po::notify(vm);

	Hypodermic::ContainerBuilder containerBuilder;
	{
		using namespace gamekeeper::core;

		// set up IoC container
		containerBuilder.registerInstance<LoggerFactory>(
			localContainer->resolve<gamekeeper::core::LoggerFactory>())->
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

	std::shared_ptr<gamekeeper::core::LoggerFactory> loggerFactory = container->resolve<gamekeeper::core::LoggerFactory>();
	loggerFactory->getComponentLogger("main") << gamekeeper::core::LogLevel::Debug << "firing up GameKeeper" << gamekeeper::core::endl;

	this->gameKeeperUI->init(vm);
	this->gameKeeperUI->startEventLoop();
	this->gameKeeperUI->onShutdown();

	return EXIT_SUCCESS;
}

Hypodermic::IContainer&
HypodermicUtil::getContainer()
{
	return *container;
}

GAMEKEEPER_NAMESPACE_END(client)
