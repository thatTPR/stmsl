#ifndef STMSL_BACKEND_HPP
#define STMSL_BACKEND_HPP
#include <petri/templates.hpp>
#include <petri/macros.hpp>
#include <cstring>
namespace stmsl
{
    enum language {
        cpp,stmsl,
    }
    template <langauge l >
    struct envs ;
    template <language l>
    struct version ;


template <> struct target<language::cpp> {enum ver{_17,_20,_26,};

    #define XMAC_TARGET(X)    X(aarch64) X(aarch64_32) X(aarch64_be) X(amdgcn) X(arm) X(arm64) X(arm64_32) X(armeb) X(avr) X(bpf) X(bpfeb) X(bpfel) X(hexagon) X(lanai) X(mips) X(mips64) X(mips64el) X(mipsel) X(msp430) X(nvptx) X(nvptx64) X(ppc32) X(ppc64) X(ppc64le) X(r600) X(riscv32) X(riscv64) X(sparc) X(sparcel) X(sparcv9) X(systemz) X(thumb) X(thumbeb) X(wasm32) X(wasm64) X(x86)  X(xcore)
    
    #define ENUMERATE(n) n,
    enum isa {XMAX_TARGET(ENUMERATE)
        x86_64
    };

    template <Str s, isa _i>
    struct target_str : CEStr<s> {
        static constexpr isa i = _i;
    };
    #define TARGET_STR(n) struct isa_##n :target_str<#n,isa::n>{};
    XMAC_TARGET(ENUMERATE) 
    struct isa_x86_64 : target_str<"x86-64",isa::x86_64>{};
};
template <> struct target<language::stmsl> {enum ver{ spv10,spv11,spv12,spv13,spv14,spv15,spv16};enum isa {spv};};

template <Str s , e En>
struct tr : pri::CEStr<s> {static constexpr e t = En;};
#define TRTEMP(n) using tr_##n = tr<#n,e::n>;
XMAC_TARGET(TRTEMP)
using tr_x86_64 = tr<"x86-64",e::x86_64>;



}; // namespace stmsl

#endif