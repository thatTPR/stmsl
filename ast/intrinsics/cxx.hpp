#ifndef STMSL_CXX
#define STMSL_CXX
#include "..smts.hpp"
#include "..backend.hpp"

    const type _Void("void",type::prim::Void);
    const type _Float("Float",type::prim::Float);
    const type _Double("Float",type::prim::Double);
    const type _UInt("UInt",type::prim::Uint);
    const type _Int("Int",type::prim::Int);
    const type _Bool("Bool",type::prim::Bool);


    // New Delete
    void funcNew(void* ptr){};
    void func_NewArr(void* ptr,size_t s){};
    void funcDelete(void* ptr){};
    void func

#endif