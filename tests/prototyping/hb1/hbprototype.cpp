#include "hbprototype.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include <gamekeeper/client/autowire.h>
#include <gamekeeper/core/logger.h>
#include <gamekeeper/core/loggerStream.h>
#include <gamekeeper/core/network/cookie.h>
#include <gamekeeper/core/network/filedownloader.h>
#include <gamekeeper/model/game.h>
#include <gamekeeper/utils/stringutils.h>

#include <map>
#include <iostream>

#include <Python.h>

GAMECLIENTUI_CLASS(HBPrototype)

using namespace gamekeeper::core;
using namespace gamekeeper::core::network;
using namespace gamekeeper::utils;
namespace po = boost::program_options;

typedef FileDownloader::CookieBucket CookieBucket;
typedef FileDownloader::Form Form;

static std::shared_ptr<FileDownloader> fileDownloader;

HBPrototype::HBPrototype(gamekeeper::core::Logger& _logger)
:	logger(_logger){}

GAMECLIENT_ADD_OPTIONS({
	cmd("hb.accountname", po::value<std::string>()->required(), "HB Account name");
	cmd("hb.password", po::value<std::string>()->required(), "HB Account password");
})

void
HBPrototype::init(const ConfigMap & configMap)
{
	fileDownloader = gamekeeper::client::Autowire<FileDownloader>();

	this->logger << LogLevel::Info << "init" << endl;

	this->username = configMap.at("hb.accountname").as<std::string>().c_str();
	this->userpass = configMap.at("hb.password").as<std::string>().c_str();
}

void
HBPrototype::onShutdown()
{
	this->logger << LogLevel::Info << "shutdown" << endl;
}

bool
HBPrototype::handleRequest(FileDownloader::ByteIstream & is)
{
	this->content = String::createFromStream(is);
	return true;
}

void
HBPrototype::startEventLoop()
{
	namespace p = std::placeholders;
	Form form;
	form["username"] = this->username;
	form["password"] = this->userpass;

	fileDownloader->postRequest("https://www.humblebundle.com/login", form);
	CookieBucket cookies = fileDownloader->getCookies();
	fileDownloader->setCookies(cookies);
	fileDownloader->getRequest("https://www.humblebundle.com/home", std::bind(&HBPrototype::handleRequest, this, p::_1));
	this->doPythonStuff();
}

bool
HBPrototype::isPortable()
{
	return false;
}

typedef struct {
  PyObject_HEAD
  void *ptr;
} SwigPyObject;

static gamekeeper::model::Game * castPyObjectToGame(PyObject *obj)
{
	SwigPyObject * swigPyObj = reinterpret_cast<SwigPyObject *>(obj);
	return static_cast<gamekeeper::model::Game*>(swigPyObj->ptr);
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
			PyObject *domTree = PyUnicode_FromString(this->content.c_str());
			if(!domTree)
			{
				Py_DECREF(args);
				Py_DECREF(pyModule);
				this->logger << LogLevel::Error << "Could not convert argument" << endl;
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
				this->logger << LogLevel::Info << "call finished, got the game list" << endl;
				std::map<std::string, gamekeeper::model::Game*> games;
				
				for(int i = 0; i < PyList_Size(result); i++)
				{
					PyObject *argsGame = PyTuple_New(1);
					PyTuple_SetItem(argsGame, 0, PyList_GetItem(result, i));
					PyObject *resultGame = PyObject_CallObject(funcGetGame, argsGame);
					if (resultGame != NULL)
					{
						gamekeeper::model::Game * game = castPyObjectToGame(resultGame);
						games[PyUnicode_AsUTF8(PyList_GetItem(result, i))] = game;
						this->logger << LogLevel::Debug << game->getName() << endl;
					}
					else
					{
						this->logger << LogLevel::Error << "Call failed for game: " << PyUnicode_AsASCIIString(PyList_GetItem(result, i)) << endl;
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
				this->logger << LogLevel::Error << "Call failed" << endl;
				return;
			}
		}
		else
		{
			if (PyErr_Occurred())
				PyErr_Print();
			this->logger << LogLevel::Error << "Cannot find function" << endl;
		}
		Py_XDECREF(funcGetAll);
		Py_XDECREF(funcGetGame);
		Py_DECREF(pyModule);
	}
	else
	{
		PyErr_Print();
		this->logger << LogLevel::Error << "Failed to load " << PYMODULENAME << endl;
		return;
	}
	
	Py_Finalize();
}
