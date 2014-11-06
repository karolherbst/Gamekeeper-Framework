#include "genericprototype.h"

#include <iostream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include <gamekeeper/client/autowire.h>
#include <gamekeeper/client/storecontroller.h>
#include <gamekeeper/core/logger.h>
#include <gamekeeper/core/loggerStream.h>

GAMECLIENTUI_CLASS(GenericPrototype)

namespace client = gamekeeper::client;
namespace core = gamekeeper::core;
namespace po = boost::program_options;
using core::endl;
using core::LogLevel;

GenericPrototype::GenericPrototype(gamekeeper::core::Logger& _logger)
:	logger(_logger){}

void
GenericPrototype::init(const ConfigMap & cm)
{
}

void
GenericPrototype::onShutdown()
{
}

static std::string
platformsToString(const std::set<gamekeeper::model::Platform> & platforms)
{
	using gamekeeper::model::Platform;
	std::string result;
	for(const auto & p : platforms)
	{
		switch(p)
		{
		case Platform::WIN_32:
			result += "win32 ";
			break;
		case Platform::MAC_32:
			result += "Mac32 ";
			break;
		case Platform::LIN_32:
			result += "lin32 ";
			break;
		case Platform::LIN_64:
			result += "lin64 ";
			break;
		}
	}
	return result;
}

static bool
whileLoop(std::string & storeName)
{
	std::cout << "enter name of game store" << std::endl;
	return std::getline(std::cin, storeName);
}

void
GenericPrototype::startEventLoop()
{
	client::Autowire<client::StoreController> sc;

	std::string storeName;

	while(whileLoop(storeName))
	{
		std::unique_ptr<client::Store> store;
		try
		{
			store = sc->get(storeName);
		}
		catch(const std::exception & e)
		{
			this->logger << LogLevel::Error << e.what() << endl;
			continue;
		}

		// we have only to login if the store isn't logged in
		if(!store->isLoggedIn())
		{
			std::vector<std::string> loginMethods = store->supportedLoginMethods();
			std::string loginMethod;
			if(loginMethods.size() == 1)
			{
				loginMethod = loginMethods[0];
				this->logger << LogLevel::Info << "using " << loginMethod << " login method"<< endl;
			}
			else
			{
				std::cout << "store supports the following login methods, please choose one of:";
				for(const auto & m : loginMethods)
				{
					std::cout << " " << m;
				}
				std::cout << std::endl;
				if(!std::getline(std::cin, loginMethod))
				{
					break;
				}
				if(std::find(loginMethods.begin(), loginMethods.end(), loginMethod) == loginMethods.end())
				{
					this->logger << LogLevel::Error << "unsupported login method" << endl;
					break;
				}
			}

			std::string username;
			std::string password;

			std::cout << "enter username" << std::endl;
			if(!std::getline(std::cin, username))
			{
				break;
			}

			std::cout << "enter password" << std::endl;
			if(!std::getline(std::cin, password))
			{
				break;
			}

			if(!store->login(loginMethod, username, password))
			{
				this->logger << LogLevel::Info << "login failed" << endl;
				continue;
			}
		}

		for(const auto & g : store->getAllGames())
		{
			this->logger << core::LogLevel::Debug << "id[" << g->getId() << "] name[" << g->getName() << "] platforms[" << platformsToString(g->getPlatforms())
				<< "] description[" << g->getDescription() << "] homepage[" << g->getHomepage() << "]" << core::endl;
		}
	}
}
