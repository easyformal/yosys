#include "net.h"
namespace DataBase {

Net::Net()
{

}

Net::Net(const std::string &name, Module *module)
    : _name(name)
    , _module(module)
{

}

const std::string &Net::name() const
{
    return _name;
}

Module *Net::module() const
{
    return _module;
}

void Net::setModule(Module *module)
{
    _module = module;
}

bool Net::isSigned() const
{
    return _isSigned;
}

void Net::setIsSigned(bool newIsSigned)
{
    _isSigned = newIsSigned;
}

NetBus *Net::bus() const
{
    return _bus;
}

NetBus::NetBus()
{

}
NetBus::NetBus(const std::string &name, Module *module)
    : _name(name)
    , _module(module)
{

}

void NetBus::add(Net *net)
{
    assert(net);
    _nets.emplace_back(net);
    net->_bus = this;
}
const std::string &NetBus::name() const
{
    return _name;
}

int NetBus::size() const
{
    return _nets.size();
}

Module *NetBus::module() const
{
    return _module;
}

void NetBus::setModule(Module *module)
{
    _module = module;
}
}
