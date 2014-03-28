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

#include "pch.h"

#include "log4cppLogger.h"
#include "log4cpploggerFactory.h"

#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>

GAMEKEEPER_NAMESPACE_START(core)

Log4cppLoggerFactory::Log4cppLoggerFactory()
:	rootCategory(log4cpp::Category::getRoot()),
	appender(new log4cpp::OstreamAppender("console", &std::cout))
{
	log4cpp::PatternLayout * layout = new log4cpp::PatternLayout();
	layout->setConversionPattern("%d{%Y-%m-%d %H:%M:%S} [%c] %p: %m%n");
	this->appender->setLayout(layout);

	rootCategory.setPriority(log4cpp::Priority::DEBUG);
	rootCategory.addAppender(this->appender);

	this->rootLogger = new Log4cppLogger(this->rootCategory);
}

Logger&
Log4cppLoggerFactory::getDefaultLogger()
{
	return getComponentLogger("Default");
}

Logger&
Log4cppLoggerFactory::getComponentLogger(const char * const id)
{
	if(this->loggers.find(id) == this->loggers.end())
	{
		Logger * newLogger = new Log4cppLogger(log4cpp::Category::getInstance(id));
		this->loggers.insert(std::make_pair(id, newLogger));
		return *newLogger;
	}
	return *this->loggers.at(id);
}

Log4cppLoggerFactory::~Log4cppLoggerFactory()
{
	if(this->appender != nullptr)
	{
		this->rootCategory.removeAppender(this->appender);
	}

	if(this->rootLogger != nullptr)
	{
		delete this->rootLogger;
	}

	for(auto entry : this->loggers)
	{
		delete entry.second;
	}
}

GAMEKEEPER_NAMESPACE_END(core)
