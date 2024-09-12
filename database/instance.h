#ifndef DATABASE_INSTANCE_H
#define DATABASE_INSTANCE_H

#include "baseobject.h"
#include <string>


namespace DataBase {
class Module;

class Instance : public DataBase::BaseObject
{
public:
    Instance();
    Instance(const std::string &name, Module *module = nullptr);
    const std::string &name() const;

    Module *module() const;

    void setModule(Module *newModule);

private:
    std::string _name;
    Module *_module;

};

} // namespace DataBase

#endif // DATABASE_INSTANCE_H
