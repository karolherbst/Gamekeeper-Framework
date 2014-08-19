#include "cursesprototype.h"

#include <cwidget/toplevel.h>

namespace cw = cwidget;

GAMECLIENTUI_CLASS(CursesPrototype)

CursesPrototype::CursesPrototype(gamekeeper::core::Logger& _logger)
:	logger(_logger){}

void
CursesPrototype::init(const ConfigMap & cm)
{
	cw::toplevel::init();
}

void
CursesPrototype::onShutdown()
{
	cw::toplevel::shutdown();
}

void
CursesPrototype::startEventLoop()
{
	cw::toplevel::mainloop();
}
