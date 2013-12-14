#include "gameBind.h"

#include <string>

Game::Game()
:	_name("test binded Game"){}

const char*
Game::name()
{
	return this->_name.c_str();
}

const char*
Game::website()
{
	return this->_website.c_str();
}

const char*
Game::publisher()
{
	return this->_publisher.c_str();
}

void
Game::name(const char* n)
{
	this->_name = n;
}

void
Game::website(const char* w)
{
	this->_website = w;
}

void
Game::publisher(const char* p)
{
	this->_publisher = p;
}
