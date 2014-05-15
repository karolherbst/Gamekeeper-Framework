#include "desuraprototype.h"

#include <map>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <gamekeeper/backend/xmlgamelistparser.h>
#include <gamekeeper/client/autowire.h>
#include <gamekeeper/core/httpfiledownloader.h>
#include <gamekeeper/core/logger.h>
#include <gamekeeper/core/loggerStream.h>
#include <gamekeeper/model/game.h>
#include <gamekeeper/utils/stringutils.h>

GAMECLIENTUI_CLASS(DesuraPrototype)

namespace backend = gamekeeper::backend;
namespace client = gamekeeper::client;
namespace core = gamekeeper::core;
namespace model = gamekeeper::model;
namespace po = boost::program_options;
namespace prop = boost::property_tree;
namespace utils = gamekeeper::utils;

GAMECLIENT_ADD_OPTIONS({
	cmd("desura.accountname", po::value<std::string>()->required(), "Desura Account name");
	cmd("desura.password", po::value<std::string>()->required(), "Desura Account password");
})

static std::unordered_set<std::unique_ptr<model::Game>>
resolveAllGames(std::basic_istream<gkbyte_t> & is, const prop::ptree & config)
{
	std::map<std::string,std::unique_ptr<model::Game>> games;
	std::map<std::string, std::string> xmlConfig
	{
		{"games.list", config.get<std::string>("games.list")},
		{"game.id", config.get<std::string>("game.id")},
		{"game.name", config.get<std::string>("game.name")}
	};
	std::unique_ptr<backend::GameListParser> glp(new backend::XMLGameListParser(xmlConfig));
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
	prop::ptree config;
	prop::read_ini(CONFIG_FILE_PATH, config);

	std::shared_ptr<core::HttpFileDownloader> hfd = client::Autowire<core::HttpFileDownloader>();
	core::HttpFileDownloader::Form form;
	form[config.get<std::string>("authfield.username")] = this->username;
	form[config.get<std::string>("authfield.password")] = this->userpass;
	core::HttpFileDownloader::CookieBuket cookies = hfd->doPostRequestForCookies(config.get<std::string>("auth.loginurl").c_str(), form);

	hfd->downloadFileWithCookies(config.get<std::string>("games.url").c_str(),
	[this, &config](std::basic_istream<gkbyte_t> & is) -> bool
	{
		std::unordered_set<std::unique_ptr<model::Game>> games = resolveAllGames(is, config);
		for(const auto & g : games)
		{
			this->logger << core::LogLevel::Debug << g->getName() << core::endl;
		}
		return true;
	}, cookies);

	hfd->downloadFileWithCookies(config.get<std::string>("auth.logouturl").c_str(),
	[](std::basic_istream<gkbyte_t> & is) -> bool
	{
		return true;
	}, cookies);
}
