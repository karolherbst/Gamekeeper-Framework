#include "desuraprototype.h"

GAMECLIENTUI_CLASS(DesuraPrototype)

DesuraPrototype::DesuraPrototype(gamekeeper::core::Logger& _logger)
:	logger(_logger){}

void
DesuraPrototype::init(const ConfigMap &)
{
}

void
DesuraPrototype::onShutdown()
{
}

void
DesuraPrototype::startEventLoop()
{
}
