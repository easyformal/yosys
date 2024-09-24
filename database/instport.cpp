#include "instport.h"
#include "module.h"
#include "instance.h"
#include "port.h"

namespace DataBase {

InstPort::InstPort()
{

}


Module *InstPort::module() const
{
    return _module;
}

void InstPort::setModule(Module *module)
{
    _module = module;
}
Port *InstPort::port() const
{
    return _port;
}

Instance *InstPort::inst() const
{
    return _inst;
}
}
