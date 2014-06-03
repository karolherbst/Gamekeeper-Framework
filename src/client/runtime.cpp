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

#include <cstdlib>
#include <iostream>

#include <boost/filesystem/operations.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <Hypodermic/ContainerBuilder.h>
#include <Hypodermic/Helpers.h>

#include <gamekeeper/backend/storemanager.h>
#include <gamekeeper/client/gamekeeper.h>
#include <gamekeeper/client/hypodermic.h>
#include <gamekeeper/client/storecontrollerimpl.h>
#include <gamekeeper/core/logger.h>
#include <gamekeeper/core/loggerStream.h>
#include <gamekeeper/core/boostpopropertyresolver.h>
#include <gamekeeper/core/curlfiledownloaderfactory.h>
#include <gamekeeper/core/gnuinstalldirspaths.h>
#include <gamekeeper/core/log4cpploggerFactory.h>
#include <gamekeeper/core/portableinstalldirspaths.h>
#include <gamekeeper/core/stdc++11threadmanager.h>
#include <gamekeeper/core/xdgpaths.h>

// some platform dependent stuff
#ifdef GAMEKEEPER_OS_IS_WINDOWS
  #include <gamekeeper/core/win32threadhelper.h>
  #include <gamekeeper/core/windowsinformation.h>
  #define OSINFORMATIONCLASS WindowsInformation
  #define THREADHELPERCLASS Win32ThreadHelper
#else
  #include <gamekeeper/core/linuxinformation.h>
  #include <gamekeeper/core/pthreadhelper.h>
  #define OSINFORMATIONCLASS LinuxInformation
  #define THREADHELPERCLASS PthreadHelper
#endif

#ifdef WITH_LIBSECRET
  #include <gamekeeper/backend/libsecretmanager.h>
  #define AUTHMANAGERCLASS LibSecretManager
  #define HAS_AUTHMANAGER 1
#endif

namespace fs = boost::filesystem;
namespace po = boost::program_options;

GAMEKEEPER_NAMESPACE_START(client)

static std::shared_ptr<Hypodermic::IContainer> localContainer;
static std::shared_ptr<Hypodermic::IContainer> container;

GameKeeperRuntime::GameKeeperRuntime()
{
	using namespace gamekeeper::core;
	Hypodermic::ContainerBuilder containerBuilder;

	// set up pre IoC container
	containerBuilder.registerType<OSINFORMATIONCLASS>()->
		as<OSInformation>()->
		singleInstance();
	containerBuilder.registerType<XDGPaths>(CREATE(new XDGPaths(INJECT(OSInformation))))->
		as<UserPaths>()->
		singleInstance();
	localContainer = containerBuilder.build();
}

static void
fillProperties(po::options_description & cmd, po::options_description & file)
{
	po::options_description descGlobalCmd("Global options");
	po::options_description descNetwork("Network options");

	descGlobalCmd.add_options()
		("help,h", "produce help message");

	descNetwork.add_options()
		("filelayout.bundle", po::value<std::string>()->default_value("auto"))
		("network.debug", po::value<bool>()->default_value(false)->implicit_value(true))
		("network.time_between_retries", po::value<uint16_t>()->default_value(300))
		("network.user_agent", po::value<std::string>())
		("network.resolve.retries", po::value<uint16_t>()->default_value(3))
		("network.resolve.timeout", po::value<uint16_t>()->default_value(5000))
		("network.connection.retries", po::value<uint16_t>()->default_value(3))
		("network.connection.timeout", po::value<uint16_t>()->default_value(30000))
		("network.download.max_buffer_size", po::value<uint32_t>()->default_value(2048));

	cmd.add(descGlobalCmd);
	cmd.add(descNetwork);
	file.add(descNetwork);
}

