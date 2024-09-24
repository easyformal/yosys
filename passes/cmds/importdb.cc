#include "kernel/yosys.h"
#include "database/database.h"
#include "database/yosysdb.h"

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

		if (DataBase::DesignSet::instance() == nullptr) {
			DataBase::DesignSet::instance()->init();
		}

		YosysImporter importer;
		DataBase::Library *new_library = importer.import(design);
		
		if (new_library != nullptr) {
			DataBase::Design *new_design = DataBase::DesignSet::instance()->addDesign("default");
			new_design->setLibrary(new_library);
			log("Importing database successfully.");
		}
		else {
			log_warning("Importing database failed.");
		}

        log_pop();
    }
}ImportDBPass;
    
PRIVATE_NAMESPACE_END
