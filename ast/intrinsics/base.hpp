#ifndef STMSL_CXX
#define STMSL_CXX
#include "..smts.hpp"
#include "..backend.hpp"


template <language l>
struct isaSetBase  {
    template <typename >
    using iterableFuncs = ; 
    using iterableTypes = ;
    using iterableMacros  = ;


    virtual constexpr iterableFuncs getFuncs();
    virtual constexpr iterableTypes getTypes();
    virtual constexpr iterableMacros getMacros();    
};


template <language l , target<l>::isa is >
struct isaSet : isaSetBase<l> ;


#include "cxx.hpp"
#include "shader.hpp"

template <target<language::stmsl>::isa is > struct isaSet<language::stmtsl,isa> {

};


#endif

