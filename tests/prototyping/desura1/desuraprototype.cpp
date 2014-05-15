#include "desuraprototype.h"

#include <map>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include <gamekeeper/backend/xmlgamelistparser.h>
#include <gamekeeper/client/autowire.h>
#include <gamekeeper/core/httpfiledownloader.h>
#include <gamekeeper/model/game.h>
#include <gamekeeper/utils/stringutils.h>

GAMECLIENTUI_CLASS(DesuraPrototype)

namespace backend = gamekeeper::backend;
namespace client = gamekeeper::client;
namespace core = gamekeeper::core;
namespace model = gamekeeper::model;
namespace po = boost::program_options;
namespace utils = gamekeeper::utils;

GAMECLIENT_ADD_OPTIONS({
	cmd("desura.accountname", po::value<std::string>()->required(), "Desura Account name");
	cmd("desura.password", po::value<std::string>()->required(), "Desura Account password");
})

static std::unordered_set<std::unique_ptr<model::Game>>
resolveAllGames(std::basic_istream<gkbyte_t> & is)
{
	std::map<std::string,std::unique_ptr<model::Game>> games;
	std::map<std::string, std::string> config
	{
		{"games.list", "/memberdata/games//game"},
		{"game.id", "nameid/text()"},
		{"game.name", "name/text()"}
	};
	std::unique_ptr<backend::GameListParser> glp(new backend::XMLGameListParser(config));
	return glp->parseGameList(is);
}

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
		std::unordered_set<std::unique_ptr<model::Game>> games = resolveAllGames(is);
		for(const auto & g : games)
		{
			std::cout << g->getName() << std::endl;
		}
		return true;
	}, cookies);

	hfd->downloadFileWithCookies("http://api.desura.com/1/memberlogout",
	[](std::basic_istream<gkbyte_t> & is) -> bool
	{
		return true;
	}, cookies);
}
