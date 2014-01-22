%module GAMELIB_BINDING_MODULE
%{
#include "gameBinding.h"
%}

%include <attribute.i>

namespace gamelib {
namespace model {
struct GameBinding
{
	const char * getId() const;
	void setId(const char*);
	
	const char * getName() const;
	void setName(const char*);
};
}
}

%rename(Game) GameBinding;
%attribute(gamelib::model::GameBinding, const char*, id, getId, setId);
%attribute(gamelib::model::GameBinding, const char*, name, getName, setName);
