#include "module.h"
#include "net.h"
#include "port.h"
#include "instance.h"
#include "instport.h"

namespace DataBase {

Module::Module()
{

}

Module::Module(const std::string &name)
    : _name(name)
{

}

DataBase::Module::~Module()
{

}

void Module::add(Net *net)
{

}

void Module::add(NetBus *netbus)
{

}

void Module::add(Port *port)
{

}

void Module::add(PortBus *portbus)
{

}

void Module::addInstance(Instance *inst)
{

}

void Module::addInstPort(InstPort *inst_port)
{

}

const std::unordered_map<std::string, Net *> &Module::nets() const
{
    return _nets;
}

const std::unordered_map<std::string, Port *> &Module::ports() const
{
    return _ports;
}

const std::unordered_map<std::string, InstPort *> &Module::inst_ports() const
{
    return _inst_ports;
}

const std::unordered_map<std::string, Instance *> &Module::instances() const
{
    return _instances;
}

} // namespace DataBase
