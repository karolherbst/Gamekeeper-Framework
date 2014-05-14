#ifndef DESURAPROTOTYPE_H
#define DESURAPROTOTYPE_H 1

#include <gamekeeper/client/gamekeeper.h>

class DesuraPrototype : public gamekeeper::client::GameKeeperUI
{
public:
	DesuraPrototype(gamekeeper::core::Logger& logger);
	GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void init(const ConfigMap &));
	GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void onShutdown());
	GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void startEventLoop());
private:
	gamekeeper::core::Logger& logger;

	const char * username = nullptr;
	const char * userpass = nullptr;
};

#endif
