#ifndef SHADI_HPP
#define SHADI_HPP
namespace stmsl {

    // TOOD make theses Using DeclType
const type _image2D("image2D",type::ty::Image2D);
const type _image2D("image3D",type::ty::Image3D);
const type _image2D("Sampler",type::ty::Sampler);

const stmt::DeclType Vec("Vec",type::ty::vec ,param_list<temp::meta>{param::ty::Type,size_t});
const stmt::DeclType Mat("Mat",type::ty::mat ,param_list<temp::meta>{param::ty::Type,size_t,size_t});

using UsingInst = typename stmt::Using;

#define STR(x) #x
#define XSTR(X) STR(x)
#define MKSTR(prefix,num) prefix XSTR(num)
#define VECDMACRO(n,T) const type vec##n(MKSTR("vec",n),param_list<temp::inst>(&T,n)); 
#define VECX(X) X(2,_Float,2) X(3,_Float)
#define VEXTX(X,...) X(__VA__ARGS__ , ) , 


template <stmt::DeclType* dt, Str vs, Str ms> 
struct DtVecName : pri::CEStr<s> {
    static constexpr stmt::DeclType* dt=_dt;
    constexpr std::string vec_name(){return vs.Str();};
    constexpr std::string mat_name(){return ms.Str();} 
};

using fname = DtVecName<&_Float,"vec","mat">;
using dname = DtVecName<&_Double,"dvec","dmat">;
using uname = DtVecName<&_UInt,"uvec","umat">;
using iname = DtVecName<&_Int,"ivec","imat">;
using bname = DtVecName<&_Int,"bvec","bmat">;
template <size_t... ss>
struct typeVecs {
    
    template <typename... tps>
    static constexpr pri::deque<type> get(){
        return pri::list<type>{((type(tps::vec_name()+std::to_string(ss) ,Vec, param_list<temp::inst>(tps::dt,ss) )...)...) };
    };


