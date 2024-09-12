#include "yosysdb.h"
#include <unordered_map>

#include <kernel/yosys.h>
#include <kernel/rtlil.h>
#include <kernel/sigtools.h>
#include "module.h"
#include "instance.h"
#include "port.h"
#include "net.h"
#include "instport.h"
#include <plog/Log.h>

YosysImporter::YosysImporter()
{

}

DataBase::Library *YosysImporter::import(Yosys::RTLIL::Design *design, const char *library_name)
{
    if (design == nullptr || library_name == nullptr) return nullptr;
    DataBase::Library *work_lib = new DataBase::Library(library_name);
    PLOGI << "import yosys design";

    for (const auto &module : design->modules().to_vector()) {
        importModule(module, work_lib);
    }
    return work_lib;
}

void YosysImporter::importModule(Yosys::RTLIL::Module *yosys_module, DataBase::Library *library)
{

    hashlib::dict<Yosys::RTLIL::SigBit, std::vector<Yosys::RTLIL::SigBit> > load_map;
    hashlib::dict<Yosys::RTLIL::SigBit, Yosys::RTLIL::SigBit> driver_map;

    std::string module_name = Yosys::RTLIL::unescape_id(yosys_module->name);
    PLOGI << "module : " << module_name;

    Yosys::SigMap active_sigmap;
    active_sigmap.set(yosys_module);
    DataBase::Module *new_mod = new DataBase::Module(module_name);

    // instance
    for (auto &cell : yosys_module->cells().to_vector())
    {
        importCell(cell, new_mod, active_sigmap);
    }

    for (auto &wire : yosys_module->wires().to_vector())
    {
        importWire(wire, new_mod, active_sigmap, load_map, driver_map);
    }

    for (auto it = yosys_module->connections().begin(); it != yosys_module->connections().end(); ++it)
    {
        importConn(it->first, it->second, new_mod, active_sigmap);
    }

    // instance
    for (auto cell : yosys_module->cells().to_vector())
    {
        PLOGI << "instance : " << cell->name.c_str();

        DataBase::Instance *new_inst = new DataBase::Instance(cell->name.c_str(), new_mod);
        new_mod->addInstance(new_inst);
    }

    active_sigmap.clear();
    library->add(new_mod);
}

void YosysImporter::importCell(Yosys::RTLIL::Cell *cell, DataBase::Module *module, const Yosys::SigMap &sigmap)
{
    std::string cell_name = Yosys::RTLIL::unescape_id(cell->name);
    PLOGI << "instance : " << cell_name;

    DataBase::Instance *new_inst = new DataBase::Instance(cell_name, module);
    module->addInstance(new_inst);
}

void YosysImporter::importWire(Yosys::RTLIL::Wire *wire, DataBase::Module *module, const Yosys::SigMap &sigmap, hashlib::dict<Yosys::RTLIL::SigBit, std::vector<Yosys::RTLIL::SigBit> > &load_map, hashlib::dict<Yosys::RTLIL::SigBit, Yosys::RTLIL::SigBit> &driver_map)
{
    if (wire->port_id == 0) {
        importNet(wire, module, sigmap, load_map, driver_map);
    }
    else {
        importPort(wire, module, sigmap, load_map, driver_map);
    }
}

void YosysImporter::importNet(Yosys::RTLIL::Wire *wire, DataBase::Module *module, const Yosys::SigMap &sigmap, hashlib::dict<Yosys::RTLIL::SigBit, std::vector<Yosys::RTLIL::SigBit> > &load_map, hashlib::dict<Yosys::RTLIL::SigBit, Yosys::RTLIL::SigBit> &driver_map)
{
    std::string wire_name = Yosys::RTLIL::unescape_id(wire->name);
    int is_signed = wire->is_signed;

    if (wire->width <= 1) {
        PLOGI << "add net : " << wire_name;
        DataBase::Net *new_net = new DataBase::Net(wire_name, module);
        new_net->setIsSigned(is_signed);
        module->add(new_net);
    }
    else {
        PLOGI << "add netbus:" << wire_name;

        DataBase::NetBus *new_netbus = new DataBase::NetBus(wire_name, module);
        Yosys::RTLIL::SigSpec sigspec = sigmap(wire);
        for (int i = 0; i < sigspec.size(); ++i) {
            Yosys::RTLIL::SigBit sigbit = sigspec[i];
            assert(sigbit.wire);
            std::string name = signal_name(sigbit);
            DataBase::Net *new_net = new DataBase::Net(name, module);
            new_net->setIsSigned(is_signed);
            module->add(new_net);
            new_netbus->add(new_net);
            PLOGI << "  net:" << name;
        }
        module->add(new_netbus);
    }
}

