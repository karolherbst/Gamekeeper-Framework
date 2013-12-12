#include <gamelib/core/interface.h>

interface Game
{
	GAMELIB_INTERFACE_DESTRUCTOR(Game)
	GAMELIB_INTERFACE_METHOD(const char* name())
	GAMELIB_INTERFACE_METHOD(const char* website())
	GAMELIB_INTERFACE_METHOD(const char* publisher())
};
