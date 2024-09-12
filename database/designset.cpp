#include "designset.h"
#include "design.h"
#include <assert.h>

// using json = nlohmann::json;

namespace DataBase {

static DesignSet *m_instance = nullptr;

DesignSet::DesignSet()
{

}

DesignSet::~DesignSet()
{
    for (const auto &pair : _designs) {
        if (pair.second != nullptr) {
            delete pair.second;
        }
    }
    _designs.clear();
}

Design *DesignSet::currentDesign() const
{
    return _currentDesign;
}

void DesignSet::setCurrentDesign(Design *newCurrentDesign)
{
    assert(newCurrentDesign);
    _currentDesign = newCurrentDesign;
}

const std::map<std::string, Design *> &DesignSet::designs() const
{
    return _designs;
}

const std::string &DesignSet::version() const
{
    return _version;
}

void DesignSet::setVersion(const std::string &newVersion)
{
    _version = newVersion;
}

// nlohmann::json DesignSet::toJson() const
// {
//     json j;
//     j["version"] = _version;

//     for (const auto &design : _designs) {
//         if (design.second == nullptr) continue;
//         j["designs"].emplace_back(design.second->toJson());
//     }
//     return j;
// }

// void DesignSet::fromJson(const nlohmann::json &j)
// {
//     _version = j.at("version").get<std::string>();
//     _designs.clear();
//     for (const auto& projectJson : j.at("designs")) {
//         Design *design = new Design();
//         design->fromJson(projectJson);
//         _designs[design->name()] = design;
//     }
// }

DesignSet *DesignSet::instance()
{
    return m_instance;
}

void DesignSet::init()
{
    m_instance = new DesignSet;
}

void DesignSet::destroy()
{
    delete m_instance;
    m_instance = nullptr;
}

Design *DesignSet::design(const std::string &name)
{
    return _designs.count(name) ? _designs[name] : nullptr;
}

bool DesignSet::hasDesign(const std::string &name) const
{
    return _designs.count(name);
}

void DesignSet::add(Design *design)
{
    assert(design != nullptr);
    assert(_designs.count(design->name()) == 0);

    _designs[design->name()] = design;
}

Design *DesignSet::addDesign(const std::string &name)
{
    assert(_designs.count(name) == 0);
    Design *lib = new Design(name);
    _designs[name] = lib;
    return lib;
}

// Design *DesignSet::addDesign(const nlohmann::json &j)
// {
//     Design *lib = new Design();
//     lib->fromJson(j);
//     _designs[lib->name()] = lib;
//     return lib;
// }


} // namespace DataBase
