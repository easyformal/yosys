#include "module.h"
#include "net.h"
#include "port.h"
#include "instance.h"
#include "instport.h"

namespace DataBase {

Module::Module()
{

}

const std::string &Module::name() const
{
    return _name;
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
    assert(net != nullptr);
    assert(_nets.count(net->name()) == 0);

    _nets[net->name()] = net;
    net->setModule(this); 
}

void Module::add(NetBus *netbus)
{
    assert(netbus != nullptr);
    assert(_netbus.count(netbus->name()) == 0);

    _netbus[netbus->name()] = netbus;
    netbus->setModule(this); 
}

void Module::add(Port *port)
{
    assert(port != nullptr);
    assert(_ports.count(port->name()) == 0);

    _ports[port->name()] = port;
    port->setModule(this); 
}

void Module::add(PortBus *portbus)
{
    assert(portbus != nullptr);
    assert(_portbus.count(portbus->name()) == 0);

    _portbus[portbus->name()] = portbus;
    portbus->setModule(this); 
}

void Module::add(Instance *inst)
{
    assert(inst != nullptr);
    assert(_instances.count(inst->name()) == 0);

    _instances[inst->name()] = inst;
    inst->setModule(this); 
}

void Module::add(InstPort *inst_port)
{
    assert(inst_port != nullptr);

    inst_port->setModule(this); 
}

const std::unordered_map<std::string, Net *> &Module::nets() const
{
    return _nets;
}

const std::unordered_map<std::string, NetBus *> &Module::netbus() const
{
    return _netbus;
}

const std::unordered_map<std::string, Port *> &Module::ports() const
{
    return _ports;
}

const std::unordered_map<std::string, PortBus *> &Module::portbus() const
{
    return _portbus;
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
