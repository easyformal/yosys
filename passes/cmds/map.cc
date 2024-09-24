#include "kernel/yosys.h"
#include "kernel/sigtools.h"
#include "kernel/rtlil_util.h"
#include "map.h"

namespace Map {
    std::vector<std::pair<Yosys::RTLIL::SigBit, Yosys::RTLIL::SigBit>> g_matched_pi;
    std::set<std::string> g_unmap_golden_pi;
    std::set<std::string> g_unmap_revised_pi;

    std::vector<std::pair<Yosys::RTLIL::SigBit, Yosys::RTLIL::SigBit>> g_matched_po;
    std::set<std::string> g_unmap_golden_po;
    std::set<std::string> g_unmap_revised_po;

    std::vector<std::pair<Yosys::RTLIL::Cell*, Yosys::RTLIL::Cell*>> g_matched_dff;
    std::set<std::string> g_unmap_golden_dff;
    std::set<std::string> g_unmap_revised_dff;

    std::vector<std::pair<Yosys::RTLIL::Cell*, Yosys::RTLIL::Cell*>> g_matched_bbox;
    std::set<std::string> g_unmap_golden_bbox;
    std::set<std::string> g_unmap_revised_bbox;
};

using namespace std;

USING_YOSYS_NAMESPACE
PRIVATE_NAMESPACE_BEGIN


struct MapPass : public Pass {
	MapPass() : Pass("map", "map") { }

        SigMap golden_sigmap;
        SigMap revised_sigmap;

	void help() override
	{
		log("\n");
		log("    map -golden <name>\n");
		log("\n");
		log("\n");
		log("    map -revised <name>\n");
		log("\n");        
    }
	void execute(std::vector<std::string> args, RTLIL::Design *design) override
	{
		log_header(design, "Map compare point.\n");

		size_t argidx;
        std::string golden_name;
        std::string revised_name;
		for (argidx = 1; argidx < args.size(); argidx++)
		{
			std::string arg = args[argidx];
			if (args[argidx] == "-golden" && argidx+1 < args.size()) {
				golden_name = args[++argidx];
				continue;
			}
			if (args[argidx] == "-revised" && argidx+1 < args.size()) {
				revised_name = args[++argidx];
				continue;
			}
        }

        RTLIL::Design *golden_design = saved_designs[golden_name];
        RTLIL::Design *revised_design = saved_designs[revised_name];

        if (!golden_design ) {
			log_cmd_error("No golden design.\n");
        }
        if (!revised_design) {
			log_cmd_error("No revised design.\n");
        }

        reset_data();
        run_map(golden_design, revised_design);

        log_pop();
    }
    void reset_data()
    {
        Map::g_matched_pi.clear();
        Map::g_unmap_golden_pi.clear();
        Map::g_unmap_revised_pi.clear();

        Map::g_matched_po.clear();
        Map::g_unmap_golden_po.clear();
        Map::g_unmap_revised_po.clear();

        Map::g_matched_dff.clear();
        Map::g_unmap_golden_dff.clear();
        Map::g_unmap_revised_dff.clear();

        Map::g_matched_bbox.clear();
        Map::g_unmap_golden_bbox.clear();
        Map::g_unmap_revised_bbox.clear();
    }