int
GameKeeperRuntime::main(int argc, const char* argv[], NewInstanceFuncPtr instanceFuncPtr, AddOptionsFuncPtr optionsFuncPtr)
{
	using namespace gamekeeper::backend;
	using namespace gamekeeper::client;
	using namespace gamekeeper::core;

	po::options_description descCmd;
	po::options_description descFile;

	fillProperties(descCmd, descFile);

	po::options_description cmdClient("Client options");
	po::options_description fileClient;
	po::options_description bothClient;
	po::options_description_easy_init cmdClientEasy = cmdClient.add_options();
	po::options_description_easy_init fileClientEasy = fileClient.add_options();
	po::options_description_easy_init bothClientEasy = bothClient.add_options();

	if(optionsFuncPtr != nullptr)
	{
		optionsFuncPtr(cmdClientEasy, fileClientEasy, bothClientEasy);
	}

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

	fs::path configFile = localContainer->resolve<UserPaths>()->getConfigFile("properties.conf");
	if(fs::exists(configFile))
	{
		po::store(po::parse_config_file<char>(configFile.string().c_str(), descFile, true), vm);
	}

	if(vm.count("help") > 0)
	{
		std::cout << descCmd << std::endl;
		return EXIT_SUCCESS;
	}

	po::notify(vm);

	// build our property resolver now
	std::shared_ptr<PropertyResolver> pr = std::make_shared<BoostPOPropertyResolver>(vm);

	Hypodermic::ContainerBuilder containerBuilder;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
	// set up the real IoC container

	// reuse instances from the pre container
	containerBuilder.registerInstance<OSInformation>(
		localContainer->resolve<OSInformation>())->
		as<OSInformation>()->
		singleInstance();
	containerBuilder.registerInstance<UserPaths>(
		localContainer->resolve<UserPaths>())->
		as<UserPaths>()->
		singleInstance();

	std::string bundleLayout = pr->get<std::string>("filelayout.bundle");

	if(bundleLayout == "auto")
	{
#if defined(GAMEKEEPER_OS_IS_LINUX)
		bundleLayout = "FHS";
#elif defined(GAMEKEEPER_OS_IS_WINDOWS)
		bundleLayout = "portable";
#endif
	}
	if(bundleLayout == "FHS")
	{
		containerBuilder.registerType<GNUInstallDirsPaths>()->
			as<BundlePaths>()->
			singleInstance();
	}
	else if(bundleLayout == "portable")
	{
		containerBuilder.registerType<PortableInstallDirsPaths>(CREATE(new PortableInstallDirsPaths(INJECT(OSInformation))))->
			as<BundlePaths>()->
			singleInstance();
	}

	containerBuilder.registerType<Log4cppLoggerFactory>(CREATE(new Log4cppLoggerFactory(INJECT(UserPaths))))->
	        as<LoggerFactory>()->
	        singleInstance();
	containerBuilder.registerInstance(pr)->
		as<PropertyResolver>()->
		singleInstance();
	containerBuilder.registerType<CurlFileDownloaderFactory>(
		CREATE(new CurlFileDownloaderFactory(INJECT(LoggerFactory), INJECT(PropertyResolver), INJECT(UserPaths))))->
		as<FileDownloaderFactory>()->
		singleInstance();
	containerBuilder.registerType<THREADHELPERCLASS>()->
		as<NativeThreadHelper>()->
	        singleInstance();
	containerBuilder.registerType<StdCpp11ThreadManager>(
		CREATE(new StdCpp11ThreadManager(INJECT(NativeThreadHelper),
	                                     INJECT(LoggerFactory))))->
		as<ThreadManager>()->
		as<ThreadFactory>()->
		singleInstance();

#ifdef HAS_AUTHMANAGER
	containerBuilder.registerType<AUTHMANAGERCLASS>()->
		as<AuthManager>()->
		singleInstance();
#define INJECT_AUTH INJECT(AuthManager)
#else
#define INJECT_AUTH nullptr
#endif
	containerBuilder.registerType<StoreManager>(CREATE(new StoreManager(INJECT(LoggerFactory), INJECT(BundlePaths), INJECT(FileDownloaderFactory),
	                                                                    INJECT_AUTH)))->
		as<StoreManager>()->
		singleInstance();

	containerBuilder.registerType<StoreControllerImpl>(CREATE(new StoreControllerImpl(INJECT(StoreManager))))->
		as<StoreController>()->
		singleInstance();
#pragma GCC diagnostic pop
	container = containerBuilder.build();

	std::shared_ptr<gamekeeper::core::LoggerFactory> loggerFactory = container->resolve<gamekeeper::core::LoggerFactory>();
	loggerFactory->getComponentLogger("main") << gamekeeper::core::LogLevel::Debug << "firing up GameKeeper" << gamekeeper::core::endl;

	this->gameKeeperUI = instanceFuncPtr(loggerFactory->getComponentLogger("UI.client"));
	this->gameKeeperUI->init(vm);
	this->gameKeeperUI->startEventLoop();

	std::shared_ptr<gamekeeper::core::ThreadManager> threadManager = container->resolve<gamekeeper::core::ThreadManager>();
	threadManager->interruptAll();
	for(int8_t count = 0; count < 5 && !threadManager->tryJoinFor(2000); count++){}

	this->gameKeeperUI->onShutdown();
	delete this->gameKeeperUI;

	return EXIT_SUCCESS;
}

Hypodermic::IContainer&
HypodermicUtil::getContainer()
{
	return *container;
}

GAMEKEEPER_NAMESPACE_END(client)
