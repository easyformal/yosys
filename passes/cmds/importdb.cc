#include "kernel/yosys.h"

USING_YOSYS_NAMESPACE
PRIVATE_NAMESPACE_BEGIN

struct ImportDBPass : public Pass {
	ImportDBPass() : Pass("import_db", "import") { }
	void help() override
	{

    }
	void execute(std::vector<std::string> args, RTLIL::Design *design) override
	{
		log_header(design, "Executing import_db pass (managing design hierarchy).\n");

        log_pop();
    }
}ImportDBPass;
    
PRIVATE_NAMESPACE_END
