#include "port.h"
namespace DataBase {

Port::Port()
{

}

Port::Port(const std::string &name, Direction direction, Module *module)
    : _name(name)
    , _direction(direction)
    , _module(module)
{

}

Module *Port::module() const
{
    return _module;
}

const std::string &Port::name() const
{
    return _name;
}

bool Port::isSigned() const
{
    return _isSigned;
}

void Port::setIsSigned(bool newIsSigned)
{
    _isSigned = newIsSigned;
}

PortBus *Port::bus() const
{
    return _bus;
}

int Port::id() const
{
    return _id;
}

void Port::setId(int newId)
{
    _id = newId;
}

PortBus::PortBus()
{

}

PortBus::PortBus(const std::string &name, Direction direction, Module *module)
    : _name(name)
    , _module(module)
    , _direction(direction)
{

}

void PortBus::add(Port *port)
{
    assert(port);
    _ports.emplace_back(port);
    port->_bus = this;
}

const std::string &PortBus::name() const
{
    return _name;
}

Module *PortBus::module() const
{
    return _module;
}

bool PortBus::isSigned() const
{
    return _isSigned;
}

void PortBus::setIsSigned(bool newIsSigned)
{
    _isSigned = newIsSigned;
}

int PortBus::size() const
{
    return _ports.size();
}

int PortBus::id() const
{
    return _id;
}

void PortBus::setId(int newId)
{
    _id = newId;
}

}
