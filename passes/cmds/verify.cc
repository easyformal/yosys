#include "kernel/yosys.h"
#include "kernel/rtlil_util.h"
#include "map.h"
#include "verify.h"
#include "kernel/abcutil.h"

using namespace std;
using namespace abc;

USING_YOSYS_NAMESPACE
PRIVATE_NAMESPACE_BEGIN


struct VerifyPass : public Pass {
	VerifyPass() : Pass("verify", "verify") { }
	void help() override
	{
		log("\n");
		log("    verify -golden <name>\n");
		log("\n");
		log("\n");
		log("    verify -revised <name>\n");
		log("\n");        
    }
	void execute(std::vector<std::string> args, RTLIL::Design *design) override
	{
		log_header(design, "Verify compare point.\n");

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
        run_verify(golden_design, revised_design);
        log_pop();
    }
    void run_verify_po(ComparePoint &point) 
    {
        

    }
    void run_verify_dff(ComparePoint &point) 
    {

    }
    void run_verify_bbox(ComparePoint &point) 
    {

    }
    void run_verify(RTLIL::Design *golden_design, RTLIL::Design *revised_design)
    {
        RTLIL::Module * golden_top = golden_design->top_module();
        RTLIL::Module * revised_top = revised_design->top_module();

        if (!golden_top ) {
			log_cmd_error("No golden top module.\n");
        }
        if (!revised_top) {
			log_cmd_error("No revised top module.\n");
        }        
        std::vector<ComparePoint>  all_compare_point = get_compare_point();

        log("Compare point size : %d", all_compare_point.size());

        for (auto &&point : all_compare_point) {
            switch (point.type)
            {
            case ComparePointType::PO:
                run_verify_po(point);
                break;
            case ComparePointType::DFF:
                run_verify_dff(point);
                break;
            case ComparePointType::BBox:
                run_verify_bbox(point);
                break;
            default:
                break;
            }
        }
        test_abc();
    }

    std::vector<ComparePoint> get_compare_point()
    {
        std::vector<ComparePoint> result;
        // PO
        for (auto &&pair : Map::g_matched_po) {
            ComparePoint point;
            point.golden_po = pair.first;
            point.revised_po = pair.second;
            point.type = ComparePointType::PO;
            result.emplace_back(point);
        }

        // DFF
        for (auto &&pair : Map::g_matched_dff) {
            ComparePoint point;
            point.golden_inst = pair.first;
            point.revised_inst = pair.second;
            point.type = ComparePointType::DFF;
            result.emplace_back(point);
        }
        // BBox
        for (auto &&pair : Map::g_matched_bbox) {
            ComparePoint point;
            point.golden_inst = pair.first;
            point.revised_inst = pair.second;
            point.type = ComparePointType::BBox;
            result.emplace_back(point);
        }
        return result;
    }

    void test_abc()
    {
        Abc_Ntk_t * gold_aig = Abc_NtkAlloc(ABC_NTK_STRASH, ABC_FUNC_AIG, 1);
        Abc_Ntk_t * gate_aig = Abc_NtkAlloc(ABC_NTK_STRASH, ABC_FUNC_AIG, 1);

        gold_aig->pName = Extra_UtilStrsav("golden");
        gate_aig->pName = Extra_UtilStrsav("Revised");

        // Abc_Obj_t * gold_pLatch, * gold_pLatchInput, * gold_pLatchOutput; 
        // gold_pLatchInput = Abc_NtkCreateBi( gold_aig );
        // gold_pLatchOutput = Abc_NtkCreateBo( gold_aig );
        // Abc_ObjAddFanin( gold_pLatch, gold_pLatchInput );
        // Abc_ObjAddFanin( gold_pLatchOutput, gold_pLatch );         

        // Abc_Obj_t * gate_pLatch, * gate_pLatchInput, * gate_pLatchOutput; 
        // gate_pLatchInput = Abc_NtkCreateBi( gate_aig );        
        // gate_pLatchOutput = Abc_NtkCreateBo( gate_aig );        
        // Abc_ObjAddFanin( gate_pLatch, gate_pLatchInput );
        // Abc_ObjAddFanin( gate_pLatchOutput, gate_pLatch );         
        

        Abc_Obj_t * pPo; 
        Abc_Obj_t * pObjA, *pObjB, *pObjC; 
        pObjA = Abc_NtkCreatePi( gold_aig ); 
        pObjB = Abc_NtkCreatePi( gold_aig ); 
        pObjC = Abc_NtkCreatePi( gold_aig ); 
        pPo = Abc_NtkCreatePo( gold_aig ); 

        Abc_ObjAssignName( pObjA, "A", NULL ); 
        Abc_ObjAssignName( pObjB, "B", NULL ); 
        Abc_ObjAssignName( pObjC, "C", NULL ); 
        Abc_ObjAssignName( pPo, "O", NULL ); 

        Abc_Obj_t *pObjAnd, * pObjF ;

        pObjAnd = Abc_AigAnd( (Abc_Aig_t *)gold_aig->pManFunc, pObjA, pObjB );
        pObjF = Abc_AigAnd( (Abc_Aig_t *)gold_aig->pManFunc, pObjAnd, pObjC );
        
        Abc_ObjAddFanin( pPo, pObjF ); 

        pObjA = Abc_NtkFindNode( gold_aig, "A"); 
        pObjB = Abc_NtkFindNode( gold_aig , "B"); 
        pObjC = Abc_NtkFindNode( gold_aig , "C"); 
        pPo = Abc_NtkCreatePo( gate_aig ); 

        Abc_ObjAssignName( pPo, "O", NULL ); 

        pObjAnd = Abc_AigAnd( (Abc_Aig_t *)gate_aig->pManFunc, pObjA, pObjB );
        pObjF = Abc_AigAnd( (Abc_Aig_t *)gate_aig->pManFunc, pObjAnd, pObjC );
        Abc_ObjAddFanin( pPo, pObjF ); 
        
        Abc_Ntk_t *miter = Abc_NtkMiter(gold_aig, gate_aig, 0, 0, 0, 0);
        if ( miter == NULL )
        {
            log_error("miter is null");
        }
        int RetValue = 0;
        
        RetValue = Abc_NtkMiterSat(miter,1000000, 0, 0, NULL, NULL );

        if ( RetValue == -1 )
            log( "UNDECIDED" );
        else if ( RetValue == 0 )
            log("SATISFIABLE" );
        else
            log("UNSATISFIABLE" );
    }
}VerifyPass;

PRIVATE_NAMESPACE_END
