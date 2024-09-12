#include "instance.h"

namespace DataBase {

Instance::Instance()
{

}

Instance::Instance(const std::string &name, Module *module)
    : _name(name)
    , _module(module)
{

}

const std::string &Instance::name() const
{
    return _name;
}

Module *Instance::module() const
{
    return _module;
}

void Instance::setModule(Module *newModule)
{
    _module = newModule;
}

} // namespace DataBase
