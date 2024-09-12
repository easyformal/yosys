#ifndef DATABASE_LIBSET_H
#define DATABASE_LIBSET_H



#include <map>
#include <string>
#include <vector>
// #include <nlohmann/json.hpp>


namespace DataBase {
class Design;

class DesignSet
{
public:
    static DesignSet *instance();
    static void init();
    static void destroy();

    Design *design(const std::string &name);
    bool hasDesign(const std::string &name) const;
    void add(Design *design);
    Design *addDesign(const std::string &name);
    // Design *addDesign(const nlohmann::json &j);

    // nlohmann::json toJson() const ;
    // void fromJson(const nlohmann::json &j);
    const std::string &version() const;
    void setVersion(const std::string &newVersion);


    const std::map<std::string, Design *> &designs() const;

    Design *currentDesign() const;
    void setCurrentDesign(Design *newCurrentDesign);

private:
    DesignSet();
    ~DesignSet();

private:
    std::map<std::string, Design*> _designs;
    Design* _currentDesign;

    // property
    std::string _version;

};

} // namespace DataBase

#endif // DATABASE_LIBSET_H
