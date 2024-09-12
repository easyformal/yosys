#include "library.h"
#include <assert.h>
#include "module.h"

namespace DataBase {

Library::Library(const std::string &name)
    : _name(name)
{

}

std::unordered_map<std::string, Module*> Library::modules()
{
    return _modules;
}

Module *Library::module(const std::string &name)
{
    return _modules.count(name) ? _modules[name] : nullptr;
}

Module *Library::topModule()
{
    return _top_module;
}

bool Library::hasModule(const std::string &name) const
{
    return _modules.count(name);
}

void Library::add(Module *module)
{
    assert(module != nullptr);
    assert(_modules.count(module->_name) == 0);

    _modules[module->_name] = module;
    module->_library = this;
}

Module * Library::addModule(const std::string &name)
{
    assert(_modules.count(name) == 0);
    Module *module = new Module;
    module->_library = this;
    module->_name = name;
    _modules[name] = module;
    return module;
}

const std::string &Library::name() const
{
    return _name;
}

void Library::setName(const std::string &newName)
{
    _name = newName;
}

} // namespace DataBase
