#include "rtlil_util.h"

std::string RTLILUtil::signal_name(const Yosys::RTLIL::SigSpec &sig, bool autoint)
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

void RTLILUtil::dump_const(std::ostream &f, const Yosys::RTLIL::Const &data, int width, int offset, bool autoint)
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


void RTLILUtil::dump_sigchunk(std::ostream &f, const Yosys::RTLIL::SigChunk &chunk, bool autoint)
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

