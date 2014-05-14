#include "desuraprototype.h"

#include <map>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include <pugixml.hpp>

#include <gamekeeper/client/autowire.h>
#include <gamekeeper/core/httpfiledownloader.h>
#include <gamekeeper/model/game.h>
#include <gamekeeper/utils/stringutils.h>

GAMECLIENTUI_CLASS(DesuraPrototype)

namespace client = gamekeeper::client;
namespace core = gamekeeper::core;
namespace model = gamekeeper::model;
namespace po = boost::program_options;
namespace utils = gamekeeper::utils;

GAMECLIENT_ADD_OPTIONS({
	cmd("desura.accountname", po::value<std::string>()->required(), "Desura Account name");
	cmd("desura.password", po::value<std::string>()->required(), "Desura Account password");
})

class GameXML : public model::Game
{
	friend class XMLGameResolver;
private:
	std::string id;
	std::string name;
public:
	const char * getId() const
	{
		return this->id.c_str();
	}

	const char * getName() const
	{
		return this->name.c_str();
	}
};

class XMLGameResolver
{
public:
	std::map<std::string,model::Game*> resolveAllGames(std::basic_istream<gkbyte_t> & is)
	{
		pugi::xml_document doc;
		std::map<std::string,model::Game*> games;

		if(doc.load(is))
		{
			std::cout << "loading XML finished" << std::endl;
			pugi::xpath_query gameQuery("/memberdata/games//game");
			pugi::xpath_query idQuery("nameid/text()");
			pugi::xpath_query nameQuery("name/text()");

			pugi::xpath_node_set result = gameQuery.evaluate_node_set(doc.document_element());
			for(size_t i = 0; i < result.size(); i++)
			{
				GameXML * game = new GameXML();
				game->id = idQuery.evaluate_string(result[i]);
				game->name = nameQuery.evaluate_string(result[i]);
				games[game->getId()] = game;
			}
		}
		else
		{
			std::cout << "loading XML failed" << std::endl;
		}

		return games;
	}
};

DesuraPrototype::DesuraPrototype(gamekeeper::core::Logger& _logger)
:	logger(_logger){}

void
DesuraPrototype::init(const ConfigMap & cm)
{
	this->username = cm.at("desura.accountname").as<std::string>().c_str();
	this->userpass = cm.at("desura.password").as<std::string>().c_str();
}

void
DesuraPrototype::onShutdown()
{
}

void
DesuraPrototype::startEventLoop()
{
	std::shared_ptr<core::HttpFileDownloader> hfd = client::Autowire<core::HttpFileDownloader>();
	core::HttpFileDownloader::Form form;
	form["username"] = this->username;
	form["password"] = this->userpass;
	core::HttpFileDownloader::CookieBuket cookies = hfd->doPostRequestForCookies("https://secure.desura.com/3/memberlogin", form);
	std::cout << "freeman: " << cookies["freeman"] << std::endl;
	std::cout << "masterchief: " << cookies["masterchief"] << std::endl;

	hfd->downloadFileWithCookies("http://api.desura.com/1/memberdata",
	[](std::basic_istream<gkbyte_t> & is) -> bool
	{
		XMLGameResolver resolver;
		std::map<std::string,model::Game*> games = resolver.resolveAllGames(is);
		for(auto it : games)
		{
			std::cout << it.second->getName() << std::endl;
		}
		return true;
	}, cookies);

	hfd->downloadFileWithCookies("http://api.desura.com/1/memberlogout",
	[](std::basic_istream<gkbyte_t> & is) -> bool
	{
		return true;
	}, cookies);
}
