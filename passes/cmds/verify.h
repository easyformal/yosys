#ifndef VERIFY_H
#define VERIFY_H
#include "kernel/yosys.h"

enum ComparePointType{
    PO,
    DFF,
    BBox
};


struct ComparePoint
{
    ComparePointType type;

    Yosys::SigBit golden_po;
    Yosys::SigBit revised_po;

    Yosys::Cell *golden_inst = nullptr;
    Yosys::Cell *revised_inst =  nullptr;
};




#endif