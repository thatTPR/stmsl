#ifndef STMSL_INTRINSICS_HPP
#define STMSL_INTRINSICS_HPP
#include "ast.hpp"
#include <petri/templates.hpp>
namespace stmtsl {
// Types


// pragma

#include "intrinsics/shader.hpp"
struct pragma {
    std::string name;
    
    void proc(pri::deque<lex> toks)
};

// Attributes 
#include "attribs.hpp"

// functions
};
#endif