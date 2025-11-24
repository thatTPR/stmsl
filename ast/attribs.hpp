#include <petri/templates.hpp>

/*

[[noreturn]]
indicates that the function does not return

[[carries_dependency]](removed in C++26)
indicates that dependency chain in release-consume std::memory_order propagates in and out of the function

[[deprecated]][[deprecated("reason")]] (C++14)(C++14)
indicates that the use of the name or entity declared with this attribute is allowed, but discouraged for some reason

[[fallthrough]](C++17) 
indicates that the fall through from the previous case label is intentional and should not be diagnosed by a compiler that warns on fall-through

[[maybe_unused]](C++17)
suppresses compiler warnings on unused entities, if any

[[nodiscard]][[nodiscard("reason")]]
(C++17)(C++20)
encourages the compiler to issue a warning if the return value is discarded

[[likely]][[unlikely]](C++20)(C++20)
indicates that the compiler should optimize for the case where a path of execution through a statement is more or less likely than any other path of execution

[[no_unique_address]](C++20)
indicates that a non-static data member need not have an address distinct from all other non-static data members of its class

[[assume(expression)]](C++23)
specifies that the expression will always evaluate to true at a given point

[[indeterminate]](C++26)
specifies that an object has an indeterminate value if it is not initialized

[[optimize_for_synchronized]](TM TS)
indicates that the function definition should be optimized for invocation from a synchronized statement


*/

enum class EnAt {e_noreturn,e_carries_dependency,e_deprecated,e_fallthrough,e_maybe_unused,e_nodiscard,e_likely,e_likely,e_no_unique_address,e_assume,e_indeterminate,e_optimize_for_synchronized}
template <Str s,,EnAt eat,typename... stTys>
struct at {
    static constexpr bool EnAt att=eat;
    pri::array<stmt::stmtty> tys = {stmt::getTyf<stTys>()...};

    template <stmt::sttmty t,stmt::stmtty... Tys>
    void _Default(stmtty& v,stmt::allvarptr& p){
        if(v==t){pri::get<t*>(p)->atse.push_back(att);}
        else if constexpr( sizeof...(Tys)>0){_Default<Tys...>(v,p)}
    }
    void Default(stmtty& v,stmt::allvarptr& p){
        _Default<stmt::getTyf<stTys...>>(v,p)
    }
    bool isTyValid(stmt::stmtty& v){
        for( stmt::stmty& r : tys){if(v==r){return true;}}
        return false;
    }
    void _func(attrib& at,stmt::stmtty& ty,stmt::allvar& p){Default(ty,p)};
    void func(attrib& at,stmt::stmtty& ty,stmt::allvarptr& v){
        if(!isTyValid()){throw AttributeNotForStmt(ty);}
        _func(ty,v);
    }
    std::string check(std::string& str){return s.get()==str;}
};

using at_noreturn : at<"noreturn",EnAt::e_noreturn,stmt::FuncDecl,stmt::FuncDef>;
using at_carries_dependency : at<"carries_dependency",EnAt::e_carries_dependency,stmt::FuncDecl,stmt::FuncDef>;
using at_deprecated : at<"deprecated",EnAt::e_deprecated,stmt::DeclType,stmt::DefType,stmt::TypeDef,stmt::VarDecl,stmt::FuncDecl,stmt::FuncDef,stmt::NS,stmt::Enum>;  
using at_fallthrough : at<"fallthrough",EnAt::e_fallthrough,stmt::Case>;
using at_maybe_unused : at<"maybe_unused",EnAt::e_maybe_unused,stmt::DeclType,stmt::DefType,stmt;:VarDecl,stmt::Enum,stmt::Enum::EnMember,stmt::StructuredBinding>;//  See c++26 ResultBinding feat
using at_nodiscard : at<"nodiscard",EnAt::e_nodiscard,stmt::FuncDecl,stmt::FuncDef,stmt::Enum,stmt::DeclType>;
using at_likely : at<"likely",EnAt::e_likely,stmt::FuncDecl,stmt::FuncDef>;
using at_unlikely : at<"unlikely",EnAt::e_likely,stmt::FuncDecl,stmt::FuncDef>;
using at_no_unique_address : at<"no_unique_address",EnAt::e_no_unique_address,stmt::VarDecl>;
using at_assume : at<"assume",EnAt::e_assume,stmt::NullStmt>{};
using at_indeterminate : at<"indeterminate",EnAt::e_indeterminate,stmt::VarDecl>;
using at_optimize_for_synchronized : at<"optimize_for_synchronized",EnAt::e_optimize_for_synchronized,stmt::FuncDef,smtt::FuncDecl>;




struct attribute {
    std::string name;EnAt eat;
std::array<stmt::stmtty>& sts;
    bool operator==(std::string n){return n==name;}
    void (*func)(attrib& at,stmt::stmtty& sts,stmt::allvarptr& v);

    attribute(std::string n,EnAt _e,void (*proc)(stmt::stmtty&,stmt::allvarptr&) , std::array<stmt::stmtty>& res) : name(n), eat(_e), func(proc),sts(res) {}
    
};
#define XATTRIB(x) x(at_noreturn), x(at_carries_dependency),x(at_deprecated) x(at_fallthrough), x(at_maybe_unused), x(at_nodiscard), x(at_likely), x(at_unlikely), x(at_no_unique_address), x(at_assume), x(at_indeterminate), x(at_optimize_for_synchronized)
#defien ATTRIBM(at) attribute(at::str(),at::att, &at::func,at::tys) 
pri::list<attribute> attribs{XATTRIB(ATTRIBM)};

