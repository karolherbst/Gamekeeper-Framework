#include "game.h"

#include <gamelib/core/common.h>

#include <string>

#include <Python.h>

class GamePy : public Game
{
private:
	PyObject_HEAD
	std::string _name;
	std::string _website;
	std::string _publisher;
	
public:
	GAMELIB_IMPLEMENTATION_OVERRIDE(const char* name())
	{
		return this->_name.c_str();
	}

	GAMELIB_IMPLEMENTATION_OVERRIDE(const char* website())
	{
		return this->_website.c_str();
	}

	GAMELIB_IMPLEMENTATION_OVERRIDE(const char* publisher())
	{
		return this->_publisher.c_str();
	}
};

static PyTypeObject gamelibGameType = {
	PyVarObject_HEAD_INIT(&PyType_Type, 0)
	"GameLib.Game",			/* tp_name */
	sizeof(GamePy),			/* tp_basicsize */
	0,						/* tp_itemsize */
	0,						/* tp_dealloc */
	0,						/* tp_print */
	0,						/* tp_getattr */
	0,						/* tp_setattr */
	0,						/* tp_reserved */
	0,						/* tp_repr */
	0,						/* tp_as_number */
	0,						/* tp_as_sequence */
	0,						/* tp_as_mapping */
	0,						/* tp_hash  */
	0,						/* tp_call */
	0,						/* tp_str */
	0,						/* tp_getattro */
	0,						/* tp_setattro */
	0,						/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,		/* tp_flags */
	"GameLib Game class",	/* tp_doc */
};

static PyModuleDef gamelibModule = {
	PyModuleDef_HEAD_INIT,
	"GameLib",
	"GameLib Python Bindings",
	-1,
	nullptr, nullptr, nullptr, nullptr, nullptr
};

extern "C" PUBLIC_API PyObject*
PyInit_libGameLibPy()
{
	PyObject* m;

	gamelibGameType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&gamelibGameType) < 0)
		return nullptr;

	m = PyModule_Create(&gamelibModule);
	if (m == nullptr)
		return nullptr;

	Py_INCREF(&gamelibGameType);
	PyModule_AddObject(m, "Game", (PyObject *)&gamelibGameType);
	return m;
}
