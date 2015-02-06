#pragma once

#include <gamekeeper/client/gamekeeper.h>

class CursesPrototype : public gamekeeper::client::GameKeeperUI
{
public:
	CursesPrototype(gamekeeper::core::Logger& logger);
	virtual void init(const ConfigMap &) override;
	virtual void onShutdown() override;
	virtual void startEventLoop() override;
	virtual bool isPortable() override;
private:
	gamekeeper::core::Logger& logger;
};
