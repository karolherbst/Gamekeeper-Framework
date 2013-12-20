#include "gameolithprototype.h"

#include <gamelib/core/httpfiledownloader.h>
#include <gamelib/model/game.h>

#include <json/reader.h>
#include <json/value.h>

GAMECLIENTUI_CLASS(GameolithPrototype);

using gamelib::model::Game;

static std::shared_ptr<gamelib::core::HttpFileDownloader> fileDownloader;

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
	:	idPath(std::string(".slug")),
		namePath(std::string(".title")){}

	Game * createGame(const Json::Value & gameNode)
	{
		GameJSON * game = new GameJSON();
		game->id = this->idPath.resolve(gameNode).asString();
		game->name = this->namePath.resolve(gameNode).asString();
		return game;
	}
};

void
GameolithPrototype::init(int argc, const char* argv[], Hypodermic::IContainer * container)
{
	fileDownloader = container->resolve<gamelib::core::HttpFileDownloader>();
}

void
GameolithPrototype::onShutdown()
{
	std::cout << "shutdown" << std::endl;
}

bool
GameolithPrototype::handleRequest(void * const buffer, size_t sz, size_t n)
{
	Json::Value root;
	Json::Reader reader;
	std::string jsonTree(static_cast<const char*>(buffer), n);
	if (reader.parse(jsonTree, root, false))
	{
		JSONGameResolver gr;
		std::map<const std::string, const Game *> games;
		for (int i = 0; i < root.size(); i++)
		{
			const Game * game = gr.createGame(root[i]);
			games[game->getId()] = game;
		}
		for (auto pair : games)
		{
			const Game * game = pair.second;
			std::cout << game->getName() << ' ' << game->getId() << std::endl;
		}
		return true;
	}
	return false;
}

void
GameolithPrototype::startEventLoop()
{
	std::cout << "starting gameolith test" << std::endl;
	fileDownloader->downloadFile("https://www.gameolith.com/user/karolherbst/games/?format=json",
	                                        [this](void * const buffer, size_t bufferSize, size_t dataLength) -> bool
	{
		return this->handleRequest(buffer, bufferSize, dataLength);
	});
}
