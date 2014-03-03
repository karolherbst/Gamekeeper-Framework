#ifndef HBPROTOTYPE_H
#define HBPROTOTYPE_H

#include <gamekeeper/client/gamekeeper.h>

#include <sstream>

class HBPrototype : public gamekeeper::client::GameKeeperUI
{
public:
	HBPrototype(gamekeeper::core::Logger& logger);
	GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void init(const ConfigMap &));
	GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void onShutdown());
	GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void addOptions(
                                        boost::program_options::options_description_easy_init & oaCmd,
                                        boost::program_options::options_description_easy_init & oaFile,
                                        boost::program_options::options_description_easy_init & oaBoth));
	GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void startEventLoop());
private:
	const char * username = nullptr;
	const char * userpass = nullptr;

	gamekeeper::core::Logger& logger;

	bool handleRequest(void * const buffer, size_t sz, size_t n);
	std::stringstream sstream;
	void doPythonStuff();
};

#endif // HBPROTOTYPE_H
