#ifndef MAP_H
#define MAP_H
#include <vector>
#include <string>
#include <set>

namespace Map {
    extern std::vector<std::pair<Yosys::RTLIL::SigBit, Yosys::RTLIL::SigBit>> g_matched_pi;
    extern std::set<std::string> g_unmap_golden_pi;
    extern std::set<std::string> g_unmap_revised_pi;

    extern std::vector<std::pair<Yosys::RTLIL::SigBit, Yosys::RTLIL::SigBit>> g_matched_po;
    extern std::set<std::string> g_unmap_golden_po;
    extern std::set<std::string> g_unmap_revised_po;

    extern std::vector<std::pair<Yosys::RTLIL::Cell*, Yosys::RTLIL::Cell*>> g_matched_dff;
    extern std::set<std::string> g_unmap_golden_dff;
    extern std::set<std::string> g_unmap_revised_dff;

    extern std::vector<std::pair<Yosys::RTLIL::Cell*, Yosys::RTLIL::Cell*>> g_matched_bbox;
    extern std::set<std::string> g_unmap_golden_bbox;
    extern std::set<std::string> g_unmap_revised_bbox;
};

#endif
