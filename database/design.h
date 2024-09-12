#ifndef DATABASE_DESIGN_H
#define DATABASE_DESIGN_H


#include <unordered_map>
#include <string>
#include <vector>
// #include <nlohmann/json.hpp>

namespace DataBase {

class Library;

class Design
{
public:
    Design();
    Design(const std::string &name);

    Library *library() const;
    void setLibrary(Library *newLibrary);

    const std::string &name() const;
    void setName(const std::string &newName);

    const std::vector<std::string> &filelists() const;
    void addFilelist(const std::string &file);
    void addFilelist(const std::vector<std::string> &files);
    void setFilelists(const std::vector<std::string> &newFilelists);

    const std::vector<std::string> &systemverilogFiles() const;
    void addSystemVerilogFile(const std::string &file);
    void addSystemVerilogFile(const std::vector<std::string> &files);
    void setSystemverilogFiles(const std::vector<std::string> &newSystemverilogFiles);

    const std::vector<std::string> &verilogFiles() const;
    void addVerilogFile(const std::vector<std::string> &files);
    void addVerilogFile(const std::string &file);
    void setVerilogFiles(const std::vector<std::string> &newVerilogFiles);

    const std::vector<std::string> &vhdlFiles() const;
    void setVhdlFiles(const std::vector<std::string> &newVhdlFiles);

    // nlohmann::json toJson() const ;
    // void fromJson(const nlohmann::json& j);

    const std::string &topModuleName() const;
    void setTopModuleName(const std::string &newTopModuleName);

private:
    Library *_library;

    // property
    std::string _name;
    std::string _topModuleName;
    std::vector<std::string> _filelists;
    std::vector<std::string> _systemverilogFiles;
    std::vector<std::string> _verilogFiles;
    std::vector<std::string> _vhdlFiles;

};

} // namespace DataBase

#endif // DATABASE_DESIGN_H
