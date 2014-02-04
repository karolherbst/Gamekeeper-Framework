#include "gameolithprototype.h"

#include <gamelib/client/autowire.h>
#include <gamelib/core/httpfiledownloader.h>
#include <gamelib/core/logger.h>
#include <gamelib/core/loggerStream.h>
#include <gamelib/model/game.h>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <json/reader.h>
#include <json/value.h>

GAMECLIENTUI_CLASS(GameolithPrototype)

using gamelib::model::Game;
using namespace gamelib::core;

static std::shared_ptr<gamelib::core::HttpFileDownloader> fileDownloader;

// simulate the config file here
static std::map<std::string, std::string> config;

static std::string convertJsonPathToJsonCppPath(std::string path)
{
	// we handle root as current
	boost::replace_all(path, "$", "@");
	boost::replace_all(path, "@.", ".");
	
	// we don't have any @. anymore
	boost::replace_all(path, "@", ".");
	
	// if we have [*] at the end, it indicates an array only, but we don't use it
	if(boost::ends_with(path, "[*]"))
	{
		boost::replace_all(path, "[*]", "");
	}
	return path;
}

static Json::Value getRootForGames(std::string path, Json::Value node)
{
	Json::Path jsoncppPath(convertJsonPathToJsonCppPath(path));
	return jsoncppPath.resolve(node);
}

class GameJSON : public gamelib::model::Game
{
	friend class JSONGameResolver;
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

class JSONGameResolver
{
private:
	Json::Path idPath;
	Json::Path namePath;
public:
	JSONGameResolver()
	:	idPath(convertJsonPathToJsonCppPath(config["bindings.game.id"])),
		namePath(convertJsonPathToJsonCppPath(config["bindings.game.name"])){}

	Game * createGame(const Json::Value & gameNode)
	{
		GameJSON * game = new GameJSON();
		game->id = this->idPath.resolve(gameNode).asString();
		game->name = this->namePath.resolve(gameNode).asString();
		return game;
	}
};

GameolithPrototype::GameolithPrototype(gamelib::core::Logger& _logger)
:	logger(_logger){}

void
GameolithPrototype::init(int argc, const char* argv[])
{
	config["bindings.games"] = "$[*]";
	config["bindings.game.id"] = "@.slug";
	config["bindings.game.name"] = "@.title";
	fileDownloader = gamelib::client::Autowire<gamelib::core::HttpFileDownloader>();
}

void
GameolithPrototype::onShutdown()
{
	this->logger << LogLevel::Debug << "shutdown" << endl;
}

bool
GameolithPrototype::handleRequest(void * const buffer, size_t sz, size_t n)
{
	Json::Value root;
	Json::Reader reader;
	std::string jsonTree(static_cast<const char*>(buffer), n);
	if (reader.parse(jsonTree, root, false))
	{
		Json::Value gameRoot = getRootForGames(config["bindings.games"], root);
		JSONGameResolver gr;
		std::map<const std::string, const Game *> games;
		for (int i = 0; i < gameRoot.size(); i++)
		{
			const Game * game = gr.createGame(gameRoot[i]);
			games[game->getId()] = game;
		}
		for (auto pair : games)
		{
			const Game * game = pair.second;
			this->logger << LogLevel::Debug << game->getName() << ' ' << game->getId() << endl;
		}
		return true;
	}
	return false;
}

void
GameolithPrototype::startEventLoop()
{
	this->logger << LogLevel::Debug << "starting gameolith test" << endl;
	fileDownloader->downloadFile("https://www.gameolith.com/user/karolherbst/games/?format=json",
	                             [this](void * const buffer, size_t bufferSize, size_t dataLength) -> bool
	{
		return this->handleRequest(buffer, bufferSize, dataLength);
	});
}
