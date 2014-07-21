%module GAMEKEEPER_BINDING_MODULE
%{
	#include <gamekeeper/core/common.h>

	#include <string>

	#include <gamekeeper/model/genericgame.h>

	GAMEKEEPER_NAMESPACE_START(bindings)
	namespace model
	{
		typedef gamekeeper::model::GenericGame Game;
	}
	GAMEKEEPER_NAMESPACE_END(bindings)
%}

%include <attribute.i>
%include <std_string.i>

namespace gamekeeper {
namespace bindings {
namespace model {
struct Game
{
	const std::string & getId() const;
	void setId(const std::string &);

	const std::string & getName() const;
	void setName(const std::string &);

	const std::string & getDescription() const;
	void setDescription(const std::string &);

	const std::string & getHomepage() const;
	void setHomepage(const std::string &);
};
}
}
}

// generate attribute like getter and setter only for language, where this is common use
#if defined(SWIGPYTHON) || defined(SWIGCSHARP)
	%attribute(gamekeeper::bindings::model::Game, const std::string &, id, getId, setId);
	%attribute(gamekeeper::bindings::model::Game, const std::string &, name, getName, setName);
	%attribute(gamekeeper::bindings::model::Game, const std::string &, description, getDescription, setDescription);
	%attribute(gamekeeper::bindings::model::Game, const std::string &, homepage, getHomepage, setHomepage);
#endif
