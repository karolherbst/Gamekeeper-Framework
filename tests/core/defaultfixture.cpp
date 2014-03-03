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

// some platform dependent stuff
#ifdef GAMEKEEPER_OS_IS_WINDOWS
  #include <gamekeeper/core/windowsinformation.h>
  #define OSINFORMATIONCLASS WindowsInformation
#else
  #include <gamekeeper/core/linuxinformation.h>
  #define OSINFORMATIONCLASS LinuxInformation
#endif

#include <Hypodermic/ContainerBuilder.h>
#include <Hypodermic/Helpers.h>

GAMEKEEPER_NAMESPACE_START(test)

DefaultFicture::DefaultFicture()
{
	Hypodermic::ContainerBuilder containerBuilder;
	{
		using namespace gamekeeper::core;
		
		// set up IoC container
		containerBuilder.registerType<OSINFORMATIONCLASS>()->
		        as<OSInformation>()->
		        singleInstance();
	}
	
	// left out not implemented stuff yet
	this->container = containerBuilder.build();
}

GAMEKEEPER_NAMESPACE_END(test)
