#ifndef GAMEOLITHPROTOTYPE_H
#define GAMEOLITHPROTOTYPE_H 1

#include <gamekeeper/client/gamelib.h>

class GameolithPrototype : public gamekeeper::client::GameLibUI
{
public:
	GameolithPrototype(gamekeeper::core::Logger& logger);
	GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void init(const ConfigMap &));
	GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void onShutdown());
	GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void startEventLoop());
private:
	bool handleRequest(void * const buffer, size_t sz, size_t n);
	gamekeeper::core::Logger& logger;
};

#endif