    void get_match_port(RTLIL::Module *top,  SigMap &sigmap, std::unordered_map<std::string, SigBit> &golden_pi, 
    std::unordered_map<std::string, SigBit> &golden_po, std::vector<std::string> &golden_pi_vec, std::vector<std::string> &golden_po_vec)
    {
        for (auto &wire : top->wires().to_vector()) {
                if (wire->port_input && !wire->port_output) {
                    Yosys::RTLIL::SigSpec sigspec = sigmap(wire);
                    for (int i = 0; i < sigspec.size(); ++i) {        
                        Yosys::RTLIL::SigBit sigbit = sigspec[i];
                        std::string name = RTLILUtil::signal_name(sigbit);
                        golden_pi[name] = sigbit;
                        golden_pi_vec.emplace_back(name);
                    }            
                }
                else if (!wire->port_input && wire->port_output) {
                    Yosys::RTLIL::SigSpec sigspec = sigmap(wire);
                    for (int i = 0; i < sigspec.size(); ++i) {        
                        Yosys::RTLIL::SigBit sigbit = sigspec[i];
                        std::string name = RTLILUtil::signal_name(sigbit);
                        golden_po[name] = sigbit;
                        golden_po_vec.emplace_back(name);
                    }          
                }
        }        
    }
    void get_match_cell(RTLIL::Module *top, std::unordered_map<std::string, Cell*> &dff, 
    std::unordered_map<std::string, Cell*> &bbox, std::vector<std::string> &dff_vec, 
    std::vector<std::string> &bbox_vec)
    {
        for (auto &cell : top->cells().to_vector())
        {
            if (cell->get_blackbox_attribute()) {
                bbox[cell->name.c_str()] = cell;
                bbox_vec.emplace_back(cell->name.c_str());
            }
		    else if (cell->type.in(ID($ff), ID($dff), ID($adff), ID($dffsr), ID($aldff), ID($dlatch), ID($dlatchsr), ID($_DFFSR_PPP_), ID($_DLATCHSR_PPP_))) {
                dff[cell->name.c_str()] = cell;
                dff_vec.emplace_back(cell->name.c_str());
            }
        }
    }
    void run_map(RTLIL::Design *golden_design, RTLIL::Design *revised_design)
    {
        RTLIL::Module * golden_top = golden_design->top_module();
        RTLIL::Module * revised_top = revised_design->top_module();

        if (!golden_top ) {
			log_cmd_error("No golden top module.\n");
        }
        if (!revised_top) {
			log_cmd_error("No revised top module.\n");
        }        


        std::unordered_map<std::string, SigBit> golden_pi;
        std::unordered_map<std::string, SigBit> golden_po;
        std::unordered_map<std::string, Cell*> golden_dff;
        std::unordered_map<std::string, Cell*> golden_bbox;

        std::vector<std::string> golden_pi_vec;
        std::vector<std::string> golden_po_vec;
        std::vector<std::string>  golden_dff_vec;
        std::vector<std::string>  golden_bbox_vec;

         std::unordered_map<std::string, SigBit> revised_pi;
        std::unordered_map<std::string, SigBit> revised_po;
        std::unordered_map<std::string, Cell*> revised_dff;
        std::unordered_map<std::string, Cell*> revised_bbox;

        std::vector<std::string> revised_pi_vec;
        std::vector<std::string> revised_po_vec;
        std::vector<std::string>  revised_dff_vec;
        std::vector<std::string>  revised_bbox_vec;
        

        golden_sigmap.set(golden_top);
        get_match_port(golden_top, golden_sigmap, golden_pi, golden_po, golden_pi_vec, golden_po_vec);
        get_match_cell(golden_top, golden_dff, golden_bbox, golden_dff_vec, golden_bbox_vec);


        revised_sigmap.set(revised_top);
        get_match_port(revised_top, revised_sigmap, revised_pi, revised_po, revised_pi_vec, revised_po_vec);
        get_match_cell(revised_top, revised_dff, revised_bbox, revised_dff_vec, revised_bbox_vec);

        matching_with_name(golden_pi, golden_pi_vec, revised_pi_vec, Map::g_matched_pi, Map::g_unmap_golden_pi, Map::g_unmap_revised_pi);
        matching_with_regex(golden_pi, golden_pi_vec, revised_pi_vec, vector<regex>{}, vector<regex>{}, Map::g_matched_pi, Map::g_unmap_golden_pi, Map::g_unmap_revised_pi);

        matching_with_name(golden_po, golden_po_vec, revised_po_vec, Map::g_matched_po, Map::g_unmap_golden_po, Map::g_unmap_revised_po);
        matching_with_regex(golden_po, golden_po_vec, revised_po_vec, vector<regex>{}, vector<regex>{}, Map::g_matched_po, Map::g_unmap_golden_po, Map::g_unmap_revised_po);

        matching_with_name(golden_dff, golden_dff_vec, revised_dff_vec, Map::g_matched_dff, Map::g_unmap_golden_dff, Map::g_unmap_revised_dff);
        matching_with_regex(golden_dff, golden_dff_vec, revised_dff_vec, vector<regex>{}, vector<regex>{}, Map::g_matched_dff, Map::g_unmap_golden_dff, Map::g_unmap_revised_dff);

        matching_with_name(golden_bbox, golden_bbox_vec, revised_bbox_vec, Map::g_matched_bbox, Map::g_unmap_golden_bbox, Map::g_unmap_revised_bbox);
        matching_with_regex(golden_bbox, golden_bbox_vec, revised_bbox_vec, vector<regex>{}, vector<regex>{}, Map::g_matched_bbox, Map::g_unmap_golden_bbox, Map::g_unmap_revised_bbox);
    
        // print log
        print_log();
    }

