#include "design.h"
// using json = nlohmann::json;

namespace DataBase {

Design::Design()
{

}

Design::Design(const std::string &name)
    : _name(name)
{

}

Library *Design::library() const
{
    return _library;
}

void Design::setLibrary(Library *newLibrary)
{
    _library = newLibrary;
}

const std::string &Design::name() const
{
    return _name;
}

void Design::setName(const std::string &newName)
{
    _name = newName;
}

const std::vector<std::string> &Design::filelists() const
{
    return _filelists;
}

void Design::addFilelist(const std::string &file)
{
    if (!file.empty())
        _filelists.emplace_back(file);
}

void Design::addFilelist(const std::vector<std::string> &files)
{
    for (auto && file : files) {
        if (!file.empty())
            _filelists.emplace_back(file);
    }
}

void Design::setFilelists(const std::vector<std::string> &newFilelists)
{
    _filelists = newFilelists;
}

const std::vector<std::string> &Design::systemverilogFiles() const
{
    return _systemverilogFiles;
}

void Design::addSystemVerilogFile(const std::string &file)
{
    if (!file.empty())
        _systemverilogFiles.emplace_back(file);
}

void Design::addSystemVerilogFile(const std::vector<std::string> &files)
{
    for (auto &&file : files) {
        if (!file.empty())
        _systemverilogFiles.emplace_back(file);
    }
}

void Design::setSystemverilogFiles(const std::vector<std::string> &newSystemverilogFiles)
{
    _systemverilogFiles = newSystemverilogFiles;
}

const std::vector<std::string> &Design::verilogFiles() const
{
    return _verilogFiles;
}

void Design::addVerilogFile(const std::vector<std::string> &files)
{
    for (auto &&file : files) {
        if (!file.empty())
            _verilogFiles.emplace_back(file);
    }
}

void Design::addVerilogFile(const std::string &file)
{
    if (!file.empty())
        _verilogFiles.emplace_back(file);
}

void Design::setVerilogFiles(const std::vector<std::string> &newVerilogFiles)
{
    _verilogFiles = newVerilogFiles;
}

const std::vector<std::string> &Design::vhdlFiles() const
{
    return _vhdlFiles;
}

void Design::setVhdlFiles(const std::vector<std::string> &newVhdlFiles)
{
    _vhdlFiles = newVhdlFiles;
}

// nlohmann::json Design::toJson() const
// {
//     json j;
//     j["name"] = _name;
//     j["top"] = _topModuleName;

//     for (const auto &str : _filelists) {
//         j["source"]["filelist"].emplace_back(str);
//     }

//     for (const auto &str : _verilogFiles) {
//         j["source"]["verilog"].emplace_back(str);
//     }

//     for (const auto &str : _systemverilogFiles) {
//         j["source"]["systemverilog"].emplace_back(str);
//     }
//     return j;
// }

// void Design::fromJson(const nlohmann::json &j)
// {
//     if (j.at("name").is_null() || j.at("name").empty()) {
//         throw std::invalid_argument("Design name is empty");
//     }
//     _name = j.at("name").get<std::string>();
//     _topModuleName = j.at("top").get<std::string>();
//     _filelists = j.at("source").at("filelist").get<std::vector<std::string>>();
//     _verilogFiles = j.at("source").at("verilog").get<std::vector<std::string>>();
//     _systemverilogFiles = j.at("source").at("systemverilog").get<std::vector<std::string>>();
// }

const std::string &Design::topModuleName() const
{
    return _topModuleName;
}

void Design::setTopModuleName(const std::string &newTopModuleName)
{
    _topModuleName = newTopModuleName;
}


} // namespace DataBase
