#ifndef HBPROTOTYPE_H
#define HBPROTOTYPE_H

#include <gamekeeper/client/gamekeeper.h>

#include <sstream>

class HBPrototype : public gamekeeper::client::GameKeeperUI
{
public:
	HBPrototype(gamekeeper::core::Logger& logger);
	virtual void init(const ConfigMap &) override;
	virtual void onShutdown() override;
	virtual void startEventLoop() override;
private:
	const char * username = nullptr;
	const char * userpass = nullptr;

	gamekeeper::core::Logger& logger;

	bool handleRequest(std::basic_istream<gkbyte_t> &);
	std::string content;
	void doPythonStuff();
};

#endif // HBPROTOTYPE_H
