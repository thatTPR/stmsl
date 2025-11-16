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

enum EnAt {
Atnoreturn,Atcarries_dependency,At_deprecated,at_fallthrough,at_maybe_unused,at_nodiscard,at_likely,at_likely,at_no_unique_address,at_assume,at_indeterminate,at_optimize_for_synchronized
}
template <Str s,typename STMTT,EnAt eat,typename Args=void>
struct at {
    using argty= argsty;
    static constexpr bool EnAt at=eat;
    std::string check(std::string& str){return s.get()==str;}
};

using at_noreturn = at<"noreturn",stmt<temp::meta>::FuncDef,EnAt::Atnoreturn>;
using at_carries_dependency = at<"carries_dependency",stmt<temp::meta>::FuncDef,EnAt::Atcarries_dependency>;
using at_deprecated = at<"deprecated",stmt<temp::meta>,EnAt::At_deprecated,std::string>;
using at_fallthrough = at<"fallthrough",stmt<temp::meta>,EnAt::at_fallthrough>;
using at_maybe_unused = at<"maybe_unused",stmt<temp::meta>,EnAt::at_maybe_unused>;
using at_nodiscard = at<"nodiscard",stmt<temp::meta>,EnAt::at_nodiscard,std::string>;
using at_likely = at<"likely",stmt<temp::meta>,EnAt::at_likely>;
using at_unlikely = at<"unlikely",stmt<temp::meta>,EnAt::at_likely>;
using at_no_unique_address = at<"no_unique_address",stmt<temp::meta>,EnAt::at_no_unique_address>;
using at_assume = at<"assume",stmt<temp::meta>,EnAt::at_assume,expr<temp::meta>>;
using at_indeterminate = at<"indeterminate",stmt<temp::meta>,EnAt::at_indeterminate>;
using at_optimize_for_synchronized = at<"optimize_for_synchronized",stmt<temp::meta>,EnAt::at_optimize_for_synchronized>;