    void print_log()
    {
        log("\n");
        log("Map result: \n");
        log("\n");
        log("PI matched: \n");
        for (auto &&pair : Map::g_matched_pi) {
            log("  %s - %s \n", RTLILUtil::signal_name(pair.first).c_str(), RTLILUtil::signal_name(pair.second).c_str());
        }

        if (Map::g_unmap_golden_pi.size() > 0) {
            log("\n");
            log("PI golden unmatched: \n");
            for (auto &&pair : Map::g_unmap_golden_pi) {
                log("  %s  \n", pair.c_str());
            }            
        }
        if (Map::g_unmap_revised_pi.size() > 0) {
            log("\n");
            log("PI revised unmatched: \n");
            for (auto &&pair : Map::g_unmap_revised_pi) {
                log("  %s  \n", pair.c_str());
            }
        }
        
        log("\n");
        log("PO matched: \n");
        for (auto &&pair : Map::g_matched_po) {
            log("  %s - %s \n", RTLILUtil::signal_name(pair.first).c_str(), RTLILUtil::signal_name(pair.second).c_str());
        }

        if (Map::g_unmap_golden_po.size() > 0) {
            log("\n");
            log("PO golden unmatched: \n");
            for (auto &&pair : Map::g_unmap_golden_po) {
                log("  %s  \n", pair.c_str());
            }
        }
        if (Map::g_unmap_revised_po.size() > 0) {
            log("\n");
            log("PO revised unmatched: \n");
            for (auto &&pair : Map::g_unmap_revised_po) {
                log("  %s  \n", pair.c_str());
            }
        }        
        log("\n");
        log("DFF matched: \n");
        for (auto &&pair : Map::g_matched_dff) {
            log("  %s - %s \n", pair.first->name.c_str(), pair.second->name.c_str());
        }

        if (Map::g_unmap_golden_dff.size() > 0) {
            log("\n");
            log("DFF golden unmatched: \n");
            for (auto &&pair : Map::g_unmap_golden_dff) {
                log("  %s  \n", pair.c_str());
            }
        }

        if (Map::g_unmap_revised_dff.size() > 0) {        
            log("\n");
            log("DFF revised unmatched: \n");
            for (auto &&pair : Map::g_unmap_revised_dff) {
                log("  %s  \n", pair.c_str());
            }
        }

        log("\n");
        log("BBOX matched: \n");
        for (auto &&pair : Map::g_matched_bbox) {
            log("  %s - %s \n", pair.first->name.c_str(), pair.second->name.c_str());
        }

        if (Map::g_unmap_golden_bbox.size() > 0) {

            log("\n");
            log("BBOX golden unmatched: \n");
            for (auto &&pair : Map::g_unmap_golden_bbox) {
                log("  %s  \n", pair.c_str());
            }
        }
        if (Map::g_unmap_revised_bbox.size() > 0) {     
            log("\n");
            log("BBOX revised unmatched: \n");
            for (auto &&pair : Map::g_unmap_revised_bbox) {
                log("  %s  \n", pair.c_str());
            }
        }
    }

        // 处理字符串，依次应用所有正则表达式，最终返回处理后的字符串
    string process_with_all_regex(const string& name, const vector<regex>& patterns, const string& replacement = "") {
        string processed_name = name;
        for (const auto& pattern : patterns) {
            if (replacement.empty()) {
                smatch match;
                if (regex_search(processed_name, match, pattern)) {
                    processed_name = match.str(0);  // 提取匹配部分，继续处理
                } else {
                    return "";  // 如果有一个正则匹配失败，返回空字符串
                }
            } else {
                processed_name = regex_replace(processed_name, pattern, replacement);
            }
        }
        return processed_name;  // 返回经过所有正则处理后的结果
    }

