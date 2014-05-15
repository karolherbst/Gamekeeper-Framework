#include "gameolithprototype.h"

#include <gamekeeper/backend/jsongamelistparser.h>
#include <gamekeeper/client/autowire.h>
#include <gamekeeper/core/httpfiledownloader.h>
#include <gamekeeper/core/logger.h>
#include <gamekeeper/core/loggerStream.h>
#include <gamekeeper/model/game.h>

GAMECLIENTUI_CLASS(GameolithPrototype)

namespace backend = gamekeeper::backend;
using gamekeeper::model::Game;
using namespace gamekeeper::core;

static std::shared_ptr<gamekeeper::core::HttpFileDownloader> fileDownloader;

GameolithPrototype::GameolithPrototype(gamekeeper::core::Logger& _logger)
:	logger(_logger){}

void
GameolithPrototype::init(const ConfigMap &)
{
	fileDownloader = gamekeeper::client::Autowire<gamekeeper::core::HttpFileDownloader>();
}

void
GameolithPrototype::onShutdown()
{
	this->logger << LogLevel::Debug << "shutdown" << endl;
}

bool
GameolithPrototype::handleRequest(std::basic_istream<gkbyte_t> & is)
{
	static std::map<std::string, std::string> config
	{
		{"games.list", "$[*]"},
		{"game.id", "@.slug"},
		{"game.name", "@.title"}
	};

	std::unique_ptr<backend::GameListParser> glp(new backend::JSONGameListParser(config));
	for (const auto & g : glp->parseGameList(is))
	{
		this->logger << LogLevel::Debug << g->getName() << ' ' << g->getId() << endl;
	}
	return true;
}

void
GameolithPrototype::startEventLoop()
{
	namespace p = std::placeholders;
	this->logger << LogLevel::Debug << "starting gameolith test" << endl;
	fileDownloader->downloadFile("https://www.gameolith.com/user/karolherbst/games/?format=json",
	                             std::bind(&GameolithPrototype::handleRequest, this, p::_1));
}
