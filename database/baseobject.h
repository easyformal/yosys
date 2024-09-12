#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <set>
#include <string>
#include <unordered_map>
#include <cassert>

namespace DataBase {

class Attribute
{
public:
    Attribute() {}
    Attribute(const std::string &key, const std::string &value);

    const std::string &key() const;
    void setKey(const std::string &newKey);

    const std::string &value() const;
    void setValue(const std::string &newValue);

private:
    std::string _key;
    std::string _value;
};

class BaseObject
{
public:
    BaseObject();

    bool hasAttribute(const std::string &key);
    void setBoolAttribute(const std::string &key, bool value = true);
    void setStringAttribute(const std::string &key, const std::string &value);

private:
    std::unordered_map<std::string, Attribute*> _attributes;
};

}
#endif // BASEOBJECT_H