    void matching_with_name(std::unordered_map<std::string, SigBit>  key_map, vector<string>& list1, vector<string>& list2, 
        vector<pair<SigBit, SigBit>>& matched, set<string>& unmatched1, set<string>& unmatched2) {

        sort(list1.begin(), list1.end());
        sort(list2.begin(), list2.end());
        
        size_t i = 0, j = 0;
        
        // 使用双指针法遍历两个列表
        while (i < list1.size() && j < list2.size()) {
            // 先判断两个处理后的名字是否完全相同
            if (list1[i] == list2[j]) {
                matched.emplace_back(key_map[list1[i]],  key_map[list2[j]]);
                ++i;
                ++j;
            } else if (list1[i] < list2[j]) {
                unmatched1.insert(list1[i]);
                ++i;
            } else {
                unmatched2.insert(list2[j]);
                ++j;
            }
        }
        
        // 添加未匹配完的剩余部分
        while (i < list1.size()) {
            unmatched1.insert(list1[i++]);
        }
        while (j < list2.size()) {
            unmatched2.insert(list2[j++]);
        }
    }
    void matching_with_regex(
        std::unordered_map<std::string, SigBit>  key_map, const vector<string>& list1, const vector<string>& list2, 
        const vector<regex>& patterns1, const vector<regex>& patterns2, vector<pair<SigBit, SigBit>>& matched, 
        set<string>& unmatched1, set<string>& unmatched2) {
        
        // 记录原始字符串和经过正则处理后的字符串
        unordered_map<string, string> original_to_processed_list1;
        unordered_map<string, string> original_to_processed_list2;
        
        // 处理 list1
        for (const auto& str : list1) {
            string processed_str = patterns1.empty() ? str : process_with_all_regex(str, patterns1);
            if (original_to_processed_list1.count(processed_str)) {
                original_to_processed_list1[str] = str;
                log_warning("Compare point '%s' are repeated after applying naming rules.", str.c_str());
            }
            else {
                original_to_processed_list1[processed_str] = str;
            }
        }
        
        // 处理 list2
        for (const auto& str : list2) {
            string processed_str = patterns2.empty() ? str : process_with_all_regex(str, patterns2);
            if (original_to_processed_list2.count(processed_str)) {
                original_to_processed_list2[str] = str;                
                log_warning("Compare point '%s' are repeated after applying naming rules.", str.c_str());
            }
            else {
                original_to_processed_list2[processed_str] = str;
            }
        }
        
        // 将两个处理后的字符串排序
        vector<string> processed_list1;
        vector<string> processed_list2;
        
        for (const auto& [processed, original] : original_to_processed_list1) {
            processed_list1.push_back(processed);
        }
        
        for (const auto& [processed, original] : original_to_processed_list2) {
            processed_list2.push_back(processed);
        }
        
        sort(processed_list1.begin(), processed_list1.end());
        sort(processed_list2.begin(), processed_list2.end());
        
        size_t i = 0, j = 0;
        
        // 使用双指针法遍历两个列表
        while (i < processed_list1.size() && j < processed_list2.size()) {
            // 先判断两个处理后的名字是否完全相同
            if (processed_list1[i] == processed_list2[j]) {
                matched.emplace_back(key_map[original_to_processed_list1[processed_list1[i]]], key_map[original_to_processed_list2[processed_list2[j]]]);
                ++i;
                ++j;
            } else if (processed_list1[i] < processed_list2[j]) {
                unmatched1.insert(original_to_processed_list1[processed_list1[i]]);
                ++i;
            } else {
                unmatched2.insert(original_to_processed_list2[processed_list2[j]]);
                ++j;
            }
        }
        
        // 添加未匹配完的剩余部分
        while (i < processed_list1.size()) {
            unmatched1.insert(original_to_processed_list1[processed_list1[i++]]);
        }
        while (j < processed_list2.size()) {
            unmatched2.insert(original_to_processed_list2[processed_list2[j++]]);
        }
    }

