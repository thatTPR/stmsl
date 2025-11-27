#ifndef STMSL_KW
#define STMSL_KW
#include "ast.hpp"

namespace stmsl{
enum pcntxt {
    turoot=        0b0000000000001,
    global=        0b0000000000010,
    funcblock=     0b0000000000100,
    funcargqual=   0b0000000001000,
    parameterlist= 0b0000000010000,
    declType=      0b0000000100000,
    layoutArgs=    0b0000001000000,
    layoutDef=     0b0000010000000,
    funcret=       0b0000100000000,
    stexpr=        0b0001000000000,
    swblock=       0b0010000000000,
    stmtDecl   =   0b0100000000000,
    macroExpr =    0b1000000000000
};


enum kwty {
    macro,
    qualifier,
    stmt,tempstmt,prim,layout_at,layout_qual,layout_Stmt
    accessSpec,
    funcStmt,
    loopStmt
};


template <kwty k>
struct contxt_kw{
    static constexpr size_t pc ;
    // static constexpr pcntxt pcs[] = {} ;
};

template <>struct contxt_kw<kwty::macro>{
    static constexpr size_t s =turoot ;};

template <>struct contxt_kw<kwty::name_attrib>{static constexpr size_t s =global|funcblock|stmtDecl|macroExpr ;};
template <>struct contxt_kw<kwty::stmt>{
    static constexpr size_t s =global|funcblock|macroExpr ;};
template <>struct contxt_kw<kwty::prim>{
    static constexpr size_t s =global|funcblock|macroExpr ;};
template <>struct contxt_kw<kwty::layout_at>{
    static constexpr size_t s = layoutArgs|macroExpr ;};
template <>struct contxt_kw<kwty::layout_qual>{
    static constexpr size_t s =layoutDef|funcargqual|macroExpr ;};
template <>struct contxt_kw<kwty::builtinFunc>{
    static constexpr size_t s = global|funcblock|macroExpr ;};
template <>struct contxt_kw<kwty::builtinVar>{
    static constexpr size_t s = global|funcblock|stexpr|macroExpr ;};
template <>struct contxt_kw<kwty::funcStmt>{
    static constexpr size_t s = funcblock|stexpr ;};

template <kwty k>
struct context_tuple {
    static constexpr size_t kwCntxt = contxt_kw<k>::s;
    // struct iter {
    //     size_t s;
    //     pcntxt operator*(){return s<=size?pcs[s]:pcs[0];}
    //     decltype(*this) operator++(){s++;return *this;}
    //     iter(size_t _s): s(_s){}
    // };
    // constexpr iter begin(){return iter(0);}
    // constexpr iter end(){return iter(size+1);}
    // bool operator==(iter& i){return s==i.s;}
    // constexpr pcntxt operator[](size_t i){return pcs[i];}
    template <pcntxt p>
    bool hasCntxt(){return kwCntxt&p;};
};



template <pri::Str s,typename KW,kwty kTY>
struct kw : hs<s>/*,context_tuple<kTY>*/ {
    static constexpr bool KW_SET=false;
    using type = KW;
    static constexpr kwty KT= kTY;
    static std::string name() {return s.str();}
    // bool _check(parser& p,std::string s){ 
    //     for(pcntxt it : *this){
    //         if(it==p.cntxt){return true;}
    //     };
    //     return false;
    // }
    bool check(std::string str){
        if(str!= name()){return false;}
        return true;};
    template <kwty kT>
    void _proc(parser& p){p.putKWt<type>;};
    template <>void _proc<kwty::macro>(parser& p){p.getMacro<KW>();};
    template <>void _proc<kwty::qualifier>(parser& p){p.putQualifier<KW>();};
    template <>void _proc<kwty::layout_Stmt>(parser& p){p.LayoutNew<KW>();};
    template <>void _proc<kwty::layout_at>(parser& p){p.putLayout<KW>();};
    template <>void _proc<kwty::layout_qual>(p.putLayoutQual<KW>());
    template <>void _proc<kwty::funcStmt>(parser& p){p.Stmt();}
    template <>void _porc<kwty::stmt>(parser& p){p.getStmt<KW>();}

