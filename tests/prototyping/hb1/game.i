%module GameLibPy
%{
#include "gameBind.h"
%}

%include <attribute.i>

struct Game
{
	const char * getName() const;
	void setName(const char*);
	
	const char * getWebsite() const;
	void setWebsite(const char*);
	
	const char * getPublisher() const;
	void setPublisher(const char*);
};

%attribute(Game, const char*, name, getName, setName);
%attribute(Game, const char*, website, getWebsite, setWebsite);
%attribute(Game, const char*, publisher, getPublisher, setPublisher);
