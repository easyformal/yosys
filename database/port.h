#ifndef PORT_H
#define PORT_H

#include "baseobject.h"
#include <string>
#include <vector>

namespace DataBase {
class Module;
class PortBus;

enum Direction {
    Input,
    Output,
    Inout,
    Unknown
};

class Port : public BaseObject
{
public:
    Port();
    Port(const std::string &name, Direction direction, Module *module = nullptr);

    Module *module() const;

    const std::string &name() const;

    bool isSigned() const;
    void setIsSigned(bool newIsSigned);

    PortBus *bus() const;

    int id() const;
    void setId(int newId);

private:
    std::string _name;
    Direction _direction = Direction::Unknown;
    bool _isSigned;
    Module *_module = nullptr;
    PortBus *_bus = nullptr;
    int _index = -1;
    int _id = 0;

    friend class PortBus;
};

class PortBus : public BaseObject
{
public:
    PortBus();
    PortBus(const std::string &name, Direction direction, Module *module = nullptr);

    void add(Port* port);

    const std::string &name() const;

    Module *module() const;

    bool isSigned() const;
    void setIsSigned(bool newIsSigned);

    int size() const;

    int id() const;
    void setId(int newId);

private:
    std::vector<Port*> _ports;
    std::string _name;
    Module *_module = nullptr;
    Direction _direction = Direction::Unknown;
    bool _isSigned = false;
    int _id = 0;

};
}
#endif // PORT_H
