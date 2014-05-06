%module GAMEKEEPER_BINDING_MODULE
%{
	#include <gamekeeper/core/common.h>

	#include <string>

	#include <gamekeeper/model/game.h>

	GAMEKEEPER_NAMESPACE_START(bindings)
	namespace model
	{

	class PUBLIC_API Game : public gamekeeper::model::Game
	{
	private:
		std::string id;
		std::string name;

	public:
		PUBLIC_INLINE inline GAMEKEEPER_IMPLEMENTATION_OVERRIDE(const char * getId() const)
		{
			return this->id.c_str();
		}

		PUBLIC_INLINE inline GAMEKEEPER_IMPLEMENTATION_OVERRIDE(const char * getName() const)
		{
			return this->name.c_str();
		}

		PUBLIC_INLINE inline void setId(const char * _id)
		{
			this->id = _id;
		}

		PUBLIC_INLINE inline void setName(const char * _name)
		{
			this->name = _name;
		}
	};

	}
	GAMEKEEPER_NAMESPACE_END(bindings)
%}

%include <attribute.i>

namespace gamekeeper {
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
	%attribute(gamekeeper::bindings::model::Game, const char*, id, getId, setId);
	%attribute(gamekeeper::bindings::model::Game, const char*, name, getName, setName);
#endif
