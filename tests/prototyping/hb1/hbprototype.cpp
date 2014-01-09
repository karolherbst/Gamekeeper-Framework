#include "hbprototype.h"

#include <gamelib/core/httpfiledownloader.h>
#include <gamelib/model/game.h>

#include <map>
#include <iostream>

#include <Python.h>

GAMECLIENTUI_CLASS(HBPrototype);

typedef gamelib::core::HttpFileDownloader::CookieBuket CookieBuket;
typedef gamelib::core::HttpFileDownloader::Cookie Cookie;
typedef gamelib::core::HttpFileDownloader::Form Form;

static std::shared_ptr<gamelib::core::HttpFileDownloader> fileDownloader;

void
HBPrototype::init(int argc, const char* argv[], Hypodermic::IContainer * container)
{
	fileDownloader = container->resolve<gamelib::core::HttpFileDownloader>();
	if(argc == 3)
	{
		this->username = argv[1];
		this->userpass = argv[2];
	}
}

void
HBPrototype::onShutdown()
{
	std::cout << "shutdown" << std::endl;
}

bool
HBPrototype::handleRequest(void * const buffer, size_t sz, size_t n)
{
	this->sstream.write(static_cast<char*>(buffer), n);
	return true;
}

void
HBPrototype::startEventLoop()
{
	Form form;
	form["username"] = this->username;
	form["password"] = this->userpass;
	
	CookieBuket cookies = fileDownloader->doPostRequestForCookies("https://www.humblebundle.com/login", form);

	fileDownloader->downloadFileWithCookies("https://www.humblebundle.com/home",
	                                        [this](void * const buffer, size_t bufferSize, size_t dataLength) -> bool
	{
		return this->handleRequest(buffer, bufferSize, dataLength);
	}, cookies);
	
	this->doPythonStuff();
}

typedef struct {
  PyObject_HEAD
  void *ptr;
  void *ty;
  int own;
  PyObject *next;
  PyObject *dict;
} SwigPyObject;

static gamelib::model::Game * castPyObjectToGame(PyObject *obj)
{
	SwigPyObject * swigPyObj = reinterpret_cast<SwigPyObject *>(obj);
	return static_cast<gamelib::model::Game*>(swigPyObj->ptr);
}

void
HBPrototype::doPythonStuff()
{
	Py_Initialize();
	
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\"" PYTHONPATH "\")");
	PyRun_SimpleString("sys.path.append(\"" GAMELIBPYPATH "\")");
	
	PyObject *moduleName = PyUnicode_FromString(PYMODULENAME);
	PyObject *pyModule = PyImport_Import(moduleName);
	Py_DECREF(moduleName);
	
	if (pyModule != NULL)
	{
		PyObject *setTree = PyObject_GetAttrString(pyModule, "setGameHTML");
		PyObject *funcGetAll = PyObject_GetAttrString(pyModule, "parseGameIdsHTML");
		PyObject *funcGetGame = PyObject_GetAttrString(pyModule, "parseGameHTML");
		
		if (setTree && PyCallable_Check(setTree) && funcGetAll && PyCallable_Check(funcGetAll) && funcGetGame
		        && PyCallable_Check(funcGetGame))
		{
			PyObject *args = PyTuple_New(1);
			PyObject *domTree = PyUnicode_FromString(this->sstream.str().c_str());
			if(!domTree)
			{
				Py_DECREF(args);
				Py_DECREF(pyModule);
				std::cout << "Could not convert argument" << std::endl;
				return;
			}
			PyTuple_SetItem(args, 0, domTree);
			PyObject *ignore = PyObject_CallObject(setTree, args);
			Py_DECREF(ignore);
			PyObject *emptyTuple = PyTuple_New(1);
			PyTuple_SetItem(emptyTuple, 0, PyUnicode_FromString("unused"));
			PyObject *result = PyObject_CallObject(funcGetAll, emptyTuple);
			Py_DECREF(emptyTuple);
			if (result != NULL)
			{
				std::cout << "call finished, got the game list" << std::endl;
				std::map<std::string, gamelib::model::Game*> games;
				
				for(int i = 0; i < PyList_Size(result); i++)
				{
					PyObject *argsGame = PyTuple_New(1);
					PyTuple_SetItem(argsGame, 0, PyList_GetItem(result, i));
					PyObject *resultGame = PyObject_CallObject(funcGetGame, argsGame);
					if (resultGame != NULL)
					{
						gamelib::model::Game * game = castPyObjectToGame(resultGame);
						games[PyUnicode_AsUTF8(PyList_GetItem(result, i))] = game;
						std::cout << game->getName() << std::endl;
					}
					else
					{
						std::cout << "Call failed for game: " << PyUnicode_AsASCIIString(PyList_GetItem(result, i)) << std::endl;
					}
				}
				
				Py_DECREF(result);
			}
			else
			{
				Py_DECREF(funcGetAll);
				Py_DECREF(funcGetGame);
				Py_DECREF(pyModule);
				PyErr_Print();
				std::cout << "Call failed" << std::endl;
				return;
			}
		}
		else
		{
			if (PyErr_Occurred())
				PyErr_Print();
			std::cout << "Cannot find function" << std::endl;
		}
		Py_XDECREF(funcGetAll);
		Py_XDECREF(funcGetGame);
		Py_DECREF(pyModule);
	}
	else
	{
		PyErr_Print();
		std::cout << "Failed to load " << PYMODULENAME << std::endl;
		return;
	}
	
	Py_Finalize();
}