void YosysImporter::importPort(Yosys::RTLIL::Wire *wire, DataBase::Module *module, const Yosys::SigMap &sigmap, hashlib::dict<Yosys::RTLIL::SigBit, std::vector<Yosys::RTLIL::SigBit> > &load_map, hashlib::dict<Yosys::RTLIL::SigBit, Yosys::RTLIL::SigBit> &driver_map)
{
    std::string wire_name = Yosys::RTLIL::unescape_id(wire->name);
    // port/portbus
    DataBase::Direction dir;
    if (wire->port_input && wire->port_output) {
        dir = DataBase::Direction::Inout;
    }
    else if (wire->port_input) {
        dir = DataBase::Direction::Input;
    }
    else if (wire->port_output) {
        dir = DataBase::Direction::Output;
    }
    else {
        dir = DataBase::Direction::Unknown;
    }
    int port_id = wire->port_id;
    int is_signed = wire->is_signed;

    if (wire->width <= 1) {
        PLOGI << "add port:" << wire_name;
        DataBase::Port *new_port = new DataBase::Port(wire_name, dir, module);
        new_port->setIsSigned(is_signed);
        new_port->setId(port_id);
        module->add(new_port);


        auto it = wire->module->connections_;
        //        if (it == cell->connections_.end()) {

        //        }
    }
    else {
        PLOGI << "add portbus:" << wire_name;

        DataBase::PortBus *new_portbus = new DataBase::PortBus(wire_name, dir, module);
        Yosys::RTLIL::SigSpec sigspec = sigmap(wire);
        for (int i = 0; i < sigspec.size(); ++i) {
            Yosys::RTLIL::SigBit sigbit = sigspec[i];
            assert(sigbit.wire);
            std::string name = signal_name(sigbit);
            DataBase::Port *new_port = new DataBase::Port(name, dir, module);
            new_port->setIsSigned(is_signed);
            new_port->setId(port_id);
            module->add(new_port);
            new_portbus->add(new_port);
            PLOGI << "  port:" << name;
        }
        module->add(new_portbus);
    }
}

void YosysImporter::importConn(const Yosys::RTLIL::SigSpec &left, const Yosys::RTLIL::SigSpec &right,
                               DataBase::Module *module, const Yosys::SigMap &sigmap)
{

}

void YosysImporter::initial_load_driver(Yosys::RTLIL::Module *yosys_module, hashlib::dict<Yosys::RTLIL::SigBit, std::vector<Yosys::RTLIL::SigBit> > &load_map, hashlib::dict<Yosys::RTLIL::SigBit, Yosys::RTLIL::SigBit> &driver_map)
{
    for (auto &it : yosys_module->connections_) {
        for (int i = 0; i < Yosys::GetSize(it.second); i++) {
            driver_map[it.second] = it.first;
            load_map[it.first[i]].emplace_back(it.second[i]);
        }
    }
}

std::string YosysImporter::signal_name(const Yosys::RTLIL::SigSpec &sig, bool autoint)
{
    std::stringstream buf;

    if (sig.is_chunk()) {
        dump_sigchunk(buf, sig.as_chunk(), autoint);
    } else {
        buf << Yosys::stringf("{ ");
        for (auto it = sig.chunks().rbegin(); it != sig.chunks().rend(); ++it) {
            dump_sigchunk(buf, *it, false);
            buf << Yosys::stringf(" ");
        }
        buf << Yosys::stringf("}");
    }
    return buf.str();
}

void YosysImporter::dump_const(std::ostream &f, const Yosys::RTLIL::Const &data, int width, int offset, bool autoint)
{
    if (width < 0)
        width = data.bits.size() - offset;
    if ((data.flags & Yosys::RTLIL::CONST_FLAG_STRING) == 0 || width != (int)data.bits.size()) {
        if (width == 32 && autoint) {
            int32_t val = 0;
            for (int i = 0; i < width; i++) {
                log_assert(offset+i < (int)data.bits.size());
                switch (data.bits[offset+i]) {
                case Yosys::State::S0: break;
                case Yosys::State::S1: val |= 1 << i; break;
                default: val = -1; break;
                }
            }
            if (val >= 0) {
                f << Yosys::stringf("%d", val);
                return;
            }
        }
        f << Yosys::stringf("%d'", width);
        if (data.is_fully_undef_x_only()) {
            f << "x";
        } else {
            for (int i = offset+width-1; i >= offset; i--) {
                log_assert(i < (int)data.bits.size());
                switch (data.bits[i]) {
                case Yosys::State::S0: f << Yosys::stringf("0"); break;
                case Yosys::State::S1: f << Yosys::stringf("1"); break;
                case Yosys::RTLIL::Sx: f << Yosys::stringf("x"); break;
                case Yosys::RTLIL::Sz: f << Yosys::stringf("z"); break;
                case Yosys::RTLIL::Sa: f << Yosys::stringf("-"); break;
                case Yosys::RTLIL::Sm: f << Yosys::stringf("m"); break;
                }
            }
        }
    } else {
        f << Yosys::stringf("\"");
        std::string str = data.decode_string();
        for (size_t i = 0; i < str.size(); i++) {
            if (str[i] == '\n')
                f << Yosys::stringf("\\n");
            else if (str[i] == '\t')
                f << Yosys::stringf("\\t");
            else if (str[i] < 32)
                f << Yosys::stringf("\\%03o", (unsigned char)str[i]);
            else if (str[i] == '"')
                f << Yosys::stringf("\\\"");
            else if (str[i] == '\\')
                f << Yosys::stringf("\\\\");
            else
                f << str[i];
        }
        f << Yosys::stringf("\"");
    }
}


void YosysImporter::dump_sigchunk(std::ostream &f, const Yosys::RTLIL::SigChunk &chunk, bool autoint)
{
    if (chunk.wire == NULL) {
        dump_const(f, chunk.data, chunk.width, chunk.offset, autoint);
    } else {
        if (chunk.width == chunk.wire->width && chunk.offset == 0)
            f << Yosys::stringf("%s", chunk.wire->name.c_str());
        else if (chunk.width == 1)
            f << Yosys::stringf("%s [%d]", chunk.wire->name.c_str(), chunk.offset);
        else
            f << Yosys::stringf("%s [%d:%d]", chunk.wire->name.c_str(), chunk.offset+chunk.width-1, chunk.offset);
    }
}



















