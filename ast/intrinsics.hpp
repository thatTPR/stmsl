#ifndef STMSL_INTRINSICS_HPP
#define STMSL_INTRINSICS_HPP
#include "ast.hpp"
#include "parser.cpp"
#include <petri/queue.hpp>
#include <petri/templates.hpp>
namespace stmtsl {
// Types


// pragma

#include "intrinsics/shader.hpp"
struct pragma {
    std::string name;
    
    void (*proc)(parser& )
    pragma(std::string&& name ,void (*pro)(parer& ) )
};

void pragma_once(parser& p){p.curFile.once=true;};
void pragma_pack(parser& p){
    p.nextTOK();
    if(p.OneOfLex<lex::ty::lparen>()){nextTOK();
        if(p.OneOfLex<lex::ty::rparen>()){setAlign(align_default);}
        else if(p.lexitback().u.name=="push"){
            nextTOK();if(p.OneOfLex<lex::ty::rparen>()){pushAlign(align_cur);}
            else if(OneOfLex<lex::ty::comma>()){nextTOK();pushAlign(lexitback().u.unum);}
        }
        else if(p.lexitback().u.name=="pop"){popAlign();}
        else {setAlign(lexitback().u.unum);}
    }
};

pri::deque<pragma> pragmas{pragma("once",pragma_once),pragma("pack",pragma_pack)}; 
// Attributes 
#include "attribs.hpp"

// functions
};
#endif