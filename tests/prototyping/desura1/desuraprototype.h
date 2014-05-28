#ifndef DESURAPROTOTYPE_H
#define DESURAPROTOTYPE_H 1

#include <gamekeeper/client/gamekeeper.h>

class DesuraPrototype : public gamekeeper::client::GameKeeperUI
{
public:
	DesuraPrototype(gamekeeper::core::Logger& logger);
	virtual void init(const ConfigMap &) override;
	virtual void onShutdown() override;
	virtual void startEventLoop() override;
private:
	gamekeeper::core::Logger& logger;

	std::string username;
	std::string userpass;
};

#endif
