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

#include <gamekeeper/core/log4cppLogger.h>
#include <gamekeeper/core/log4cpploggerFactory.h>
#include <gamekeeper/core/userpaths.h>

#include <string>
#include <vector>

#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>

#include <log4cpp/Category.hh>
#include <log4cpp/Configurator.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PropertyConfigurator.hh>

namespace balgo = boost::algorithm;
namespace bfs = boost::filesystem;

GAMEKEEPER_NAMESPACE_START(core)

static std::string
parseLine(std::string line, std::shared_ptr<UserPaths> & userpaths)
{
	balgo::erase_all(line, " ");
	std::string::size_type pos = line.find(".fileName=");

	if(pos != std::string::npos)
	{
		std::string filename = line.substr(pos + 10);
		bfs::path logfile = userpaths->getDataFile(std::string("log/") + filename);
		bfs::create_directories(logfile.parent_path());
		line.replace(pos + 10, std::string::npos, logfile.string());
	}

	if(line.find_first_of("rootCategory") != 0)
	{
		balgo::replace_all(line, "category.", "category.GameKeeper.");
	}
	return line;
}

class Log4cppLoggerFactory::PImpl
{
public:
	PImpl(std::shared_ptr<UserPaths>);
	~PImpl();

	Logger& getComponentLogger(const char * const);

	Logger * rootLogger = nullptr;
	log4cpp::Appender * appender;
	log4cpp::Category & rootCategory;
	std::unordered_map<const char *, Logger *> loggers;
};

Log4cppLoggerFactory::PImpl::PImpl(std::shared_ptr<UserPaths> userpaths)
:	appender(new log4cpp::OstreamAppender("console", &std::cout)),
	rootCategory(log4cpp::Category::getInstance("GameKeeper"))
{
	// add default appender
	log4cpp::PatternLayout * layout = new log4cpp::PatternLayout();
	layout->setConversionPattern("%d{%Y-%m-%d %H:%M:%S} [%c] %p: %m%n");
	this->appender->setLayout(layout);

	// start with DEBUG level until the file is loaded, so that we get all errors
	this->rootCategory.setPriority(log4cpp::Priority::DEBUG);
	this->rootCategory.addAppender(this->appender);

	this->rootLogger = new Log4cppLogger(this->rootCategory);

	// read configuration file if exists
	bfs::path cFile = userpaths->getConfigFile("log.conf");
	if(!bfs::exists(cFile))
	{
		return;
	}

	Logger& logger = this->getComponentLogger("logger");
	logger << LogLevel::Debug << "parse config file at: " << cFile.string() << endl;

	bfs::ifstream ifstream(cFile);
	bfs::path cFileGen = userpaths->getConfigFile("log.conf.gen");
	bfs::remove(cFileGen);
	bfs::ofstream ofstream(cFileGen);
	std::string line;
	std::string parsedLine;

	while(!ifstream.eof())
	{
		std::getline(ifstream, line);
		// ignore empty lines
		if(line.empty())
		{
			continue;
		}

		parsedLine = parseLine(line, userpaths);
		logger << LogLevel::Debug << "parsed line \"" << line << "\" to: \"" << parsedLine << "\"" << endl;
		ofstream << parsedLine << std::endl;
	}

	// ignore the file if nothing was parsed
	if(ofstream.tellp() <= 0)
	{
		return;
	}

	logger << LogLevel::Info << "moving to new configuration" << endl;

	log4cpp::PropertyConfigurator pci;
	pci.configure(cFileGen.string());
	bfs::remove(cFileGen);

	// we have another logger noew
	this->rootCategory.removeAppender(this->appender);
	this->appender = nullptr;
	delete this->rootLogger;
	this->rootLogger = new Log4cppLogger(this->rootCategory);
}

Logger&
Log4cppLoggerFactory::getDefaultLogger()
{
	return getComponentLogger("GameKeeper.Default");
}

Logger&
Log4cppLoggerFactory::PImpl::getComponentLogger(const char * const id)
{
	if(this->loggers.find(id) == this->loggers.end())
	{
		std::string log4cppname = "GameKeeper.";
		log4cppname += id;
		Logger * newLogger = new Log4cppLogger(log4cpp::Category::getInstance(log4cppname));
		this->loggers.insert(std::make_pair(id, newLogger));
		return *newLogger;
	}
	return *this->loggers.at(id);
}

Log4cppLoggerFactory::PImpl::~PImpl()
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

Log4cppLoggerFactory::Log4cppLoggerFactory(std::shared_ptr<UserPaths> userpaths)
:	data(new Log4cppLoggerFactory::PImpl(userpaths)){}

Logger&
Log4cppLoggerFactory::getComponentLogger(const char * const id)
{
	return this->data->getComponentLogger(id);
}

Log4cppLoggerFactory::~Log4cppLoggerFactory(){}

GAMEKEEPER_NAMESPACE_END(core)
