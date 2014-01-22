%module GAMELIB_BINDING_MODULE
%{
#include "gameBinding.h"
%}

%include <attribute.i>

%rename(Game) gamelib::model::bindings::GameBinding;

namespace gamelib {
namespace model {
namespace bindings {
struct GameBinding
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
	%attribute(gamelib::model::bindings::GameBinding, const char*, id, getId, setId);
	%attribute(gamelib::model::bindings::GameBinding, const char*, name, getName, setName);
#endif
