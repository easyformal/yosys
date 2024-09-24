#ifndef RTLILUTIL_H
#define RTLILUTIL_H

#include "kernel/yosys.h"

class RTLILUtil
{
public:
    static  std::string signal_name(const Yosys::RTLIL::SigSpec &sig, bool autoint = true);
    static void dump_const(std::ostream &f, const Yosys::RTLIL::Const &data, int width = -1, int offset = 0, bool autoint = true);
    static void dump_sigchunk(std::ostream &f, const Yosys::RTLIL::SigChunk &chunk, bool autoint = true);

};

#endif