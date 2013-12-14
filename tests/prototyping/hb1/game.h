#include <gamelib/core/interface.h>

namespace gameLibModel
{

interface PUBLIC_API Game
{
	GAMELIB_INTERFACE_DESTRUCTOR(Game)
	GAMELIB_INTERFACE_METHOD(const char* name())
	GAMELIB_INTERFACE_METHOD(const char* website())
	GAMELIB_INTERFACE_METHOD(const char* publisher())

	GAMELIB_INTERFACE_METHOD(void name(const char*))
	GAMELIB_INTERFACE_METHOD(void website(const char*))
	GAMELIB_INTERFACE_METHOD(void publisher(const char*))
};

}