    // template <>void _proc<kwty::accessSpec>(parser& p){p.accessSpec<access::>}
    // template <>void _proc<kwty::
    // template <>void _proc<kwty::builtinFunc>();
    // template <>void _proc<kwty::builtinVar>();
    template <>void _proc<kwty::loopStmt>(parser& p){p.putLoop<type>();};  
    static void proc(parser& p){_proc<kTY>(p);};
    // void lex(parser& p);
};

template <typename T,typename... Ts>
struct context_join {
    static constexpr size_t s = T::kwCntxt | context_join<Ts...>::s;
};

template <typename T>
struct context_join {
    static constexpr size_t s = T::kwCntxt | context_join<Ts...>::s;
};


template <typename T,typename... Ts>
struct KW_set {
    static constexpr bool KW_SET=true; 
    template <typename _T,typename... _Ts>
    bool _proc(parser& p){
        if constexpr (_T::check(p.lexitback().u.name)){_T::proc(p);}
        else if constexpr (sizeof...(_Ts)){return _proc<_Ts...>(p);}
        return false;
    };
    template <typename _T,typename... _Ts>
    bool _check(std::string& p){
        if constexpr (_T::check(p)){return true;}
        else if constexpr (sizeof...(_Ts)>0){return _check<_Ts...>(p);}
        return false;
    };

    template <typename _T,typename... _Ts>
    bool check(std::string& str){return _check<T,Ts...>(str);}
    bool proc(parser& p){return _proc<T,Ts...>(p);};