    void matching_with_name(std::unordered_map<std::string, Cell*>  key_map, vector<string>& list1, vector<string>& list2, 
        vector<pair< Cell*,  Cell*>>& matched, set<string>& unmatched1, set<string>& unmatched2) {

        sort(list1.begin(), list1.end());
        sort(list2.begin(), list2.end());
        
        size_t i = 0, j = 0;
        
        // 使用双指针法遍历两个列表
        while (i < list1.size() && j < list2.size()) {
            // 先判断两个处理后的名字是否完全相同
            if (list1[i] == list2[j]) {
                matched.emplace_back(key_map[list1[i]],  key_map[list2[j]]);
                ++i;
                ++j;
            } else if (list1[i] < list2[j]) {
                unmatched1.insert(list1[i]);
                ++i;
            } else {
                unmatched2.insert(list2[j]);
                ++j;
            }
        }
        
        // 添加未匹配完的剩余部分
        while (i < list1.size()) {
            unmatched1.insert(list1[i++]);
        }
        while (j < list2.size()) {
            unmatched2.insert(list2[j++]);
        }
    }
    void matching_with_regex(
        std::unordered_map<std::string,  Cell*>  key_map, const vector<string>& list1, const vector<string>& list2, 
        const vector<regex>& patterns1, const vector<regex>& patterns2, vector<pair< Cell*,  Cell*>>& matched, 
        set<string>& unmatched1, set<string>& unmatched2) {
        
        // 记录原始字符串和经过正则处理后的字符串
        unordered_map<string, string> original_to_processed_list1;
        unordered_map<string, string> original_to_processed_list2;
        
        // 处理 list1
        for (const auto& str : list1) {
            string processed_str = patterns1.empty() ? str : process_with_all_regex(str, patterns1);
            if (original_to_processed_list1.count(processed_str)) {
                original_to_processed_list1[str] = str;
                log_warning("Compare point '%s' are repeated after applying naming rules.", str.c_str());
            }
            else {
                original_to_processed_list1[processed_str] = str;
            }
        }
        
        // 处理 list2
        for (const auto& str : list2) {
            string processed_str = patterns2.empty() ? str : process_with_all_regex(str, patterns2);
            if (original_to_processed_list2.count(processed_str)) {
                original_to_processed_list2[str] = str;                
                log_warning("Compare point '%s' are repeated after applying naming rules.", str.c_str());
            }
            else {
                original_to_processed_list2[processed_str] = str;
            }
        }
        
        // 将两个处理后的字符串排序
        vector<string> processed_list1;
        vector<string> processed_list2;
        
        for (const auto& [processed, original] : original_to_processed_list1) {
            processed_list1.push_back(processed);
        }
        
        for (const auto& [processed, original] : original_to_processed_list2) {
            processed_list2.push_back(processed);
        }
        
        sort(processed_list1.begin(), processed_list1.end());
        sort(processed_list2.begin(), processed_list2.end());
        
        size_t i = 0, j = 0;
        
        // 使用双指针法遍历两个列表
        while (i < processed_list1.size() && j < processed_list2.size()) {
            // 先判断两个处理后的名字是否完全相同
            if (processed_list1[i] == processed_list2[j]) {
                matched.emplace_back(key_map[original_to_processed_list1[processed_list1[i]]], key_map[original_to_processed_list2[processed_list2[j]]]);
                ++i;
                ++j;
            } else if (processed_list1[i] < processed_list2[j]) {
                unmatched1.insert(original_to_processed_list1[processed_list1[i]]);
                ++i;
            } else {
                unmatched2.insert(original_to_processed_list2[processed_list2[j]]);
                ++j;
            }
        }
        
        // 添加未匹配完的剩余部分
        while (i < processed_list1.size()) {
            unmatched1.insert(original_to_processed_list1[processed_list1[i++]]);
        }
        while (j < processed_list2.size()) {
            unmatched2.insert(original_to_processed_list2[processed_list2[j++]]);
        }
    }
}MapPass;
    
PRIVATE_NAMESPACE_END
