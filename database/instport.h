#ifndef INSTPORT_H
#define INSTPORT_H

#include <string>


namespace DataBase {
class Module;
class Port;
class Instance;

class InstPort
{
public:
    InstPort();

    Module *module() const;
    void setModule(Module *module);

    Port *port() const;

    Instance *inst() const;

private:
    Module *_module;
    Port *_port;
    Instance *_inst;
};
}
#endif // INSTPORT_H
