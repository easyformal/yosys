#ifndef ABCUTIL_H
#define ABCUTIL_H

#include "base/abc/abc.h"

namespace abc {
    typedef struct Abc_Ntk_t_       Abc_Ntk_t;

    // network types
    typedef enum { 
        ABC_NTK_NONE = 0,   // 0:  unknown
        ABC_NTK_NETLIST,    // 1:  network with PIs/POs, latches, nodes, and nets
        ABC_NTK_LOGIC,      // 2:  network with PIs/POs, latches, and nodes
        ABC_NTK_STRASH,     // 3:  structurally hashed AIG (two input AND gates with c-attributes on edges)
        ABC_NTK_OTHER       // 4:  unused
    } Abc_NtkType_t;

    // network functionality
    typedef enum { 
        ABC_FUNC_NONE = 0,  // 0:  unknown
        ABC_FUNC_SOP,       // 1:  sum-of-products
        ABC_FUNC_BDD,       // 2:  binary decision diagrams
        ABC_FUNC_AIG,       // 3:  and-inverter graphs
        ABC_FUNC_MAP,       // 4:  standard cell library
        ABC_FUNC_BLIFMV,    // 5:  BLIF-MV node functions
        ABC_FUNC_BLACKBOX,  // 6:  black box about which nothing is known
        ABC_FUNC_OTHER      // 7:  unused
    } Abc_NtkFunc_t;

struct Abc_Ntk_t_ 
{
    // general information 
    Abc_NtkType_t     ntkType;       // type of the network
    Abc_NtkFunc_t     ntkFunc;       // functionality of the network
    char *            pName;         // the network name
    char *            pSpec;         // the name of the spec file if present
    Nm_Man_t *        pManName;      // name manager (stores names of objects)
    // components of the network
    Vec_Ptr_t *       vObjs;         // the array of all objects (net, nodes, latches, etc)
    Vec_Ptr_t *       vPis;          // the array of primary inputs
    Vec_Ptr_t *       vPos;          // the array of primary outputs
    Vec_Ptr_t *       vCis;          // the array of combinational inputs  (PIs, latches)
    Vec_Ptr_t *       vCos;          // the array of combinational outputs (POs, asserts, latches)
    Vec_Ptr_t *       vPios;         // the array of PIOs
    Vec_Ptr_t *       vBoxes;        // the array of boxes
    Vec_Ptr_t *       vLtlProperties;
    // the number of living objects
    int nObjCounts[ABC_OBJ_NUMBER];  // the number of objects by type
    int               nObjs;         // the number of live objs
    int               nConstrs;      // the number of constraints
    int               nBarBufs;      // the number of barrier buffers
    int               nBarBufs2;     // the number of barrier buffers
    // the backup network and the step number
    Abc_Ntk_t *       pNetBackup;    // the pointer to the previous backup network
    int               iStep;         // the generation number for the given network
    // hierarchy
    Abc_Des_t *       pDesign;       // design (hierarchical networks only)     
    Abc_Ntk_t *       pAltView;      // alternative structural view of the network
    int               fHieVisited;   // flag to mark the visited network
    int               fHiePath;      // flag to mark the network on the path
    int               Id;            // model ID
    double            dTemp;         // temporary value
    // miscellaneous data members
    int               nTravIds;      // the unique traversal IDs of nodes
    Vec_Int_t         vTravIds;      // trav IDs of the objects
    Mem_Fixed_t *     pMmObj;        // memory manager for objects
    Mem_Step_t *      pMmStep;       // memory manager for arrays
    void *            pManFunc;      // functionality manager (AIG manager, BDD manager, or memory manager for SOPs)
    Abc_ManTime_t *   pManTime;      // the timing manager (for mapped networks) stores arrival/required times for all nodes
    void *            pManCut;       // the cut manager (for AIGs) stores information about the cuts computed for the nodes
    float             AndGateDelay;  // an average estimated delay of one AND gate
    int               LevelMax;      // maximum number of levels
    Vec_Int_t *       vLevelsR;      // level in the reverse topological order (for AIGs)
    Vec_Ptr_t *       vSupps;        // CO support information
    int *             pModel;        // counter-example (for miters)
    Abc_Cex_t *       pSeqModel;     // counter-example (for sequential miters)
    Vec_Ptr_t *       vSeqModelVec;  // vector of counter-examples (for sequential miters)
    Abc_Ntk_t *       pExdc;         // the EXDC network (if given)
    void *            pExcare;       // the EXDC network (if given)
    void *            pData;         // misc
    Abc_Ntk_t *       pCopy;         // copy of this network
    void *            pBSMan;        // application manager
    void *            pSCLib;        // SC library
    Vec_Int_t *       vGates;        // SC library gates
    Vec_Int_t *       vPhases;       // fanins phases in the mapped netlist
    char *            pWLoadUsed;    // wire load model used
    float *           pLutTimes;     // arrivals/requireds/slacks using LUT-delay model
    Vec_Ptr_t *       vOnehots;      // names of one-hot-encoded registers
    Vec_Int_t *       vObjPerm;      // permutation saved
    Vec_Int_t *       vTopo;
    Vec_Ptr_t *       vAttrs;        // managers of various node attributes (node functionality, global BDDs, etc)
    Vec_Int_t *       vNameIds;      // name IDs
    Vec_Int_t *       vFins;         // obj/type info
    Vec_Int_t *       vOrigNodeIds;  // original node IDs
};

    Abc_Ntk_t * Abc_NtkAlloc( Abc_NtkType_t Type, Abc_NtkFunc_t Func, int fUseMemMan );

    extern char *        Extra_UtilStrsav( const char *s );

}
#endif