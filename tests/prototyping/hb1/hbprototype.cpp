#include "hbprototype.h"

#include "game.h"

#include <map>
#include <iostream>

#include <curl/curl.h>
#include <Python.h>

GAMECLIENTUI_CLASS(HBPrototype);

void
HBPrototype::init(int argc, const char* argv[])
{
	if(argc == 2)
		this->hbcookie = argv[1];
}

void
HBPrototype::onShutdown()
{
	std::cout << "shutdown" << std::endl;
}

size_t
HBPrototype::curlWrapper(void *buffer, size_t sz, size_t n, void *f)
{
	return static_cast<HBPrototype*>(f)->handleRequest(buffer, sz, n);
}

size_t
HBPrototype::handleRequest(void *buffer, size_t sz, size_t n)
{
	this->sstream.write(static_cast<char*>(buffer), n);
	return n;
}

void
HBPrototype::startEventLoop()
{
	std::cout << "starting hb test with cookie: " << this->hbcookie << std::endl;

	CURL *curl = curl_easy_init();
	std::string cookie = std::string("_simpleauth_sess=") + this->hbcookie;
	curl_easy_setopt(curl, CURLOPT_URL, "https://www.humblebundle.com/home");
	curl_easy_setopt(curl, CURLOPT_COOKIE, cookie.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HBPrototype::curlWrapper);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	
	CURLcode curlCode = curl_easy_perform(curl);
	
	if(curlCode != CURLE_OK)
	{
		std::cout << "error occured:  " << curl_easy_strerror(curlCode) << std::endl;
	}
	else
	{
		this->doPythonStuff();
	}
	
	curl_easy_cleanup(curl);
}

typedef struct {
  PyObject_HEAD
  void *ptr;
  void *ty;
  int own;
  PyObject *next;
  PyObject *dict;
} SwigPyObject;

static gameLibModel::Game * castPyObjectToGame(PyObject *obj)
{
	SwigPyObject * swigPyObj = reinterpret_cast<SwigPyObject *>(obj);
	return static_cast<gameLibModel::Game*>(swigPyObj->ptr);
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
				std::map<std::string, gameLibModel::Game*> games;
				
				for(int i = 0; i < PyList_Size(result); i++)
				{
					PyObject *argsGame = PyTuple_New(1);
					PyTuple_SetItem(argsGame, 0, PyList_GetItem(result, i));
					PyObject *resultGame = PyObject_CallObject(funcGetGame, argsGame);
					if (resultGame != NULL)
					{
						gameLibModel::Game * game = castPyObjectToGame(resultGame);
						games[PyUnicode_AsUTF8(PyList_GetItem(result, i))] = game;
						std::cout << game->name() << " (" << game->website() << ")" << std::endl;
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
