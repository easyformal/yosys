#ifndef YOSYSDB_H
#define YOSYSDB_H

#include "library.h"
#include "net.h"
#include <kernel/rtlil.h>
namespace Yosys {
    class SigMap;
    namespace RTLIL {
        struct Design;
        struct Module;
        struct Cell;
        struct Wire;
        struct SigSpec;
        struct SigBit;
        struct SigChunk;
        struct Const;
    }
}
#include <kernel/hashlib.h>

class YosysImporter
{
public:
    YosysImporter();
    DataBase::Library *import(Yosys::RTLIL::Design *design, const char *library_name = "work");
    void importModule(Yosys::RTLIL::Module *yosys_module, DataBase::Library *library);
    void importCell(Yosys::RTLIL::Cell *cell, DataBase::Module *module, const Yosys::SigMap &sigmap);
    void importWire(Yosys::RTLIL::Wire *wire, DataBase::Module *module, const Yosys::SigMap &sigmap, hashlib::dict<Yosys::RTLIL::SigBit, std::vector<Yosys::RTLIL::SigBit> > &load_map, hashlib::dict<Yosys::RTLIL::SigBit, Yosys::RTLIL::SigBit> &driver_map);
    void importNet(Yosys::RTLIL::Wire *wire, DataBase::Module *module, const Yosys::SigMap &sigmap, hashlib::dict<Yosys::RTLIL::SigBit, std::vector<Yosys::RTLIL::SigBit> > &load_map, hashlib::dict<Yosys::RTLIL::SigBit, Yosys::RTLIL::SigBit> &driver_map);
    void importPort(Yosys::RTLIL::Wire *wire, DataBase::Module *module, const Yosys::SigMap &sigmap, hashlib::dict<Yosys::RTLIL::SigBit, std::vector<Yosys::RTLIL::SigBit> > &load_map, hashlib::dict<Yosys::RTLIL::SigBit, Yosys::RTLIL::SigBit> &driver_map);
    void importConn(const Yosys::RTLIL::SigSpec &left, const Yosys::RTLIL::SigSpec &right, DataBase::Module *module, const Yosys::SigMap &sigmap);
    void initial_load_driver(Yosys::RTLIL::Module *yosys_module, hashlib::dict<Yosys::RTLIL::SigBit, std::vector<Yosys::RTLIL::SigBit> > &load_map, hashlib::dict<Yosys::RTLIL::SigBit, Yosys::RTLIL::SigBit> &driver_map);

    std::string signal_name(const Yosys::RTLIL::SigSpec &sig, bool autoint = true);
    void dump_const(std::ostream &f, const Yosys::RTLIL::Const &data, int width = -1, int offset = 0, bool autoint = true);
    void dump_sigchunk(std::ostream &f, const Yosys::RTLIL::SigChunk &chunk, bool autoint = true);

private:
    std::map<Yosys::RTLIL::SigBit, DataBase::Net*> net_map;

};

#endif // YOSYSDB_H
