#include "game.h"

#include <string>

class PUBLIC_API Game : public gameLibModel::Game
{
private:
	std::string _name;
	std::string _website;
	std::string _publisher;
	
public:
	Game();
	GAMELIB_INTERFACE_DESTRUCTOR(Game)
	GAMELIB_IMPLEMENTATION_OVERRIDE(const char* name());
	GAMELIB_IMPLEMENTATION_OVERRIDE(const char* website());
	GAMELIB_IMPLEMENTATION_OVERRIDE(const char* publisher());
	
	GAMELIB_IMPLEMENTATION_OVERRIDE(void name(const char*));
	GAMELIB_IMPLEMENTATION_OVERRIDE(void website(const char*));
	GAMELIB_IMPLEMENTATION_OVERRIDE(void publisher(const char*));
};
