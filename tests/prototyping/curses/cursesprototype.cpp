#include "cursesprototype.h"

GAMECLIENTUI_CLASS(CursesPrototype)

CursesPrototype::CursesPrototype(gamekeeper::core::Logger& _logger)
:	logger(_logger){}

void
CursesPrototype::init(const ConfigMap & cm)
{
}

void
CursesPrototype::onShutdown()
{
}

void
CursesPrototype::startEventLoop()
{
}
