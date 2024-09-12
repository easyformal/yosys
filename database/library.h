#ifndef DATABASE_LIBRARY_H
#define DATABASE_LIBRARY_H

#include <unordered_map>
#include <string>
#include <vector>


namespace DataBase {
class Module;

class Library
{
public:
    Library(const std::string &name);

    std::unordered_map<std::string, Module*> modules();
    Module *module(const std::string &name);
    Module *topModule();

    bool hasModule(const std::string &name) const;
    void add(Module *module);
    Module *addModule(const std::string &name);

    const std::string &name() const;
    void setName(const std::string &newName);

private:
    std::unordered_map<std::string, Module*> _modules;
    Module *_top_module = nullptr;
    std::string _name;
};

} // namespace DataBase

#endif // DATABASE_LIBRARY_H
