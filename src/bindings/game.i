%module GAMELIB_BINDING_MODULE
%{
#include "game.h"
%}

%include <attribute.i>

namespace gamelib {
namespace bindings {
namespace model {
struct Game
{
	const char * getId() const;
	void setId(const char*);
	
	const char * getName() const;
	void setName(const char*);
};
}
}
}

// generate attribute like getter and setter only for language, where this is common use
#if defined(SWIGPYTHON) || defined(SWIGCSHARP)
	%attribute(gamelib::bindings::model::Game, const char*, id, getId, setId);
	%attribute(gamelib::bindings::model::Game, const char*, name, getName, setName);
#endif
