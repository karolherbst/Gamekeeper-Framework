#ifndef GAMEOLITHPROTOTYPE_H
#define GAMEOLITHPROTOTYPE_H 1

#include <gamelib/client/gamelib.h>

class GameolithPrototype : public gamelib::client::GameLibUI
{
public:
	GameolithPrototype(gamelib::core::Logger& logger);
	GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void init(const ConfigMap &));
	GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void onShutdown());
	GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void startEventLoop());
private:
	bool handleRequest(void * const buffer, size_t sz, size_t n);
	gamelib::core::Logger& logger;
};

#endif
