#ifndef GAMEOLITHPROTOTYPE_H
#define GAMEOLITHPROTOTYPE_H 1

#include <gamelib/client/gamelib.h>

class GameolithPrototype : public gamelib::client::GameLibUI
{
public:
	GAMELIB_IMPLEMENTATION_OVERRIDE(void init(int argc, const char* argv[], Hypodermic::IContainer * container));
	GAMELIB_IMPLEMENTATION_OVERRIDE(void onShutdown());
	GAMELIB_IMPLEMENTATION_OVERRIDE(void startEventLoop());
private:
	bool handleRequest(void * const buffer, size_t sz, size_t n);
};

#endif
