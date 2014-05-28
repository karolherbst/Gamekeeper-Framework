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
		PUBLIC_INLINE inline virtual const std::string & getId() const override
		{
			return this->id;
		}

		PUBLIC_INLINE inline virtual const std::string & getName() const override
		{
			return this->name;
		}

		PUBLIC_INLINE inline virtual const std::set<gamekeeper::model::Platform> & getPlatforms() const override
		{
			static std::set<gamekeeper::model::Platform> ps;
			return ps;
		}

		PUBLIC_INLINE inline void setId(const std::string & _id)
		{
			this->id = _id;
		}

		PUBLIC_INLINE inline void setName(const std::string & _name)
		{
			this->name = _name;
		}
	};

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
};
}
}
}

// generate attribute like getter and setter only for language, where this is common use
#if defined(SWIGPYTHON) || defined(SWIGCSHARP)
	%attribute(gamekeeper::bindings::model::Game, const std::string &, id, getId, setId);
	%attribute(gamekeeper::bindings::model::Game, const std::string &, name, getName, setName);
#endif
