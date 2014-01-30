#include "hbprototype.h"

#include <gamelib/client/autowire.h>
#include <gamelib/core/httpfiledownloader.h>
#include <gamelib/core/logger.h>
#include <gamelib/core/loggerStream.h>
#include <gamelib/model/game.h>

#include <map>
#include <iostream>

#include <Python.h>

GAMECLIENTUI_CLASS(HBPrototype)

using namespace gamelib::core;

typedef HttpFileDownloader::CookieBuket CookieBuket;
typedef HttpFileDownloader::Cookie Cookie;
typedef HttpFileDownloader::Form Form;

static std::shared_ptr<HttpFileDownloader> fileDownloader;

HBPrototype::HBPrototype(gamelib::core::Logger& _logger)
:	logger(_logger){}

void
HBPrototype::init(int argc, const char* argv[])
{
	fileDownloader = gamelib::client::Autowire<gamelib::core::HttpFileDownloader>();

	this->logger << LOG_LEVEL::INFO << "init" << endl;

	if(argc == 3)
	{
		this->username = argv[1];
		this->userpass = argv[2];
	}
}

void
HBPrototype::onShutdown()
{
	this->logger << LOG_LEVEL::INFO << "shutdown" << endl;
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
	PyRun_SimpleString("sys.path.append(\"" PYMODULEPATH "\")");
	PyRun_SimpleString("sys.path.append(\"" PYTHONPATH "\")");
	
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
				this->logger << LOG_LEVEL::ERROR << "Could not convert argument" << endl;
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
				this->logger << LOG_LEVEL::INFO << "call finished, got the game list" << endl;
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
						this->logger << LOG_LEVEL::DEBUG << game->getName() << endl;
					}
					else
					{
						this->logger << LOG_LEVEL::ERROR << "Call failed for game: " << PyUnicode_AsASCIIString(PyList_GetItem(result, i)) << endl;
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
				this->logger << LOG_LEVEL::ERROR << "Call failed" << endl;
				return;
			}
		}
		else
		{
			if (PyErr_Occurred())
				PyErr_Print();
			this->logger << LOG_LEVEL::ERROR << "Cannot find function" << endl;
		}
		Py_XDECREF(funcGetAll);
		Py_XDECREF(funcGetGame);
		Py_DECREF(pyModule);
	}
	else
	{
		PyErr_Print();
		this->logger << LOG_LEVEL::ERROR << "Failed to load " << PYMODULENAME << endl;
		return;
	}
	
	Py_Finalize();
}
