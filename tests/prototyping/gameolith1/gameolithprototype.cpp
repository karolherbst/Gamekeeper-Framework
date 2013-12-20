#include "gameolithprototype.h"

#include <gamelib/core/httpfiledownloader.h>
#include <gamelib/model/game.h>

GAMECLIENTUI_CLASS(GameolithPrototype);

static std::shared_ptr<gamelib::core::HttpFileDownloader> fileDownloader;

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
	std::cout << static_cast<const char*>(buffer) << std::endl;
	return true;
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
