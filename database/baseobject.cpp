#include "baseobject.h"
namespace DataBase {

BaseObject::BaseObject()
{

}

bool BaseObject::hasAttribute(const std::string &key)
{
    return _attributes.count(key);
}

void BaseObject::setBoolAttribute(const std::string &key, bool value)
{
    std::string str_value;
    if (value) {
        str_value = "TRUE";
    }
    else {
        str_value = "FALSE";
    }
    if (_attributes.count(key)) {
        _attributes[key]->setValue(str_value);
    }
    else {
        Attribute *att = new Attribute(key, str_value);
        _attributes[key] = att;
    }
}

const std::string &Attribute::key() const
{
    return _key;
}

void Attribute::setKey(const std::string &newKey)
{
    _key = newKey;
}

const std::string &Attribute::value() const
{
    return _value;
}

void Attribute::setValue(const std::string &newValue)
{
    _value = newValue;
}

Attribute::Attribute(const std::string &key, const std::string &value) : _key(key),
    _value(value)
{}

}
