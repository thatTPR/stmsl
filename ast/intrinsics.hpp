#ifndef STMSL_INTRINSICS_HPP
#define STMSL_INTRINSICS_HPP
#include "ast.hpp"
#include <petri/templates.hpp>
namespace stmtsl {
// Types



const type<temp::inst> _image2D("image2D",type::ty::Image2D);
const type<temp::inst> _image2D("image3D",type::ty::Image3D);
const type<temp::inst> _image2D("Sampler",type::ty::Sampler);
const type<temp::inst> _Void("void",type::prim::Void);const type<temp::meta> m_Void("void",type::prim::Void);
const type<temp::inst> _Float("Float",type::prim::Float);
const type<temp::inst> _Double("Float",type::prim::Double);
const type<temp::inst> _UInt("UInt",type::prim::Uint);
const type<temp::inst> _Int("Int",type::prim::Int);
const type<temp::inst> _Bool("Bool",type::prim::Bool);

const type<temp::meta> Vec("Vec",1);
const type<temp::meta> Mat("Mat",2);

using UsingInst = typename stmt<temp::inst>::Using;

const type<temp::inst> vec2("vec2",Vec,&_Float,2);
const type<temp::inst> vec3("vec3",Vec,&_Float,3);
const type<temp::inst> vec4("vec4",Vec,&_Float,4);
const type<temp::inst> vec2("vec2",Vec,&_Double,2);
const type<temp::inst> vec3("vec3",Vec,&_Double,3);
const type<temp::inst> vec4("vec4",Vec,&_Double,4);
const type<temp::inst> ivec2("ivec2",Vec,&_Int,2);
const type<temp::inst> ivec3("ivec3",Vec,&_Int,3);
const type<temp::inst> ivec4("ivec4",Vec,&_Int,4);
const type<temp::inst> uvec2("uvec2",Vec,&_UInt,2);
const type<temp::inst> uvec3("uvec3",Vec,&_UInt,3);
const type<temp::inst> uvec4("uvec4",Vec,&_UInt,4);
const type<temp::inst> ivec2("ivec2",Vec,&_Int,2);
const type<temp::inst> ivec3("ivec3",Vec,&_Int,3);
const type<temp::inst> ivec4("ivec4",Vec,&_Int,4);
const type<temp::inst> bvec2("bvec2",Vec,&_Bool,2);
const type<temp::inst> bvec3("bvec3",Vec,&_Bool,3);
const type<temp::inst> bvec4("bvec4",Vec,&_Bool,4);
const type<temp::inst> mat2("mat2",Mat,&_Float,2);
const type<temp::inst> mat3("mat3",Mat,&_Float,3);
const type<temp::inst> mat4("mat4",Mat,&_Float,4);
const type<temp::inst> dmat2("mat2",Mat,&_Double,2);
const type<temp::inst> dmat3("mat3",Mat,&_Double,3);
const type<temp::inst> dmat4("mat4",Mat,&_Double,4);
const type<temp::inst> imat2("imat2",Mat,&_Int,2);
const type<temp::inst> imat3("imat3",Mat,&_Int,3);
const type<temp::inst> imat4("imat4",Mat,&_Int,4);
const type<temp::inst> umat2("umat2",Mat,&_UInt,2);
const type<temp::inst> umat3("umat3",Mat,&_UInt,3);
const type<temp::inst> umat4("umat4",Mat,&_UInt,4);
const type<temp::inst> imat2("imat2",Mat,&_Int,2);
const type<temp::inst> imat3("imat3",Mat,&_Int,3);
const type<temp::inst> imat4("imat4",Mat,&_Int,4);
const type<temp::inst> bmat2("bmat2",Mat,&_Bool,2);
const type<temp::inst> bmat3("bmat3",Mat,&_Bool,3);
const type<temp::inst> bmat4("bmat4",Mat,&_Bool,4);
const type<temp::inst> mat2x2("mat2x2",Mat,&_Float,2,2);
const type<temp::inst> mat3x2("mat3x2",Mat,&_Float,3,2);
const type<temp::inst> mat4x2("mat4x2",Mat,&_Float,4,2);
const type<temp::inst> dmat2x2("dmat2x2",Mat,&_Double,2,2);
const type<temp::inst> dmat3x2("dmat3x2",Mat,&_Double,3,2);
const type<temp::inst> dmat4x2("dmat4x2",Mat,&_Double,4,2);
const type<temp::inst> imat2x2("imat2x2",Mat,&_Int,2,2);
const type<temp::inst> imat3x2("imat3x2",Mat,&_Int,3,2);
const type<temp::inst> imat4x2("imat4x2",Mat,&_Int,4,2);
const type<temp::inst> umat2x2("umat2x2",Mat,&_UInt,2,2);
const type<temp::inst> umat3x2("umat3x2",Mat,&_UInt,3,2);
const type<temp::inst> umat4x2("umat4x2",Mat,&_UInt,4,2);
const type<temp::inst> imat2x2("imat2x2",Mat,&_Int,2,2);
const type<temp::inst> imat3x2("imat3x2",Mat,&_Int,3,2);
const type<temp::inst> imat4x2("imat4x2",Mat,&_Int,4,2);
const type<temp::inst> bmat2x2("bmat2x2",Mat,&_Bool,2,2);
const type<temp::inst> bmat3x2("bmat3x2",Mat,&_Bool,3,2);
const type<temp::inst> bmat4x2("bmat4x2",Mat,&_Bool,4,2);//
const type<temp::inst> mat2x3("mat2x3",Mat,&_Float,2,3);
const type<temp::inst> mat3x3("mat3x3",Mat,&_Float,3,3);
const type<temp::inst> mat4x3("mat4x3",Mat,&_Float,4,3);
const type<temp::inst> dmat2x3("dmat2x3",Mat,&_Double,2,3);
const type<temp::inst> dmat3x3("dmat3x3",Mat,&_Double,3,3);
const type<temp::inst> dmat4x3("dmat4x3",Mat,&_Double,4,3);
const type<temp::inst> imat2x3("imat2x3",Mat,&_Int,2,3);
const type<temp::inst> imat3x3("imat3x3",Mat,&_Int,3,3);
const type<temp::inst> imat4x3("imat4x3",Mat,&_Int,4,3);
const type<temp::inst> umat2x3("umat2x3",Mat,&_UInt,2,3);
const type<temp::inst> umat3x3("umat3x3",Mat,&_UInt,3,3);
const type<temp::inst> umat4x3("umat4x3",Mat,&_UInt,4,3);
const type<temp::inst> imat2x3("imat2x3",Mat,&_Int,2,3);
const type<temp::inst> imat3x3("imat3x3",Mat,&_Int,3,3);
const type<temp::inst> imat4x3("imat4x3",Mat,&_Int,4,3);
const type<temp::inst> bmat2x3("bmat2x3",Mat,&_Bool,2,3);
const type<temp::inst> bmat3x3("bmat3x3",Mat,&_Bool,3,3);
const type<temp::inst> bmat4x3("bmat4x3",Mat,&_Bool,4,3);//
const type<temp::inst> mat2x4("mat2x4",Mat,&_Float,2,4);
const type<temp::inst> mat3x4("mat3x4",Mat,&_Float,3,4);
const type<temp::inst> mat4x4("mat4x4",Mat,&_Float,4,4);
const type<temp::inst> dmat2x4("dmat2x4",Mat,&_Double,2,4);
const type<temp::inst> dmat3x4("dmat3x4",Mat,&_Double,3,4);
const type<temp::inst> dmat4x4("dmat4x4",Mat,&_Double,4,4);
const type<temp::inst> imat2x4("imat2x4",Mat,&_Int,2,4);
const type<temp::inst> imat3x4("imat3x4",Mat,&_Int,3,4);
const type<temp::inst> imat4x4("imat4x4",Mat,&_Int,4,4);
const type<temp::inst> umat2x4("umat2x4",Mat,&_UInt,2,4);
const type<temp::inst> umat3x4("umat3x4",Mat,&_UInt,3,4);
const type<temp::inst> umat4x4("umat4x4",Mat,&_UInt,4,4);
const type<temp::inst> imat2x4("imat2x4",Mat,&_Int,2,4);
const type<temp::inst> imat3x4("imat3x4",Mat,&_Int,3,4);
const type<temp::inst> imat4x4("imat4x4",Mat,&_Int,4,4);
const type<temp::inst> bmat2x4("bmat2x4",Mat,&_Bool,2,4);
const type<temp::inst> bmat3x4("bmat3x4",Mat,&_Bool,3,4);
const type<temp::inst> bmat4x4("bmat4x4",Mat,&_Bool,4,4);

#define INTRINSIC_TYS vec2,vec3,vec4,ivec2,ivec3,ivec4,uvec2,uvec3,uvec4,ivec2,ivec3,ivec4,bvec2,bvec3,bvec4,mat2,mat3,mat4,imat2,imat3,imat4,umat2,umat3,umat4,imat2,imat3,imat4,bmat2,bmat3,bmat4,mat2x2,mat3x2,mat4x2,imat2x2,imat3x2,imat4x2,umat2x2,umat3x2,umat4x2,imat2x2,imat3x2,imat4x2,bmat2x2,bmat3x2,bmat4x2,mat2x3,mat3x3,mat4x3,imat2x3,imat3x3,imat4x3,umat2x3,umat3x3,umat4x3,imat2x3,imat3x3,imat4x3,bmat2x3,bmat3x3,bmat4x3,mat2x4,mat3x4,mat4x4,imat2x4,imat3x4,imat4x4,umat2x4,umat3x4,umat4x4,imat2x4,imat3x4,imat4x4,bmat2x4,bmat3x4,bmat4x4
 
// pragma


struct pragma {
    std::string name;
    
    void proc(pri::deque<lex> toks)
};

// Attributes 
struct attribute {
    std::string name;
    virtual void proc()
    attribute()
};

// functions
};
#endif