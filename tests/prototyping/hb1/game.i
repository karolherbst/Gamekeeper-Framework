%module GameLibPy
%{
#include "gameBind.h"
%}
struct Game
{
    const char* name();
    const char* website();
    const char* publisher();
    
    void name(const char*);
    void website(const char*);
    void publisher(const char*);
};
