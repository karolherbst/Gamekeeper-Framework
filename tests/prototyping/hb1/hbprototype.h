#ifndef HBPROTOTYPE_H
#define HBPROTOTYPE_H

#include <gamelib/client/gamelib.h>

#include <sstream>

class HBPrototype : public gamelib::client::GameLibUI
{
public:
	GAMELIB_IMPLEMENTATION_OVERRIDE(void init(int argc, const char* argv[], Hypodermic::IContainer * container));
	GAMELIB_IMPLEMENTATION_OVERRIDE(void onShutdown());
	GAMELIB_IMPLEMENTATION_OVERRIDE(void startEventLoop());
private:
	const char* hbcookie = nullptr;
	
	bool handleRequest(void * const buffer, size_t sz, size_t n);
	std::stringstream sstream;
	void doPythonStuff();
};

#endif // HBPROTOTYPE_H
