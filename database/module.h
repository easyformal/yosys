#ifndef DATABASE_CELL_H
#define DATABASE_CELL_H

#include "baseobject.h"
#include <string>
#include <unordered_map>

namespace DataBase {

class Library;
class Net;
class NetBus;
class Port;
class PortBus;
class Instance;
class InstPort;

class Module : public BaseObject
{
public:
    Module();
    Module(const std::string &name);
    ~Module();

    const std::string &name() const;

    void add(Net *net);
    void add(NetBus *netbus);
    void add(Port *port);
    void add(PortBus *portbus);
    void add(Instance *inst);
    void add(InstPort *inst_port);

    const std::unordered_map<std::string, Net *> &nets() const;
    const std::unordered_map<std::string, NetBus *> &netbus() const;

    const std::unordered_map<std::string, Port *> &ports() const;
    const std::unordered_map<std::string, PortBus *> &portbus() const;

    const std::unordered_map<std::string, InstPort *> &inst_ports() const;

    const std::unordered_map<std::string, Instance *> &instances() const;

private:
    std::string _name;
    Library *_library;

    std::unordered_map<std::string, Net*> _nets;
    std::unordered_map<std::string, NetBus*> _netbus;
    std::unordered_map<std::string, Port*> _ports;
    std::unordered_map<std::string, PortBus*> _portbus;
    std::unordered_map<std::string, InstPort*> _inst_ports;
    std::unordered_map<std::string, Instance*> _instances;


    friend class Library;

};

} // namespace DataBase

#endif // DATABASE_CELL_H
