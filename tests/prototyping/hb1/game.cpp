#include "gameBind.h"

#include <string>

Game::Game()
:	name("test binded Game"){}

const char*
Game::getName() const
{
	return this->name.c_str();
}

const char*
Game::getWebsite() const
{
	return this->website.c_str();
}

const char*
Game::getPublisher() const
{
	return this->publisher.c_str();
}

void
Game::setName(const char* n)
{
	this->name = n;
}

void
Game::setWebsite(const char* w)
{
	this->website = w;
}

void
Game::setPublisher(const char* p)
{
	this->publisher = p;
}