    template <size_t s1,size_t s2,typename... tps >
    static constexpr pri::deque<type> __getm(){
        if constexpr (s1==s2) {
            return pri::deque<type>{ (type(tps::mat_name() + std::to_string(s1), Mat , param_list<temp::inst>(tps::dt,s0,s1)) ...) } ;
        };
        return pri::deque<type>{ (type(tps::mat_name() + std::to_string(s1) + std::string(s2) , Mat , param_list<temp::inst>(tps::dt,s0,s1)) ...) } ;
    };
    template <typename... tps>
    static constexpr pri::list<type> getm(){
        pri::deque<type> ret;
        ((((ret+=__getm<ss,ss,tps...>())...)...)...);
        return ret;
};

template <>
template <size_t... sss

pri::deque<const type> vects = typeVec<2,3,4>::get<fname,fname,uname,iname,bname>();
pri::deque<const type> mats = typeMat<2,3,4>::getm<fname,fname,uname,iname,bname>();


stmt::DeclType  _is_same = []() ->stmt::DeclType {
    stmt::DeclType res("_is_one_of",param_list<temp::meta>{param<temp::meta>(param::ty::Typename),param<temp_meta>(param::ty::Typename)}); 
    return res;
};
stmt::DeclType _is_one_of = ([]()->stmt::DeclType {
    stmt::DeclType res("_is_one_of",param_list<temp::meta>{param<temp::meta>(param::ty::Typename),param<temp::meta>(param::ty::Typename),param<temp_meta>(param::ty::Typename,true)});
    param_list<temp::inst> pl ;pl.emplace_back(res.plist.front(),res.plist[1]);
    res.plist.front().refs.push_back(pl.front());res.plist[1].refs.push_back(pl[1]);
    expr e(_is_same.get(pl).variables.front().data) ;
    res.Def.variables.push_back(stmt::VarDecl("value",_Bool,std::vector<qual>{qual::qStatic,qual::qConstexpr},)
    return res;
})();


stmt::Concept _genType("genType",{param<temp::meta>("T",param::ty::Typename)},accMember_seq{accMember(_is_one_of,{"T"},)} )	//float / int / uint / double scalars & vectors
stmt::Concept _genIType("genIType",)//	integer scalar + integer vectors
stmt::Concept _genUType("genUType",)//	unsigned scalar + unsigned vectors
stmt::Concept _genBType("genBType",)//	boolean scalar + boolean vectors
stmt::Concept _genDType("genDType",)//	double scalar + double vectors
stmt::Concept _genFType("genFType",)//	float scalar + float vectors
stmt::Concept _genMType("genMType",)//	matrices (mat2, mat3, mat4, dmat*)


template <param_list<temp::meta> , param_list<temp::inst> specp>
struct spec {

};
template <param_list<temp::meta> , param_list<temp::inst>... Specs >
struct funcTy  {

};
template<Str s, typename FuncTy  > struct IntrinscFunc : CEStr<s> {
    template <lanage>
    stmt::FuncDecl()
};

// Trig



const stmt::FuncDecl getFunc(std::string str, param_list<temp::meta>&& plist , ){
    stmt::FuncDecl fdecl(plist,);
    fdecl.func
};

const stmt::FuncDecl


const stmt::FuncDecl _acos("acos",param_list<temp::meta>{param(rettT,inT),} ,rett, arg_list);
const stmt::FuncDecl _acosh("acosh",);
const stmt::FuncDecl _asin("asin",);
const stmt::FuncDecl _asinh("asinh");
const stmt::FuncDecl _atan("atan");
const stmt::FuncDecl _atanh("atanh");
const stmt::FuncDecl _cos("cos");
const stmt::FuncDecl _cosh("cosh");
const stmt::FuncDecl _degrees("degrees");
const stmt::FuncDecl _radians("radians");
const stmt::FuncDecl _sin("sin");
const stmt::FuncDecl _sinh("sinh");
const stmt::FuncDecl _tan("tan");
const stmt::FuncDecl _tanh("tanh");
// Math
const stmt::FuncDecl _abs("abs");
const stmt::FuncDecl _ceil("ceil");
const stmt::FuncDecl _clamp("clamp");
const stmt::FuncDecl _dFdx("dFdx");
const stmt::FuncDecl _dFdy("dFdy");
const stmt::FuncDecl _exp("exp");
const stmt::FuncDecl _exp2("exp2");
const stmt::FuncDecl _floor("floor");
const stmt::FuncDecl _floor("floor");
const stmt::FuncDecl _fma("fma");
const stmt::FuncDecl _fract("fract");
const stmt::FuncDecl _fwidth("fwidth");
const stmt::FuncDecl _inversesqrt("inversesqrt");
const stmt::FuncDecl _isinf("isinf");
const stmt::FuncDecl _isnan("isnan");
const stmt::FuncDecl _log("log");
const stmt::FuncDecl _log2("log2");
const stmt::FuncDecl _max("max");
const stmt::FuncDecl _min("min");
const stmt::FuncDecl _mix("mix");
const stmt::FuncDecl _mod("mod");
const stmt::FuncDecl _modf("modf");
const stmt::FuncDecl _noise("noise");
const stmt::FuncDecl _pow("pow");
const stmt::FuncDecl _round("round");
const stmt::FuncDecl _roundEven("roundEven");
const stmt::FuncDecl _sign("sign");
const stmt::FuncDecl _smoothstep("smoothstep");
const stmt::FuncDecl _sqrt("sqrt");
const stmt::FuncDecl _step("step");
const stmt::FuncDecl _trunc("trunc");
// Flt
const stmt::FuncDecl _floatBitsToInt("floatBitsToInt");
const stmt::FuncDecl _frexp("frexp");
const stmt::FuncDecl _intBitsToFloat("intBitsToFloat");
const stmt::FuncDecl _ldexp("ldexp");
const stmt::FuncDecl _packDouble2x32("packDouble2x32");
const stmt::FuncDecl _packHalf2x16("packHalf2x16");
const stmt::FuncDecl _packUnorm("packUnorm");
const stmt::FuncDecl _unpackDouble2x32("unpackDouble2x32");
const stmt::FuncDecl _unpackHalf2x16("unpackHalf2x16");
const stmt::FuncDecl _unpackUnorm("unpackUnorm");
// Builtin Var

// Make This anonymous
const stmt::VarDecl _gl_PerVertex("gl_PerVertex" ,{qual::qOut},type(stmt::VarDecl(vec4,"gl_Position"),stmt::VarDecl(_Float,"gl_PointSize"),stmt::VarDecl(_Float,"gl_ClipDistance",type<temp::meta>::ty::arr)));

const stmt::VarDecl _gl_ClipDistance("gl_ClipDistance",  );
const stmt::VarDecl _gl_CullDistance("gl_CullDistance",);
const stmt::VarDecl _gl_FragCoord("gl_FragCoord");
const stmt::VarDecl _gl_FragDepth("gl_FragDepth");
const stmt::VarDecl _gl_FrontFacing("gl_FrontFacing");
const stmt::VarDecl _gl_GlobalInvocationID("gl_GlobalInvocationID");
const stmt::VarDecl _gl_HelperInvocation("gl_HelperInvocation");
const stmt::VarDecl _gl_InstanceID("gl_InstanceID");
const stmt::VarDecl _gl_InvocationID("gl_InvocationID");
const stmt::VarDecl _gl_Layer("gl_Layer");
const stmt::VarDecl _gl_LocalInvocationID("gl_LocalInvocationID");
const stmt::VarDecl _gl_LocalInvocationIndex("gl_LocalInvocationIndex");
const stmt::VarDecl _gl_NumSamples("gl_NumSamples");
const stmt::VarDecl _gl_NumWorkGroups("gl_NumWorkGroups");
const stmt::VarDecl _gl_PatchVerticesIn("gl_PatchVerticesIn");
const stmt::VarDecl _gl_PointCoord("gl_PointCoord");
const stmt::VarDecl _gl_PointSize("gl_PointSize");
const stmt::VarDecl _gl_Position("gl_Position");
const stmt::VarDecl _gl_PrimitiveID("gl_PrimitiveID");
const stmt::VarDecl _gl_PrimitiveIDIn("gl_PrimitiveIDIn");
const stmt::VarDecl _gl_SampleID("gl_SampleID");
const stmt::VarDecl _gl_SampleMask("gl_SampleMask");
const stmt::VarDecl _gl_SampleMaskIn("gl_SampleMaskIn");
const stmt::VarDecl _gl_SamplePosition("gl_SamplePosition");
const stmt::VarDecl _gl_TessCoord("gl_TessCoord");
const stmt::VarDecl _gl_TessLevelInner("gl_TessLevelInner");
const stmt::VarDecl _gl_TessLevelOuter("gl_TessLevelOuter");
const stmt::VarDecl _gl_VertexID("gl_VertexID");
const stmt::VarDecl _gl_ViewportIndex("gl_ViewportIndex");
const stmt::VarDecl _gl_WorkGroupID("gl_WorkGroupID");
const stmt::VarDecl _gl_WorkGroupSize("gl_WorkGroupSize");
// Vector
const stmt::FuncDecl _cross("cross");
const stmt::FuncDecl _distance("distance");
const stmt::FuncDecl _dot("dot");
const stmt::FuncDecl _equal("equal");
const stmt::FuncDecl _faceforward("faceforward");
const stmt::FuncDecl _length("length");
const stmt::FuncDecl _normalize("normalize");
const stmt::FuncDecl _notEqual("notEqual");
const stmt::FuncDecl _reflect("reflect");
const stmt::FuncDecl _refract("refract");
// Component Comparison
const stmt::FuncDecl _all("all");
const stmt::FuncDecl _any("any");
const stmt::FuncDecl _greaterThan("greaterThan");
const stmt::FuncDecl _greaterThanEqual("greaterThanEqual");
const stmt::FuncDecl _lessThan("lessThan");
const stmt::FuncDecl _lessThanEqual("lessThanEqual");
const stmt::FuncDecl _not("not");

// Geometry Shader
const stmt::FuncDecl _EmitStreamVertex("EmitStreamVertex");
const stmt::FuncDecl _EmitVertex("EmitVertex");
const stmt::FuncDecl _EndPrimitive("EndPrimitive");
const stmt::FuncDecl _EndStreamPrimitive("EndStreamPrimitive");
// Texture Sampling
const stmt::FuncDecl _interpolateAtCentroid("interpolateAtCentroid");
const stmt::FuncDecl _interpolateAtOffset("interpolateAtOffset");
const stmt::FuncDecl _interpolateAtSample("interpolateAtSample");
const stmt::FuncDecl _texelFetch("texelFetch");
const stmt::FuncDecl _texelFetchOffset("texelFetchOffset");
const stmt::FuncDecl _texture("texture");
const stmt::FuncDecl _textureGather("textureGather");
const stmt::FuncDecl _textureGatherOffset("textureGatherOffset");
const stmt::FuncDecl _textureGatherOffsets("textureGatherOffsets");
const stmt::FuncDecl _textureGrad("textureGrad");
const stmt::FuncDecl _textureGradOffset("textureGradOffset");
const stmt::FuncDecl _textureLod("textureLod");
const stmt::FuncDecl _textureLodOffset("textureLodOffset");
const stmt::FuncDecl _textureOffset("textureOffset");
const stmt::FuncDecl _textureProj("textureProj");
const stmt::FuncDecl _textureProjGrad("textureProjGrad");
const stmt::FuncDecl _textureProjGradOffset("textureProjGradOffset");
const stmt::FuncDecl _textureProjLod("textureProjLod");
const stmt::FuncDecl _textureProjLodOffset("textureProjLodOffset");
const stmt::FuncDecl _textureProjOffset("textureProjOffset");
const stmt::FuncDecl _textureQueryLevels("textureQueryLevels");
const stmt::FuncDecl _textureQueryLod("textureQueryLod");
const stmt::FuncDecl _textureSamples("textureSamples");
const stmt::FuncDecl _textureSize("textureSize");
// Matrix
const stmt::FuncDecl _determinant("determinant");
const stmt::FuncDecl _groupMemoryBarrier("groupMemoryBarrier");
const stmt::FuncDecl _inverse("inverse");
const stmt::FuncDecl _matrixCompMult("matrixCompMult");
const stmt::FuncDecl _outerProduct("outerProduct");
const stmt::FuncDecl _transpose("transpose");
// Integer
const stmt::FuncDecl _bitCount("bitCount");
const stmt::FuncDecl _bitfieldExtract("bitfieldExtract");
const stmt::FuncDecl _bitfieldInsert("bitfieldInsert");
const stmt::FuncDecl _bitfieldReverse("bitfieldReverse");
const stmt::FuncDecl _findLSB("findLSB");
const stmt::FuncDecl _findMSB("findMSB");
const stmt::FuncDecl _uaddCarry("uaddCarry");
const stmt::FuncDecl _umulExtended("umulExtended");
const stmt::FuncDecl _usubBorrow("usubBorrow");
// Image
const stmt::FuncDecl _imageAtomicAdd("imageAtomicAdd");
const stmt::FuncDecl _imageAtomicAnd("imageAtomicAnd");
const stmt::FuncDecl _imageAtomicCompSwap("imageAtomicCompSwap");
const stmt::FuncDecl _imageAtomicExchange("imageAtomicExchange");
const stmt::FuncDecl _imageAtomicMax("imageAtomicMax");
const stmt::FuncDecl _imageAtomicMin("imageAtomicMin");
const stmt::FuncDecl _imageAtomicOr("imageAtomicOr");
const stmt::FuncDecl _imageAtomicXor("imageAtomicXor");
const stmt::FuncDecl _imageLoad("imageLoad");
const stmt::FuncDecl _imageSamples("imageSamples");
const stmt::FuncDecl _imageSize("imageSize");
const stmt::FuncDecl _imageStore("imageStore");
// Atomic
const stmt::FuncDecl _atomicAdd("atomicAdd");
const stmt::FuncDecl _atomicAnd("atomicAnd");
const stmt::FuncDecl _atomicCompSwap("atomicCompSwap");
const stmt::FuncDecl _atomicCounter("atomicCounter");
const stmt::FuncDecl _atomicCounterDecrement("atomicCounterDecrement");
const stmt::FuncDecl _atomicCounterIncrement("atomicCounterIncrement");
const stmt::FuncDecl _atomicExchange("atomicExchange");
const stmt::FuncDecl _atomicMax("atomicMax");
const stmt::FuncDecl _atomicMin("atomicMin");
const stmt::FuncDecl _atomicOr("atomicOr");
const stmt::FuncDecl _atomicXor("atomicXor");
// Memory Barrier
const stmt::FuncDecl _barrier("barrier");
const stmt::FuncDecl _groupMemoryBarrier("groupMemoryBarrier");
const stmt::FuncDecl _memoryBarrier("memoryBarrier");
const stmt::FuncDecl _memoryBarrierAtomicCounter("memoryBarrierAtomicCounter");
const stmt::FuncDecl _memoryBarrierBuffer("memoryBarrierBuffer");
const stmt::FuncDecl _memoryBarrierImage("memoryBarrierImage");
const stmt::FuncDecl _memoryBarrierShared("memoryBarrierShared");



pri::deque<stmt::FuncDecl> fdecls =  
template <> void addGlobal<language::stmsl>(ast& a){
    a.global.types+=vects;a.global.type+=mats;
    a.global.funcs+=funcs;
};
};
#endif