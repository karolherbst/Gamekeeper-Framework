#ifndef GAMEOLITHPROTOTYPE_H
#define GAMEOLITHPROTOTYPE_H 1

#include <gamekeeper/client/gamekeeper.h>

#include <istream>

class GameolithPrototype : public gamekeeper::client::GameKeeperUI
{
public:
	GameolithPrototype(gamekeeper::core::Logger& logger);
	GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void init(const ConfigMap &));
	GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void onShutdown());
	GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void startEventLoop());
private:
	bool handleRequest(std::basic_istream<gkbyte_t> &);
	gamekeeper::core::Logger& logger;
};

#endif
