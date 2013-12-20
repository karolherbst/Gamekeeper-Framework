#include "gameolithprototype.h"

#include <gamelib/core/httpfiledownloader.h>
#include <gamelib/model/game.h>

#include <json/reader.h>
#include <json/value.h>

GAMECLIENTUI_CLASS(GameolithPrototype);

static std::shared_ptr<gamelib::core::HttpFileDownloader> fileDownloader;

class GameJSON : public gamelib::model::Game
{
private:
	Json::Value root;
public:
	GameJSON(const Json::Value& value)
	:	root(value){}

	const char * getId() const
	{
		return this->root["slug"].asCString();
	}

	const char * getName() const
	{
		return this->root["title"].asCString();
	}

	void setId(const char*){}
	void setName(const char*){}
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
		for (int i = 0; i < root.size(); i++)
		{
			GameJSON game(root[i]);
			std::cout << game.getName() << ' ' << game.getId() << std::endl;
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
