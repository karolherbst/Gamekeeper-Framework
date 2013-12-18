#include <gamelib/core/interface.h>

namespace gameLibModel
{

interface PUBLIC_API Game
{
	GAMELIB_INTERFACE_DESTRUCTOR(Game)
	GAMELIB_INTERFACE_METHOD(const char* getName() const);
	GAMELIB_INTERFACE_METHOD(const char* getWebsite() const);
	GAMELIB_INTERFACE_METHOD(const char* getPublisher() const);

	GAMELIB_INTERFACE_METHOD(void setName(const char*));
	GAMELIB_INTERFACE_METHOD(void setWebsite(const char*));
	GAMELIB_INTERFACE_METHOD(void setPublisher(const char*));
};

}
