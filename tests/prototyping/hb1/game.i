%module GameLibPy
%{
#include "gameBind.h"
%}
struct Game
{
    void name(const char*);
    void website(const char*);
    void publisher(const char*);
};
