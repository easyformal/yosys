add_qtc_library(DataBase
  DEPENDS Plog NJson
  PUBLIC_DEPENDS Qt::Core
  SOURCES
  database_global.h
  baseobject.h baseobject.cpp
  net.h net.cpp
  instance.h instance.cpp
  module.h module.cpp
  port.h port.cpp
  instport.h instport.cpp
  library.h library.cpp
  designset.h designset.cpp
  design.h design.cpp
  yosysdb.h yosysdb.cpp
  database.h
  EXPLICIT_MOC
  SKIP_AUTOMOC
)

find_package(Qt6 COMPONENTS Test QUIET)

extend_qtc_library(DataBase
  CONDITION TARGET Qt::Test
  DEPENDS Qt::Test
  DEFINES WITH_TESTS
)
extend_qtc_library(DataBase
  CONDITION WIN32
  DEFINES PLOG_IMPORT
)

extend_qtc_library(DataBase
  CONDITION UNIX
  DEFINES PLOG_GLOBAL
)

extend_qtc_library(DataBase
  DEPENDS Yosys
  DEFINES ${YOSYS_CONFIG_FLAGS}
  INCLUDES ${YOSYS_INCLUDE_DIR}
)

