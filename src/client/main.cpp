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

#include "pch.h"

#include <gamelib/core/common.h>

#include <gamelib/core/logger.h>
#include <gamelib/core/loggerStream.h>
#include <gamelib/client/gamelib.h>

#include <Hypodermic/ContainerBuilder.h>
#include <Hypodermic/Helpers.h>

#include "../core/curlFileDownloader.h"
#include "../core/log4cpploggerFactory.h"

static gamelib::client::GameLibUI* gamelibI = nullptr;

/**
 * main entry point of gamelib
 *
 * these are the steps which are done here:
 *	1. read the configuration file
 *	2. overwrite properties from application arguments
 *	3. initialize/configure global objects
 *	4. create the GameLib implementation provided by the Language Frontend
 *	5. call gamelib.setConfig()
 *	6. call gamelib.init()
 *	7. call gamelib.startEventLoop()
 *	8. call gamelib.shutdown()
 *	9. destroy gamelib
 *
 * @author Karol Herbst
 * @since 0
 */
PUBLIC_API int main(int argc, const char* argv[])
{
	Hypodermic::ContainerBuilder containerBuilder;
	{
		using namespace gamelib::core;
		
		// set up IoC container
		containerBuilder.registerType<Log4cppLoggerFactory>()->
		        as<LoggerFactory>()->
		        singleInstance();
		containerBuilder.registerType<CurlFileDownloader>(CREATE(new CurlFileDownloader(INJECT(LoggerFactory))))->
		        as<FileDownloader>()->
		        as<HttpFileDownloader>()->
		        singleInstance();
	}
	
	// left out not implemented stuff yet
	std::shared_ptr<Hypodermic::IContainer> container = containerBuilder.build();
	std::shared_ptr<gamelib::core::LoggerFactory> loggerFactory = container->resolve<gamelib::core::LoggerFactory>();
	loggerFactory->getComponentLogger("main") << gamelib::core::LOG_LEVEL::DEBUG << "firing up gamelib" << gamelib::core::endl;
	gamelibI = gamelib::client::newInstance(loggerFactory->getComponentLogger("UI.client"));
	gamelibI->init(argc, argv, container.get());
	gamelibI->startEventLoop();
	gamelibI->onShutdown();
	delete gamelibI;
	gamelibI = nullptr;
}
