#pragma once

#include <gamekeeper/client/gamekeeper.h>

class GenericPrototype : public gamekeeper::client::GameKeeperUI
{
public:
	GenericPrototype(gamekeeper::core::Logger& logger);
	virtual void init(const ConfigMap &) override;
	virtual void onShutdown() override;
	virtual void startEventLoop() override;
	virtual bool isPortable() override;
private:
	gamekeeper::core::Logger& logger;
};
