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

#include "log4cpploggerFactory.h"

#include "log4cppLogger.h"

#include <log4cpp/Appender.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>

GAMELIB_NAMESPACE_START(core)

Log4cppLoggerFactory::Log4cppLoggerFactory()
{
	log4cpp::Appender *appender = new log4cpp::OstreamAppender("console", &std::cout);
	log4cpp::PatternLayout * layout = new log4cpp::PatternLayout();
	layout->setConversionPattern("%d{%Y-%m-%d %H:%M:%S} [%c] %p: %m%n");
	appender->setLayout(layout);

	log4cpp::Category& rootCategory = log4cpp::Category::getInstance("GameLib Root");
	rootCategory.setPriority(log4cpp::Priority::DEBUG);
	rootCategory.addAppender(appender);

	log4cpp::Category& uiCategory = log4cpp::Category::getInstance("UI");
	uiCategory.setPriority(log4cpp::Priority::DEBUG);
	uiCategory.addAppender(appender);

	this->rootLogger = new Log4cppLogger(rootCategory);
	this->uiLogger = new Log4cppLogger(uiCategory);
}

Logger&
Log4cppLoggerFactory::getDefaultLogger()
{
	return *this->rootLogger;
}

Logger&
Log4cppLoggerFactory::getUILogger()
{
	return *this->uiLogger;
}

Log4cppLoggerFactory::~Log4cppLoggerFactory()
{
	if (this->rootLogger != nullptr)
	{
		delete this->rootLogger;
	}

	if (this->uiLogger != nullptr)
	{
		delete this->uiLogger;
	}
}

GAMELIB_NAMESPACE_END(core)