    bool found(parser& p){return _proc<T,Ts...>(p);}
};  


using kw_version =  kw<"#version",macroStmt::mStmtVersion,kwty::macro> ;
// void kw_version::proc(parser& p){    p.setVersion(p.getNum(p.untilEOL()));}

using kw_Incl = kw<"#include",macroStmt::mStmtInclude,kwty::macro> ;
using kw_Define =  kw<"#define",macroStmt::mStmtDefine,kwty::macro> ;
using kw_if =  kw<"#if",macroStmt::mStmtIf,kwty::macro> ;
using kw_else =  kw<"#else",macroStmt::mStmtElse,kwty::macro>;
using kw_elif =  kw<"#elif",macroStmt::mStmtElif,kwty::macro> ;
using kw_elifdef =  kw<"#elifdef",macroStmt::mStmtElifdef,kwty::macro> ;
using kw_elifndef =  kw<"#elifndef",macroStmt::mStmtElifndef,kwty::macro> ;
using kw_endif =  kw<"#endif",macroStmt::mStmtEndIf,kwty::macro> ;
using kw_ifndef =  kw<"#ifndef",macroStmt::mStmtIfndef,kwty::macro> ;
using kw_ifdef =  kw<"#ifdef",macroStmt::mStmtIfdef,kwty::macro> ;
using kw_Layout =  kw<"layout",stmt::Layout,kwty::layout_Stmt> ;

using KW_listm=KW_set<kw_version,kw_Incl,kw_Define ,kw_if ,kw_elif ,kw_elif ,kw_elifdef ,kw_elifndef ,kw_endif ,kw_ifndef ,kw_ifdef >;


template <pri::Str s,stmt::Layout::ty TY>
struct kw_lyt : kw<s,stmt::Layout,kwty::layout_at> {
    static constexpr stmt::Layout::ty typeLyt=TY;
} ; 

using kw_Location =  kw_lyt<"location",stmt::Layout::ty::location> ;
using kw_Binding =  kw_lyt<"binding",stmt::Layout::ty::binding> ;


using kw_uniform =  kw<"uniform",stmt::Layout,kwty::layout_qual> ;

using KW_LISTLYT=KW_set<KW_LISTLYT, kw_uniform >;
struct kw_Buffer :  kw<"buffer",kwty::layout_qual>{
    void proc(parser& p){
        p.curAcc.push(accSpec::Public);
        p.getStmt<stmt::DefType>();
    };
} ;
using KW_LISTLYT = KW_set<kw_Location, kw_Binding,kw_Buffer>; 
  
struct kw_Struct : public kw<"struct",type<temp::meta> ,kwty::Struct>{
    void proc(parser& p){
        p.accessPush<accSpec::Public>();
        p.getStmt<stmt::DeclType>();
    };
};

struct kw_Class : public kw<"class",type<temp::meta>,kwty::Struct>{
    void proc(parser& p){
        p.accessPush<accSpec::Private>();
        p.getStmt<stmt::DeclType>();
    };
};
struct kw_Union : public kw<"union",type<temp::meta>,kwty::Struct>{
    void proc(parser& p){p.getStmt<stmt::DeclUnion>();};
}

using kw_Enum = kw<"enum",Enum , kwty::stmt>;
using KW_TYPEKW=KW_set<kw_Enum, kw_Struct,kw_Class ,>;#ifdef CXX_C
template <Str s, op::ty op>
struct kw_opt  : public kw<s,expr,kwty::prim>{

};
using kw_New = kw<"new",op::ty::opNew>
using kw_Delete = kw<"delete",op::ty::opDelete>
using kw_sizeof = kw<"sizeof",op::ty::opSizeof>

using kw_const_cast = kw_cast<"const_cast",op::ty::ConstCast>
using kw_static_cast = kw_cast<"static_cast",op::ty::StaticCast>;
using kw_reinterpret_cast = kw_cast<"reinterpret_cast",op::ty::ReinterpretCast>;
using kw_dynamic_cast = kw_cast<"dynamic_cast",op::ty::DynamicCast>;
using KW_CASTS = KW_set<kw_const_cast,kw_static_cast,kw_reinterpret_cast,kw_dynamic_cast>;

using KW_OPS = KW_set<KW_New,kwDelete,kw_sizeof> ; 

using kw_Nullptr = kw_cast<"nullptr",value<temp::meta>,kwty::prim>;
using kw_Null = kw<"NULL",value<temp::meta>,kwty::prim>;
using kw_long = kw<"long",value<temp::meta>,kwty::prim>;
using kw_short = kw<"short",value<temp::meta>,kwty::prim>;
using kw_unsigned = kw<"unsigned",integralT,kwty::prim>;
using kw_signed = kw<"signed",integralT,kwty::prim>;
using KW_PRIM = KW_set<kw_New,kw_Delete,kw_Nullptr,kw_Null,kw_New,kw_long,kw_short,kw_unsigned,kw_signed,KW_OPS>

using kw_Auto = kw<"auto",value<temp::meta>::kwty::prim>;

using kw_This =kw<"this",type<temp::meta>,kwty::qualifier>; 
template <Str s,typename QQt>
struct kw_qual : public kw<s,QQt,kwty::qualifier>{void proc(parser& p){p.pushQual(QQt::quality);}};
using kw_Virtual =  kw_qual<"virtual",QVirtual>;
using kw_Override =  kw_qual<"override",QOverride> ;
using kw_Explicit =  kw_qual<"explicit",QExplicit> ;
using kw_Final =  kw_qual<"final",QFinal>;
using kw_Noexcept = kw_qual<"noexcept",QNoexcept>;
using kw_Constexpr =  kw_qual<"constexpr",QConstExpr> ;
using kw_Consteval = kw_qual<"consteval",qConstEval>;
using kw_Static =  kw_qual<"static",QStatic> ;
using kw_thread_local = kw_qual<"thread_local",Qthread_local>;
using kw_register  = kw_qual<"register",Qregister>;
using kw_Extern  = kw_qual<_Qual"extern" , QExtern >;
using kw_Const =  kw_qual<"const",QConst> ;
using kw_Volatile =  kw_qual<"volatile",QVolatile> ;
using kw_Inline = kw_qul<"inline",QInline>;
using kw_Flat =  kw_qual<"flat",QFlat> ;
using kw_in =  kw_qual<"in",QIn> ;
using kw_out =  kw_qual<"out",QOut> ;
using kw_inout =  kw_qual<"inout",QInout> ;

using KW_LYTQ=KW_set< kw_Flat , kw_in , kw_out , kw_inout >;
using KW_QUAL_STORAGE=KW_set<kw_Const,kw_Volatile,kw_Constexpr,kw_Constexpr,kw_Consteval,kw_Static,kw_thread_local,kw_register>;
using KW_QUAL=KW_set< kw_Virtual,kw_Override,kw_Explicit,kw_Final,kw_Noexcept,kw_Const,kw_Volatile,kw_Constexpr,kw_Consteval,kw_Static,kw_Inline >;

using KW_LISTKW=KW_set<KW_LISTKW, KW_QUAL , KW_LYTQ,kw_Extern,>;

using KW_LISTKW=KW_set<KW_LISTKW,kw_Auto,kw_This, KW_QUAL >;
#ifdef CXX_C 
using KW_LISTKW=KW_set<KW_LISTKW, KW_Union ,KW_PRIM>;
#endif

struct kw_Template :public  kw<"template",void,kwty::Tempstmt>{
    void proc(parser& p){p.getTemplate();};
} ;

using kw_concept = kw<"concept",void,kwty::stmt>;
using kw_requires = kw<"requires",void,kwty::stmt>;
using kw_pre = kw<"pre",void ,kwty::stmt>;
using kw_post=kw<"pre",void,kwty::stmt>;
using kw_Typename =  kw<"typename",void,kwty::stmt> ;
using kw_Typedef = kw<"typedef",void ,kwty::stmt>
using kw_Friend = kw<"friend",QFriend,kwty::qualifier>;
using kw_Operator = kw<"operator",stmt::OperatorDecl,kwty::stmt>;
using kw_Decltype = kw<"decltype",stmt::FuncDecl,kwty::stmt>;
using kw_alignas = kw<"alignas",stmt::FuncDecl,kwty::stmt>


using KW_LISTKW=KW_set<KW_LISTKW,kw_Template,kw_concept,kw_require,kw_pre,kw_post,kw_Typename ,kw_Friend,kw_Operator,kw_Decltype,alignas>;
template <pri::Str s,accSpec asT>
struct kw_as : public kw<s,void,kwty::accessSpec> {
    void proc(parser& p){
        p.until<lex::ty::colon>();
        p.erase();
        p.access<asT>();
    };
};
using kw_Public = kw_as<"public",accSpec::Public>;
using kw_Private = kw_as<"private",accSpec::Private>;
using kw_Protected = kw_as<"protected",accSpec::Protected>;
using kw_Using = kw<"using",stmt::Using,kwty::stmt>;

using KW_ACCS=KW_set<kw_Public , kw_Private , kw_Protected >;
struct kw_Namespace : public  kw<"namespace",stmt::NS,kwty::stmt>{
    void proc(parser& p){
        p.Fromuntil<lex::ty::lbrace>();}
};
using kw_goto = kw<"goto",stmt::Goto,kwty::funcStmt>;
using kw_If =  kw<"if",stmt::If,kwty::funcStmt> ;
using kw_Else =  kw<"else",stmt::Else,kwty::funcStmt> ;
using kw_Switch =  kw<"switch",stmt::Switch,kwty::funcStmt> ;
using kw_Case =  kw<"case",stmt::Case,kwty::funcStmt> ;
using kw_Default = kw<"default",stmt::Case,kwty::funcStmt>;
using kw_While =  kw<"while",stmt::While,kwty::loopStmt> ;
using kw_For =  kw<"for",stmt::For,kwty::loopStmt> ;
using kw_Try = kw<"try",stmt::Try,kwty::funcStmt>;
using kw_Catch = kw<"catch",stmt::Catch,kwty::funcStmt>;
using kw_Throw = kw<"throw",stmt::Throw,kwty::funcStmt>;
struct kw_Do : public  kw<"do",void,kwty::funcStmt> {
void proc(parser& p){
        p.until<lex::ty::lbrace>();p.getBlock<>();
        p.untilKW<true,kw_For,kw_While>();
    };
};

using kw_Return =  kw<"return",stmt::Return,kwty::funcStmt> ;
using kw_co_await = kw<"co_await",stmt::Await,kwty::funcStmt>; 
using kw_co_yield = kw<"co_yield",stmt::Yield,kwty::funcStmt>; 
using kw_co_return = kw<"co_return",stmt::Return, kwty::funcStmt>;

using kw_Break =  kw<"break",stmt::Break,kwty::stmtStmt> ;
using kw_Continue =  kw<"continue",stmt::Continue,kwty::stmtStmt> ;
using kw_Asm = kw<"asm",stmt::Asm,

using KW_LISTSTMT=KW_set<kw_If  , kw_Else  , kw_Switch,  kw_While, kw_For,kw_Do ,kw_Try ,kw_Catch ,kw_Throw, kw_Return , kw_Break , kw_Continue >;
using KW_SWSTMT=KW_set< kw_Case, kw_Default>;
using KW_LISTKW=KW_set<KW_LISTKW,KW_LISTSTMT , KW_SWSTMT>;
#ifdef CXX_20
using kw_export = public kw<"export",void,kwty::stmt>;
using kw_import = public kw<"import",void ,kwty::stmt>;
using KW_LISTKW=KW_set<KW_LISTKW, kw_export,kw_import >;
#endif

using KW_LIST=KW_set<KW_LISTKW ,KW_LISTLYT,KW_LISTPRIM>;

}
#endif