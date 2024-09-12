#ifndef NET_H
#define NET_H

#include "baseobject.h"
#include <string>
#include <vector>

namespace DataBase {

class Module;
class NetBus;

class Net : public BaseObject
{
public:
    Net();
    Net(const std::string &name, Module *module = nullptr);

    const std::string &name() const;

    Module *module() const;

    bool isSigned() const;
    void setIsSigned(bool newIsSigned);

    NetBus *bus() const;

private:
    std::string _name;
    Module *_module = nullptr;
    NetBus *_bus = nullptr;
    bool _isSigned;
    friend class NetBus;
};

class NetBus : public BaseObject
{
public:
    NetBus();
    NetBus(const std::string &name, Module *module = nullptr);

    void add(Net* net);

    const std::string &name() const;
    int size() const;

    Module *module() const;

private:
    std::vector<Net*> _nets;
    std::string _name;
    Module *_module = nullptr;
    bool _isSigned = false;
    int _size = 0;
};

}
#endif // NET_H
