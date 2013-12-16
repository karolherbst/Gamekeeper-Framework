%module GameLibPy
%{
#include "gameBind.h"
%}

%include <attribute.i>

struct Game{};
%attribute(Game, const char*, name, getName, setName);
%attribute(Game, const char*, website, getWebsite, setWebsite);
%attribute(Game, const char*, publisher, getPublisher, setPublisher);
