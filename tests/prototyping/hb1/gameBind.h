#include "game.h"

#include <string>

class PUBLIC_API Game : public gameLibModel::Game
{
private:
	std::string name;
	std::string website;
	std::string publisher;
	
public:
	Game();
	GAMELIB_INTERFACE_DESTRUCTOR(Game)
	GAMELIB_IMPLEMENTATION_OVERRIDE(const char* getName() const);
	GAMELIB_IMPLEMENTATION_OVERRIDE(const char* getWebsite() const);
	GAMELIB_IMPLEMENTATION_OVERRIDE(const char* getPublisher() const);
	
	GAMELIB_IMPLEMENTATION_OVERRIDE(void setName(const char*));
	GAMELIB_IMPLEMENTATION_OVERRIDE(void setWebsite(const char*));
	GAMELIB_IMPLEMENTATION_OVERRIDE(void setPublisher(const char*));
};
