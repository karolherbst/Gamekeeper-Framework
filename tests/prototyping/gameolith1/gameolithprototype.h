#ifndef GAMEOLITHPROTOTYPE_H
#define GAMEOLITHPROTOTYPE_H 1

#include <gamekeeper/client/gamekeeper.h>

#include <istream>

class GameolithPrototype : public gamekeeper::client::GameKeeperUI
{
public:
	GameolithPrototype(gamekeeper::core::Logger& logger);
	virtual void init(const ConfigMap &) override;
	virtual void onShutdown() override;
	virtual void startEventLoop() override;
private:
	bool handleRequest(std::basic_istream<gkbyte_t> &);
	gamekeeper::core::Logger& logger;
};

#endif
