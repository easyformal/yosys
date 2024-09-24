#include "database_util.h"
#include "kernel/yosys.h"

void DatabaseUtil::print_stat(DataBase::Library *library)
{
        if (library == nullptr) return ;
        std::string name = library->name();
       Yosys:: log( "=========================================\n") ;
       Yosys:: log( "library : %s\n",  name.c_str()) ;

        std::string top_module = library->topModule() == nullptr ? "nullptr" : library->topModule()->name();
       Yosys:: log( "top module : %s\n",  top_module.c_str()) ;

        for (auto &&mod : library->modules()) {
            Yosys:: log( "module : %s\n",  mod.first.c_str()) ;
            
            for (auto &&port : mod.second->portbus()) {
                Yosys:: log( "  portbus : %s\n",  port.first.c_str()) ;
            }
            for (auto &&net : mod.second->netbus()) {
                Yosys:: log( "  netbus : %s\n",  net.first.c_str()) ;
            }
            
            for (auto &&port : mod.second->ports()) {
                if (port.second->bus()) continue;
                Yosys:: log( "  port : %s\n",  port.first.c_str()) ;
            }
            for (auto &&net : mod.second->nets()) {
                if (net.second->bus()) continue;                
                Yosys:: log( "  net : %s\n",  net.first.c_str()) ;
            }
            for (auto &&inst : mod.second->instances()) {
                Yosys:: log( "  inst : %s\n",  inst.first.c_str()) ;
            }
        }
       Yosys:: log( "=========================================\n") ;
}