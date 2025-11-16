#ifndef STMSL_AST
#define STMSL_AST
#include <initializer_list>
#include <petri/list.hpp>
#include <petri/stack.hpp>
#include <petri/variant.hpp>
#include "sys.cpp"
#include "lex.hpp"
#include "intrinsics.hpp"
#include <functional>
namespace stmsl{

#include <string>
enum struct temp {meta,inst};

struct op {
    enum ty {opType,
        opNoExcept,opdot=lex::ty::dot,oppack=lex::ty::pack,
        opcond=lex::ty::cond,opcolon=lex::ty::colon,
        opdotptr=lex::ty::dotptr,
        opNew,opDelete,opNewArr,opDeleteArr,
        opbnot=lex::ty::bnot,
        oparrow=lex::ty::arrow,
        opthree=lex::ty::three,
        opeq = lex::ty::eq,
        
        oppeq=lex::ty::peq,opxoreq=lex::ty::xoreq,opmeq=lex::ty::meq,opandeq=lex::ty::andeq,oporeq=lex::ty::oreq,opNoteq=lex::ty::Noteq,opmuleq=lex::ty::muleq,opdiveq=lex::ty::diveq,
        opNot=lex::ty::Not,
        oplus=lex::ty::plus,
        opmod=lex::ty::mod,
        opmodeq=lex::ty::modeq,
        ominus=lex::ty::minus,
        oband=lex::ty::band,
        obor=lex::ty::bor,
        obxor=lex::ty::bxor,
        omul=lex::ty::mul,
        odiv=lex::ty::div,
        olt=lex::ty::ltangle,olteq=lex::ty::lteq,strml=lex::ty::strmlt,strmleq=lex::ty::strmlteq,
        ogt=lex::ty::gtangle,ogteq=lex::ty::gteq,strmg=lex::ty::strmgt,strmgeq=lex::ty::strmgteq,
        opp=lex::ty::pp,
        omm=lex::ty::mm,
        opand=lex::ty::oand,
        opor=lex::ty::oor,
        opxor = lex::ty::bxor,opoxoreq=lex::ty::xoreq,
        oindex=lex::ty::lbrack,
        ocall = lex::ty::lparen,
        err,none
    }
    ty oper;
    op(lex::ty tp) {
        case::lex::plus : {oper=ty::oplus;break;}
        case::lex::minus : {oper=ty::ominus;break;}
        case::lex::band : {oper=ty::oband;break;}
        case::lex::bor : {oper=ty::obor;break;}
        case::lex::bxor : {oper=ty::obxor;break;}
        case::lex::mul : {oper=ty::omul;break;}
        case::lex::div : {oper=ty::odiv;break;}
        case::lex::ltangle : {oper=ty::olt;break;}
        case::lex::gtangle : {oper=ty::ogt;break;}
        case::lex::pp : {oper=ty::opp;break;}
        case::lex::mm : {oper=ty::omm;break;}
        case::lex::oand : {oper=ty::opoand;break;}
        case::lex::oor : {oper=ty::opoor;break;}
        default : {oper=ty::err;}
    }
};


template <temp q>
struct ref ;
template <temp q>
struct value ;

template <temp q>
struct type;
template <temp q>
std::string getName(type<q>& ty);
using nstype = pri::deque<type<temp::inst>>;
template <temp q>
using type_list<q> = pri::deque<type<q>> ;


template <temp q>
struct param ;
template <temp q>
struct param_list : public pri::deque<param<q>>;




enum class result {rNs,rFunc,rOperator,rEnum,rEnumMem,rVar,rType,rUsing,rTypeDef,rParam,rErr};
template <temp q>
using resty = pri::variant<param<q>*,stmt<q>::NS*,stmt<q>::FuncDecl*,stmt<q>::OperatorDecl*,stmt<q>::Enum*,stmt<q>::Enum::member*,stmt<q>::VarDecl*,stmt<q>::DeclType*,stmt<q>::Using*,stmt<q>::rTypeDef*>;
template <temp q>
struct accMember {
        bool tmplt;param_list<temp::inst> plist;lex::ty acc;op::ty oprt;
        std::string name;
        result r=result::rErr;
        resty<temp::meta> inst;
        accMember(result _r){r=r;}
        accMember(std::string str){name=str;acc=lex::ty::dcolon;}
        accMember(type<q>* _a){it=_a;}
    };
template <temp Q>
struct accMember_list : public pri::deque<accMember<q>> {bool globalAcc;bool resolvec;}
    


template <template <temp> typename T>
T<temp::inst> getNode(T<temp::meta>& n);
// TODO mabe specialize separately;
template <temp q>
struct param {
    enum ty {Typename,Type,PtrToMember};bool  tmplt=false;
    bool pack=false;
    ty t;//in param<inst> a value of Typename signfies the qualification of dependent type 
    type<q>* arg;
    value<q> val;// For ty::Type
    param_list<q> pl;// For Template
    accMember_list<q> memberList;
    bool value_def;std::enable_if<q==temp::meta,param<temp::inst>>::type valdef; 
    using tyInst=pri::variant<expr<temp::meta>,type<temp::meta>,type<temp::inst>,accMember_list<q>>;bool Expr;
    std::enable_if<q==temp::meta,expr<q>>::type exprVal;
    tyInst varg;
    std::string name;
    std::string str(){
        switch(t){
            case ty::Typename : {return return std::string("Type");}
            case ty::Type : {return std::string("Type") }
            case ty::Template {return std::string("template")}
            case ty::PtrToMember {return std::string("memberPtr")}
        }
    };
static constexpr temp other_q = q==temp::meta?temp::inst:temp::meta;
    bool operator==(param<other_q>& rhs){ if(t==ty::Type){ return (t==rhs.t)and (rhs.val<=pri::get<type<temp::inst>(rhs.arg)) and (pack==t.pack);}
        return t==rhs.t and (pack==t.pack) };
    bool operator==(param<q>& rhs){
        if(t==ty::Type){ return (t==rhs.t)and (pri::get<type<temp::inst>>(arg)==pri::get<type<temp::inst>(rhs.arg)) and (pack==t.pack);}
        return t==rhs.t and (pack==t.pack) };
    
    void operator=(expr<q>& r ){
        if constexpr(q==temp::inst){if(t==ty::Type){pri::get<expr<q>>(varg)=r;return;}}
        std::string str="Expr not assignable for"+str()+ "parameter";
         throw ValueError(str);
    };
    param(type<q>& _Ty) {ptr=&_Ty;if(q==temp::meta){
        if(ty.Template){t=ty::Template;}
        else{t=ty::Type;}}
        else{t=ty::Type;}}
    param(std::string str,ty _t) :name(str) t(_t) ;
    template <temp Q>
    param(std::string str,type<Q>& tp) : name(str) { pri::get<type<Q>>(arg)=tp;};
    template <temp Q>
    param(type<Q>& tp) : name(str) { pri::get<type<Q>>(arg)=tp;name=getName<Q>(tp);};


param(uint ref) {*this=param(_UInt);construct_type(&_UInt,ref);}
param(int ref) {*this=param(_Float);construct_type(&_Float,ref);}
param(float ref) {*this=param(_Int);construct_type(&_Int,ref);}
param(bool ref) {*this=param(_Bool);construct_type(&_Bool,ref);}
param() = default ;

};


template <temp q>
struct param_list : public pri::deque<param<q>> {
    param<q>* findByName(std::string name){for(param<q>& it : *this){if(name==it.name){return &it;}}
    throw MemberNotFound<param_list<q>>("In Param List");
}  
} ;
template <temp q>
struct other_q {static constexpr temp other = q==temp::meta?temp::inst:temp::meta;}
template <temp q>
bool operator==(param_list<q>& lhs , param_list<other_q<q>::other>& rhs){
    param_list::iter itr = rhs.begin();
    param_list::iter itl = lhs.begin();
    for(;itl!=lhs.end() and (itr!=rhs.end());++itl){
        if( *it!=*itr){return false;};
        ++itr;
    };
    if(itr==rhs.end() and (itl==lhs.end())){return true;}
    if constexpr (q==temp::meta){
        if(itr==rhs.end()){return false;}
        if(lhs.back().pack){
            for(;itr;++itr){if(*itr!=lhs.back()){return false}}
        }
    }
    else {
        if(itl==lhs.end()){return false;}
        if(rhs.back().pack){
            for(;itl;++itl){if(*itl!=rhs.back()){return false}}
        }
    }
    return true;
};
template <temp q>
struct stmt ;
template <temp q>
accSpec getAcc(stmt<q>& st);

template < template <temp > typename Tt>
Tt<temp::inst> getInstance(Tt<temp::meta>& f,param_list<q> plist={});


    class TypeError : public std::exception {
    std::string msg;param::ty t;
public:
    explicit TypeError(const param::ty t, const std::string& message) : msg(message) {}

    // override what() to return description
    const char* what() const noexcept override {return msg.c_str();}
};
template <temp q>
type<q> getName(std::string name,param_list<q>& plist){
    for(param<q> it : plist){if(it.name==name){
        if(it.t=param::type::Typename){
            return pri:get<type<temp::inst>(it.arg);
        }
        else {
            throw TypeError(it.t,"TypeError"); 
        }   
    }
};
}

template <temp q>
struct expr;

struct integralT {
    enum ty {flt,Int,Double,Ch,Ty};ty t;
    enum len{Short,Long,LongLong};
    len l;sign s;
    enum sign{Signed,Unsigned};
    template <typename T>
    using mSigned=signed T;
    template <typename T>
    using mUnSigned=unsigned T;
    value<temp::meta> mt;
    bool is(type<temp::meta>& ref){};
    // integralT(type<temp::meta>)
};
struct enmember {
    std::string name;
    expr<temp::meta> cexprval;  
    enmember(std::string str) : name(str){}
};
struct EnumT : public pri::deque<enmember> {
    bool enclass=false;attrib_list ats;
    integralT intt; bool SpeqSeq=false;
    using member = enmember;
    expr<temp::meta> find(std::string name){
        for(enmember& it : *this){if(it.name==name){return &it;}}
        throw MemberNotFound<EnumT>("Enum Member Not Found");
    };
    operator bool (){return isDependent;}
} ;
#include "stmt.hpp"

struct attrib{
    using argList = pri::deque<lex>;
    std::string name;std::string nsacc;bool Ns=false;
    argList args;
    attrib(std::string str) : name(str){};
};
struct attrib_list : public pri::deque<attrib>{
    std::string nsUse;bool useNs;
};
template <temp q>
struct lambda ;


template <temp q>
struct value {
    enum ty {anyvalue,
        prvalue=0b100 // operator expressions
        xvalue=0b01,member,ptrmember,swizzle,arr // member.access,swizzle,arr[]
        lvalue=0b10,varRef,thisRef,funcCall,literal,initlist // variable name , function call,literal
        rvalue, // prvalue or xvalue // If set to this that means we have paren;
        glvalue=0b11,// xvalue or lvalue
        typeValue,lambdav
    };    
    enum vlty { }
    ty generalTy(ty y){
        switch(){
case ty::member : {return ty::xvalue;}
case ty::swizzle : {return ty::xvalue;}
case ty::arr : {return ty::xvalue;}
case ty::varRef : {return ty::lvalue;}
case ty::funcCall : {return ty::lvalue;}
case ty::literal : {return ty::lvalue;}
default: {return y;}
        }
    };
    ty t=ty::anyvalue;
    
    type<q>* tp;
    int ref;
    bool isPtr(){return ref>0;} 

    using InitList = pri::deque<expr<q>>;
    template <temp q>
    struct _funcCall ;
    template<> struct _funcCall<temp::inst> {
        param_list<q> prms;
        stmt<temp::meta>::arg_list args;
        stmt<q>::StmtFuncDef* funcDecl;

    }
    template <>struct _funcCall<temp::meta>{
        accMember_list acclist;
        stmt<temp::meta>::arg_list argl;
        _funcCall(accMember_list& _acclist,
        stmt<temp::meta>::arg_list& _argl) : acclist(_acclist) , argl(_argl) {}
    };
    using FuncCall = _funcCall<q>;

    using variable =stmt<q>::StmtVarDecl*;
    struct literal {
        pri::variant<float,uint,int> lit;enum ty{FltLit,UintLit,IntLit};ty t;
        literal(float num):t(ty::FltLit) {pri::get<float>(lit)=num;} 
        literal(uint num):t(ty::UintLit) {pri::get<uint>(lit)=num;}
        literal(int num):t(ty::IntLit) {pri::get<int>(lit)=num;}
    };
    
    template <meta q>
    struct ptrMember;
    template<> struct ptrMember <temp::meta>{accMember_list compound_tp ;accMember_list member;
        stmt<temp::meta>::VarDecl* get_member(){return pri::get<stmt<temp::meta>::VarDecl*>(type.back().inst);}
        stmt<temp::meta>::type* get_tp(){return pri::get<stmt<temp::inst>::type*>(b);}
    }
    template<> struct ptrMember <temp::inst>{type<temp::inst> compound_tp ;stmt<temp::inst>::VarDecl* member;}


    #ifdef CXX_C
    using ty = pri::variant<bool,int,uint,float,std::string,char>;
    #elif
    using ty = pri::variant<bool,int,uint,float,std::string>;
    #endif
    using valty=typename std::conditional<temp::meta==q,
    pri::variant<accMember_list, ptrMember<q>,funcCall,literal,lambda<q>,InitList>,
    pri::variant<ptrMember<q>,variable,funcCall,literal,lambda<q>>,InitList >::type;
    valty val;

    expr<q> val;

    
    enum truTy {
        eaccList,eptrMember,efuncCall,eliteral,elambda,eInitList
    };

    tryTy tt;
    template <ty tp>void set(){t=tp;};
    param<temp::inst> get_prm(){param<temp::inst> res;}
    void addArgs(stmt<q>::arg_list&& argl){
        // TODO;
    };
    template <typename T>
    void setTruT();
    template <> setTruT<accMember_list>(){tt=truTy::eaccList;}
    template <> setTruT<ptrMember<q>>(){tt=truTy::eptrMember;}
    template <> setTruT<funcCall>(){tt=truTy::efuncCall;}
    template <> setTruT<lambda<q>>(){tt=truTy::elambda;}
    template <> setTruT<InitList>(){tt=truTy::eInitList;}
    template <typename T>
    value(T&& v,ty p){pri::get<T>(val)=v;t=p;setTruT<T>();}
    value(type<q>& _tp){tp=&_tp;}
    value(type<q>& _tp,ty _t){tp=&_tp;t=_t;}
    template <typename T>
    value(T v){t=ty::literal;
        if constexpr (std::is_same<T,float>::value){tp=&_Float;}
        if constexpr (std::is_same<T,uint>::value){tp=&_UInt;}
        if constexpr (std::is_same<T,bool>::value){tp=&_Bool;}
        if constexpr (std::is_same<T,char>::value){tp=&Float;}
        if constexpr (std::is_same<T,std::string>::value){tp=&Float;}
    }
    value(expr<q>&& e) : {t=ty::rvalue;val=e;}
    
};


template <temp q>
struct expr {
    struct node {
        ConstVal cval=ConstVal::unknown;
                enum ConstVal {ConstExpr,Const,no,value,unknown};

        bool isConstExpr(){return cval==ConstVal::ConstExpr;}
        bool isConst(){return cval==ConstVal::Const;}
        op::ty o=op::ty::none;
        enum opty {prefixUnary,postfixUnary,binary,ternary,None};
        op::ty prefix;op::ty postfix;bool Prefix;bool Postfix;
        value<q> val;
        bool cexpr(){if(cval==ConstVal::ConstExpr ){return true;}};
        bool TrailOp(){pri::OneOf<opty::binary,opty::ternary>()}        
        value<q>::ty valueT(){return val.t;}

        node() = default;
        node(node& arg) {*this=arg;}
        node(value<q>&& vt) : val(vt){o=op::ty::none}; 
        node(expr<q>& e)  {val=value<q>(e);};
        node(expr<q>&& e)  {val=value<q>(e);};
        node(opty ot) : opT(ot);
    
    };
    template <op::ty tyOp,node::opty Op>
    node::opty getOpTy(){
        switch constexpr (Op){
            case node::opty::prefixUnary :{
                switch constexpr (tyOp){
                    case op::ty::opp :{return true;}
                    case op::ty::omm :{return true;}
                    case op::ty::opNot :{return true;}
                };
                return false;
            };
            case node::opty::postfixUnary :{
                                switch constexpr (tyOp){
                    case op::ty::opp :{return true;}
                    case op::ty::omm :{return true;}
                };
                return false;
            };
            case node::opty::binary :{
                switch constexpr (tyOp){
                    case op::ty::opp :{return false;}
                    case op::ty::omm :{return false;}
                    case op::ty::opNot :{return false;}
                };
                return true;
            };
        }
    };
    pri::deque<node> e;
    value<q> val;
    void addFuncCall(stmt<q>::arg_list&& argl){e.back().val.addArgs(argl)}
    value<q>& val(){return val;}
        template <op::ty OpT,node::opty cur=node::opty::binary>
        void add(){e.back().opTy=cur;e.back().o=OpT;};
        template <op::ty OpT> add<OpT,node::opty::prefixUnary>(){e.back().prefix=opT;e.back().Prefix=true;}
        template <op::ty OpT> add<OpT,node::opty::prefixUnary>(){e.back().postfix=opT;e.back().Postfix=true;}

        template <op::ty OpT,node::opty  OpTy>
        void push(){e.push_back(node());add<OpT,OpTy>();};


    void emplace(value<q>&& args){
        if(e.back().o==none){e.back().val=args;}
        else e.emplace_back(args);
    };
    void emplace(expr<q>&& args){
         if(e.back().o==none){e.back().val=value<q>(args);}
        else e.emplace_back(args);
    };
    void addFuncCall(){};
    template <typename >
    void pushLiteral(T val){
        if(e.back().o!=op::ty::none){
            e.back().val=value<q>(val);
        }else e.emplace_back(value<q>(val));}
    type<q> getType(){return tree.val.getType()};
    operator type<q>(){return getType();}
    expr<q> Not(){
        expr<q> res;
        res.tree.opT=node::opty::prefixUnary;
        res.tree.o=op::ty::opNot;
        res.tree.rhs=new node(tree);
        return res;
    };


    void validateConstExpr(){
        pri::deque<node>::iter it;bool isCexpr;
        for(it=e.begin() ;it!=e.end();++it ){
            if(it->cexpr()){
                
            }
          
        };
    };
    operator bool(){
        if(tree.vt==valuet::literal and (tree.ty.t==type::ty::Bool) ){return bliteral; }
    }
    bool isConstExpr(){return tree.getConstVal();}
    expr(bool s) : type(ty::literal) {bvalue=s;}
    expr(uint s) : type(ty::literal) {};
    expr(float flt) : type(ty::literal) {};
    expr(int s) : type(ty::litreal) {};
};
template <temp q>
struct lambda {

    stmt<q>::arg_list args;
    struct  capture {
        bool byRef;
        value<q> vl;
        capture(bool br,value<q>& value) : byRef(br), vl(value);
    } ;
    struct captureList : public pri::deque<value<q>>{bool byCopy=false;
        void push(bool byr,value<q>& vl){
            for(const auto it : *this){
                if(it.vl==vl){throw CaptureRepeated();}
            };
            emplace_back(byr,vl);
        }
    };
    bool Template=false;
    attrib_list front_ats;
    attrib_list back_ats;
    bool Noexcept=false;
    param_list<q> plist;
    using argList = stmt<q>::arg_list;
    using tyty= std::conditional<temp::meta==q,value<q>,type<q>>::type
    tyty rettp;bool trailing=false;
    attrib_list attribs;
    stmt<q>::block body;
};

template <temp q>
struct type {
    static constexpr temp tempState = q;
    bool Template ;
    bool dependentType;//Set if type is dependent of superType
    enum ty {//constructor=0,expr=1,
        vec=0,
        func=2,Buffer=3,strct=4,arr=5,
        Image2D=6,Image3D=7,Sampler=8,
        Param,Ns,Alias,enum,
        // Vec=24,Mat=25,Ivec=26,Imat=27,Uvec=28,Umat=29,Bvec=30,Bmat=31
    }
    enum prim {Void=9,Float=10,Int=11,Uint=12,Bool=13,pod,ctype}

    template <prim PrimT>struct primType {using type =void;}
    template <>struct primType<prim::Float> {using type =float;};
    template <>struct primType<prim::Int> {using type =int;};
    template <>struct primType<prim::Uint> {using type =uint;};
    template <>struct primType<prim::Bool> {using type =bool;};

        

    op::tyop arr[] ;
    std::string name ; 
    param_list<q> prms;
    std::enable_if<q==temp::meta,bool>::type tempTy;
    std::enable_if<q==temp::meta,pri::deque<type<temp::inst>>::type insts;
    ty t;
    // prim pt;
    // accSpec acc=accSpec::public;
    struct spec {
        type<q> q;
        param_list<temp::meta> plist;
        param_list<temp::inst> specl;
        bool operator==(param_list<temp::inst>& pl);
        bool operator<=(param_list<temp::inst>& pl);
        bool isCloser(spec& ot,param_list<temp::inst>& pl){
            size_t sot=0;
            for(param<temp::inst> it : ot.specl){
                for(param<temp::meta> ite : ot.plist){
                    if(specl)
                };
            };

        };
    };
    bool isTemplate=false;
    bool isUnion=false;
    // enum TYPE {compound,pod,member}
    // TYPE varType;
    bool compound;
    struct inher {
        accSpec acc;
        using tyty  = typename std::conditional<temp::meta==q,value<q>::accMember_list<q>,type<q>*>::type;
        tyty t;
        inher(accSpec spc) : acc(spc);
        inher(tyty r) : t(r){} 
    };
        using inher_list=pri::deque<inher>;      
        pri::deque<inher> inherits;
        stmt<q>::specTree<type<q>> specs;
        pri::deque<stmt<q>::DeclType*> frndsTypes;// DeclType
        pri::deque<stmt<q>::FuncDecl*> frndsFunctions; // DeclFunction
        pri::deque<stmt<q>::OperatorDecl*> frndsOperators; // DeclFunction
    
        pri::deque<stmt<q>::Using> usings;// Usings stmts
        pri::deque<stmt<q>::TypeDef> tdefs;// 
        
        pri::deque<stmt<q>::DeclType> types;// 
        // #ifdef CXX_C
        pri::dequq<stmt<q>::Union> unions;
        // #endif CXX_C
        pri::deque<stmt<q>::Enum> Enums; 
        
        pri::deque<stmt<q>::VarDecl> variables;
        pri::deque<stmt<q>::FuncDecl> methods;// Has type FuncDecl
        pri::deque<stmt<q>::OperatorDecl> operators;// Has type StmtOperatorDecl
        
        pri::deque<stmt<q>::ConstructorDecl> constructors; 
        pri::dequq<stmt<q>::ConstructorDecl> destructors;

        template <typename T , pri::deque<T> type<q>::* _ptr>
        struct accRange {
            pri::deque<T> type<q>::* ptr=_ptr;
            using ty= T;
            accSpec acc; 
            pri::deque<T>::iter from,to;pri::deque<T>::iter start;
            void end(){++start;from=start;}
            accRange(accSpec& acc,pri::deque<T>::iter&& _start) : acc(_acc)  :start(_start) {}
        };
        pri::deque<accRange<stmt<q>>::Using,&type<q>::usings> range_usings;
        pri::deque<accRange<stmt<q>>::TypeDef,&type<q>::tdefs> range_tdefs;
        pri::deque<accRange<stmt<q>>::DeclType,&type<q>::types> range_types;
        pri::deque<accRange<stmt<q>>::Union,&type<q>::unions> range_unions;
        pri::deque<accRange<stmt<q>>::Enum,&type<q>::Enums> range_enums;
        pri::deque<accRange<stmt<q>>::VarDecl,&type<q>::variables> range_variables;
        pri::deque<accRange<stmt<q>>::FuncDecl,&type<q>::methods> range_methods;
        pri::deque<accRange<stmt<q>>::OperatorDecl,&type<q>::operators> range_operators;
        pri::deque<accRange<stmt<q>>::ConstructorDecl,&type<q>::constructors> range_constructors;
        pri::deque<accRange<stmt<q>>::ConstructorDecl,&type<q>::destructors> range_destructors;



        template <typename T,pri::deque<T> type<q>::* ptr >
        void _setAcc(accSpec acc){

        };
        template <temp Q>
        static void _procInherits();
        template<> static void _procInherits<temp::inst>(){

        };
        template<> static void _procInherits<temp::meta>(){};

        static void procInherits(){inline _procInherits<q>();}

        struct Instance {
            type<q>* t; 
        };
        Instance getInst(){
            type<q> inst ; inst = *this;
            
        };

        
        bool isPod(){   
            if(!constructors.empty() or !destructors.empty()){return false;}
            for(stmt<q>::FuncDecl* it :methods){if(it->Virtual){return false;}}
            else return true;
        };

    template <typename T ,pri::deque<T> type<q>::* ptr >
    T* find(std::string name ){
        for(T& it : this->*ptr ){if(it->name ==name){return &it;}}
        throw NameNotFound<T>
    };

     void find(std::string name,result* r,resty<q>* res){
            try { pri::get<VarDecl*>(*res)=find<VarDecl,&type<q>::variables>(name);*r=result::rVar;return;} catch (const NameNotFound<VarDecl>& e){}
            try { pri::get<FuncDecl*>(*res)=find<FuncDecl,&type<q>::methods>(name);*r=result::rFunc;return;} catch (const NameNotFound<FuncDecl>& e){}
            try { pri::get<TypeDecl*>(*res)=find<TypeDecl,&type<q>::types>(name);*r=result::rType;return;}         catch (const NameNotFound<TypeDecl>& e){}
            try { pri::get<Enum*>(*res)=find<Enum,&type<q>::Enums>(name);*r=result::rEnum;return;}           catch (const NameNotFound<Enum>& e){}
            try { pri::get<Using*>(*res)=find<Using,&type<q>::usings>(name);*r=result::rUsing;return;}        catch (const NameNotFound<Using>& e){}
            try { pri::get<TypeDef*>(*res)=find<Union,&NS::tdefs>(name);*r=result::rTypeDef;return;}            catch (const NameNotFound<TypeDef>& e){}
            #ifdef CXX_C
            try { pri::get<Union*>(*res)=find<Union,&type<q>::unions>(name);*r=result::rUnion;return;}        catch (const NameNotFound<Union>& e){}
            #endif
                        throw NameNotFound();
    }


    template <typename T>
    struct ptrmem ;
    template <>struct ptrmem<stmt<q>::DeclType> {static constexpr pri::deque<stmt<q>::DeclType> type<q>::* ptr=&type<q>::types ;}
    template <>struct ptrmem<stmt<q>::VarDecl> {static constexpr pri::deque<stmt<q>::VarDecl> type<q>::* ptr=&type<q>::vars ;}
    template <>struct ptrmem<stmt<q>::Using> {static constexpr pri::deque<stmt<q>::Using> type<q>::* ptr=&type<q>::variables ;}
    template <>struct ptrmem<stmt<q>::FuncDecl> {static constexpr pri::deque<stmt<q>::FuncDecl> type<q>::* ptr=&type<q>::methods ;}
    template <>struct ptrmem<stmt<q>::Enum> {static constexpr pri::deque<stmt<q>::Enum> type<q>::* ptr=&type<q>::Enums ;}
    template <>struct ptrmem<stmt<q>::TypeDef> {static constexpr pri::deque<stmt<q>::TypeDef> type<q>::* ptr=&type<q>::tdefs ;}
    template <>struct ptrmem<stmt<q>::Union> {static constexpr pri::deque<stmt<q>::Union> type<q>::* ptr=&type<q>::unions ;}
    template <>struct ptrmem<stmt<q>::OperatorDecl> {static constexpr pri::deque<stmt<q>::OperatorDecl> type<q>::* ptr=&type<q>::operators ;}
    
    
    template <typename T>
    T* findWhat(std::string name){
        for( T&  it: this->*ptrmem<T>::ptr ){
            if(name == it.name){return it;}
        };  throw NameNotFound();
    };
    
    void find(std::string name,result* r,resty<q>* res ){
        pri::get<*>(*res) = findWhat(name) ;

        try{pri::get<stmt<q>::DeclType*>(*res) = findWhat<stmt<q>::DeclType>(name);return;}catch(const NameNotFound& e){}; 
        try{pri::get<stmt<q>::VarDecl*>(*res) = findWhat<stmt<q>::VarDecl>(name);return;}catch(const NameNotFound& e){}; 
        try{pri::get<stmt<q>::Using*>(*res) = findWhat<stmt<q>::Using>(name);return;}catch(const NameNotFound& e){}; 
        try{pri::get<stmt<q>::FuncDecl*>(*res) = findWhat<stmt<q>::FuncDecl>(name);return;}catch(const NameNotFound& e){}; 
        try{pri::get<stmt<q>::Enum*>(*res) = findWhat<stmt<q>::Enum>(name);return;}catch(const NameNotFound& e){}; 
        try{pri::get<stmt<q>::TypeDef*>(*res) = findWhat<stmt<q>::TypeDef>(name);return;}catch(const NameNotFound& e){}; 
        try{pri::get<stmt<q>::Union*>(*res) = findWhat<stmt<q>::Union>(name);return;}catch(const NameNotFound& e){}; 
        throw NameNotFound()
    };
    template <temp Q>
    stmt<q>::OperatorDecl* findOperator(op::ty op,arg_list args,param_list<Q>& prms,op::ty op){// TODO correctly
        for(stmt<q>::OperatorDecl it : operators){if(!it.typeConv and (it.opt==op) and ()){return &it;} };
    };
    template <temp Q>
    stmt<q>::OperatorDecl* findOperator(op::ty op,arg_list args,param_list<Q>& prms,type<q>* tyt);
    template <>
    stmt<q>::OperatorDecl* findOperator<temp::inst>(op::ty op,arg_list args,param_list<Q>& prms,type<q>* tyt){
        for(stmt<q>::OperatorDecl it : operators){if(it->typeConv and (tyt== it.opv)){return &it;} };
    };
    
    template <temp Q>
    ConstructorDecl* findConstructors(op::ty op,arg_list args,param_list<Q>& prms) ;
        template <>
    ConstructorDecl* findConstructors<temp::ins>(op::ty op,arg_list args,param_list<Q>& prms) ;{

    };

    template <typename T>
    void push(T&& p){this->*ptrmem<T>::ptr.push_back(p);};

    
    void pushConstructor(stmt<q>::ConstructorDecl&& c ){constructors.push_back(c);}
    void pushDesctructors(stmt<q>::ConstructorDecl&& c ){destructors.push_back(c);}


    bool exists(std::string name,result* r = result::rErr,resty<q>* res=nullptr){
        try {find(name,r,res);} catch (const  NameNotFound& e){return false}
        return true;
    };

    
    size_t alignment=1;size_t size;
    void calcAlignment(){alignment=1;
        for(type<q> it : ts  ){it.calcAlignment();if(it.alignment>alignment){alignment=it.alignment;}}};
    void calcSize(){size=0;
        for(type<q> it : ts  ){it.calcAlignment();size+=it.size;}};};


    template <temp Qr,temp Qs>
    type<Qr> get(parameter_list<Qs>& plist){
        for(spec& it : this->specs){
            if(it==plist){return it;}
        };
        for(spec& it : this->specs){
            if(it<=plist){return it;}
        };
    };

    template <bool Name>
    bool approxEq(type<q>& s){
        if(t != s.t){return false;}
        if(t>=24){if(dims!=s.dims){return false;}}
        if constexpr(Name){
            if(s.name != name){return false;}
        };
        return true;
    };
    bool tyEq(type<q>& s){
            if(ts.size()!= s.ts.size()){return false;}
        pri::list<type<q>>::iter tit = ts.begin();
        pri::list<type<q>>::iter sit = s.ts.begin();
        while(tit!=ts.end() and sit != s.ts.end() ){
            if(*tit != *sit){return false;}
            ++tit;++sit;
        };
        return true;
    };    
    bool operator<=(type<q>& s){return approxEq<true>(s)}
    bool operator<(type<q>& s){
        if(!approxEq<true>(s)){return false;}
        return ts.size()<s.ts.size();
    };
    bool operator>=(type<q>& s){return approxEq<false>(s)}
    bool operator>(type<q>& s){
        if(!approxEq<false>(s)){return false;}
        return ts.size()<s.ts.size();
    };
    bool operator==(type<q>& s){
        if(!approxEq<true>(s)){return false;}
        return tyEq(s);
    };
    bool operator!=(type<q>& s){
        if(!approxEq<false>(s)){return false;}
        return tyEq(s);        
    };
    void calc_alignment(){size_t biggest=1;
        for(const type it : ts){
            if(it.alignment>biggest){biggest=it.alignment;}
        }
        alignment =biggest;
    };

    bool valid_param(param_list<temp::inst>& plist){
        param_list<q>::iter titer= prms.begin(); 
        for(const auto& it : plist){
            if(it.t!=titer->t){return false;}
            ++titer;
        };
        for(;titer;++titer){
            if(!titer->value_default){return false;}
        }
        return true;
    };
    template <temp Qt,temp Qp>
    type<Qt> get(param_list<Qp>& plist){
        
        if(Qp==temp::meta){
            if(Qp==temp::meta)
            type<Qt> h ;h.prms=plist;
        }
        else {
            if(!this->valid_param(plist)){throw std::logic_error("Param List Invalid");}
            if(Qt==temp::inst){ // Replace Params
                size_t =0;
                for(param<Qp>& it : plist){
                    
                };
            }
            else {

            }
        };
    };
   
    
    type(std::string str, prim _tp) : name(str) , pt(_tp)  {}
    type(std::string str, ty _tp) : name(str) , t(tp)  {}


    type(type<q> __ty,pri::deque<size_t> _dims){*this=__ty;dims=_dims;}

    type(std::string _name,prim _ty,param_list<q> _prms) :t(_t) {prms=_prms;tempTy=true;pt=_ty;};
    type(std::string _name,ty _ty,param_list<q> _prms) :t(_t) {prms=_prms;tempTy=true;t=_ty;};  

    template <temp Qt,temp Qp>
    type(std::string _name,type<Qt> _t,param_list<Qp> _prms) :t(_t) {*this=_t.get(_prms);name=_name;};


size_t dim;pri::deque<size_t> dims;

    bool hasSwizzle(){if(t==ty::vec or(t==ty::mat)){return true;}else return false;}
    bool isCompound(){return compound;}
    
    struct swizzle {
    enum c {
        x= 0,y= 1,z= 2,w= 3,
        r= 0,g= 1,b= 2,a= 3,
        u= 0,v= 1,
        s= 0,t= 1,p= 2,q= 3,
    };
    //xyzw,rgba,
    static const size_t swiz_err=100;
    size_t swizz(char c) {
        switch(c){
case 'x' :{return 0;};
case 'y' :{return 1;};
case 'z' :{return 2;};
case 'w' :{return 3;};
case 'r' :{return 0;};
case 'g' :{return 1;};
case 'b' :{return 2;};
case 'a' :{return 3;};
case 'u' :{return 0;};
case 'v' :{return 1;};
case 's' :{return 0;};
case 't' :{return 1;};
case 'p' :{return 2;};
case 'q' :{return 3;};
        }
        return swiz_err;
    };
  
    std::vector<size_t> swizz(std::string sw){
        if(sw.size()>dim){throw SwizzleError("Swizzle ")};
        std::vector<size_t> res;
        for(char c : sw){res.push_back(swizz(c));}
        return res;
    };
    size_t size;
    std::vector<size_t > swi;
    bool operator==(type<inst>& f){
        if(s.size()<=f.dim)
    };
     
    swizzleTy(std::string s,type<inst>& i){
        for(char c : s){
            size_t h = siwzz(c);
            if(h!=swiz_err){
                if(h<=i.dim){swi.push_back(h);}
                else{err::e<err::t::swizzle_fortype>()}
            }
            else{err::e<err::t::swizzle_notexist>()}
        };
    };

};
    template <size_t s, size_t... Ss>
    void pushSize(size_t s,size_t... Ss){
        dims.push_back(s);
        if(sizeof...(Ss)>0){pushSize(Ss);}
    }

    type(std::string str,size_t size) : t(ty::vec) name(str)  {dim=size;}
    template <size_t... Ss> 
    type(std::string str,type<temp::meta>& arrtemp,type<temp::inst>* ty,size_t ss,size_t... Ss) :t(ty::vec) dim(arrtemp=dim)  {*this=*ty;pushSize(ss,Ss...);}
    type(std::string str,size_t ss) : dim(ss) t(ty::vec) {name=str;dim=ss;}
};


template <temp q>
struct tyty {
    std::conditional<q==temp::meta,accMember_listype<q>>* t;
    size_t refN;size_t ptrN;
    bool fixArr ; 
};

template <temp Q>
std::string getName(type<Q>& tp){return tp.name;}


stmt<temp::inst>::tyty getTy(stmt<temp::meta>::tyty& r){

};


stmt<temp::inst>::block getBlock(stmt<temp::meta>::block& r){
    stmt<temp::inst>::block res;
    for(stmt<temp::meta>& rhs: r){res.push_back(getNode(rhs));}
    return res;
};
template <> stmt<temp::inst> getNode<stmt>(stmt<temp::meta>& l){
    switch(l.t){

    };
};


template <temp q>
bool operator==(stmt<q>::arg_list& argsl, pri::deque<expr<q>>& argsr){
    stmt<q>::arg_list::iter itl = argsl.begin();
    pri::deque<expr<q>>::iter itr= argsr.begin();
    for(;itl!=argsl.end() and (itr!=argsr.end);++itl){
        if(*itr!=*itl){return false;}
        ++itr;
    };
} ;
template <temp q>
accSpec getAcc(stmt<q>&  st){return st.acc;}


struct mStmtInclude {};
struct mStmtIf {};
struct mStmtElif {};
struct mStmtElifdef {};
struct mStmtElifndef {};
struct mStmtIfdef {};
struct mStmtIfndef {};
struct mStmtElse {};
struct mStmtEndIf{}
struct mStmtDefine {};
    enum ty{            _include,_define,_if,_ifdef,_ifndef,_elif,_elifdef,_else};
   
template <temp q,bool CEXPR>
bool ConstructorAvail(type<q> lhs,value<temp::inst>& vl,stmt<q>::StmtFuncDecl* res=nullptr){// Checks if there is a constructor for value
    for(stmt<q>& pr : lhs.constructors ) {
        stmt<q>::StmtFuncDecl& ref = pri::get<stmt<q>::StmtFuncDecl>(pr.var);
        if(CEXPR and ref.ConstExpr or !CEXPR){
            if (ref.args.size()==1 and (vl.ty==ref.args.front().tp)){
                res=&ref;return true;}
        };
    }
    return false;
};
template <temp q>
bool operator<(type<q>& lhs,value<temp::inst>& vl){// Checks if there is a constructor for value
    return ConstructorAvail<q,false>(lhs,vl);};
bool operator<=(type<q>& lhs,value<temp::inst>& vl){// Same as < but with constexpr
    return ConstructorAvail<q,true>(lhs,vl);};


    template <temp q>
    StmtVarDecl& findVarName(stmt<q>::block& b,std::string name){
        for(stmt<q>& it : b){
            if(it.t==stmtty::VarDecl){return it;}
        };
    };
struct mStmtVersion;

    template <temp q>
    bool isPtr(result* r,resty* res){
        switch(*r){
            case result::rValue : {return pri::get<value<q>>(*res).isPtr();}
            case result::rVar : {return pri::get<stmt<q>::StmtVarDecl*>(*res)->isPtr()}
            default :{return false;}
        }
    };
    template <temp q>
    bool isCompound(result* r,resty* res){
        switch(*r){
            case result::rValue : {return pri::get<value<q>>(*res).tp->isCompound();}
            case result::rVar : {return pri::get<stmt<q>::StmtVarDecl*>(*res)->tp->isCompound()}
            case result::rType : {return pri::get<type<q>*>(*res)->isCompound();}
            default :{return false;}
        }

    };

    template <temp q>
    bool isMember(result* r,resty* res){return *r==result::rMethod ||*r==result::rVar;};
    template <temp q>
    bool isMemberPtr(result* r,resty* res){return isPtr(r,res) && isMember(r,res);};
    template <temp q>
    bool isPtrCompound(result* r,resty* res){return isPtr<q>(r,res) && isCompound<q>(r,res)};
    template <temp q>
    bool hasSwizzle(result* r,resty* res){
        switch(*r){
            case result::rValue : {return pri::get<value<q>>(*res).tp->hasSwizzle();}
            case result::rVar : {return pri::get<stmt<q>::StmtVarDecl*>(*res)->tp->hasSwizzle()}
            default :{return false;}
        }
    };
    template <temp q>
    bool hasMember(result* r,resty* res){return isCompound<q>(r,res) || hasSwizzle<q>(r,res);};
template <temp q>
struct ast   {
    uint version;
    pri::deque<stmt<q>::StmtLayout> layouts;
    stmt<q>::StmtNS global;
    
    
    
    pri::list<type<q>> Types;
    pri::list<stmt<q>::StmtFuncDecl> Funcs;
    
    bool nsblck=true;bool fblck;bool swBlock;bool IfChain;
    pri::deque<stmt<q>::block*> curBl;
    pri::deque<stmt<q>::StmtNS*> nss;
    pri::deque<stmt<q>::StmtDeclType*> strcts;
    pri::deque<param_list<q>*> curtemp;
    pri::stack<stmt<q>::StmtTry*> tries;
    stmt<q>::FuncDef* fdef;
    type<q>* curtp;

    stmt<q>::FuncDef* entry_pt;

    accSpec curAcc;

    stmt<q>::block& curBlock(){return *(curBl.back());};
    param_list<q>& curParams(){return *(curtemp.back());};

    stmt<q>::TypeDef& getTypeDef(){if(nsblck){nss.back().tdefs.back()}else{strcts.back()->tdefs.back()};}
    stmt<q>::TypeDef& getUsing(){if(nsblck){nss.back().usings.back()}else{strcts.back()->usings.back()};}
    stmt<q>* getStmt(){return curBl.back().back();}
    stmt<q>::VarDecl* getRef(std::string name){
        for(stmt<q>::VarDecl& it :  fdef->args){
            if(it.name==name){return it;}
        };
        for(stmt<q>::VarDecl& it :  fdef->args){
            if(it.name==name){return it;}
        };

    };

    bool pushNS(std::string name){
        if(curBl.t!=ns){return false;}
        curBl.push_back(nss.back()->includeNS(ns).body);
        return true;
    };
    
    void pushbl(stmt<q>::block& bl){curBl.emplace(&bl);}
    void pushbl(stmt<q>::block& bl,param_list<q>& pl){curBl.emplace(&bl,&pl)}
    void popbl(){
        if(curBl.back().bl==&strcts.back().body){strcts.pop_back();}
        if(curBl.back().ty==bl::ty::Template){curtemp.pop_back();}
        if(curBl.back().ty==bl::ty::ns){nss.pop_back();}
        curBl.pop();
    };

    
    template <typename StmtTy,bool Strct=false> StmtTy* _pushStmt(StmtTy&& st ){curBlock().back()->push_back(stmt<q>(st));return &pri::get<StmtTy>(curBl().back().var;);};// Review

    template <bool Strct>stmt<q>::Case* _pushStmt<stmt<q>::Case>(stmt<q>::Case&& st ){curBl.back()->push_back(stmt<q>(st));return &pri::get<stmt<q>::block>(curBlock.back());};
    template <bool Strct>stmt<q>::Default* _pushStmt<stmt<q>::Default>(stmt<q>::Default&& st ){curBl.back()->push_back(stmt<q>(st));return &pri::get<stmt<q>::block>(curBlock.back());};
    template <bool Strct>stmt<q>::Using* _pushStmt<stmt<q>::Using,Strct>(stmt<q>::Using&& st ){if constexpr (Strct){nss.back().usings.push_back(st);return &nss.back().usings.back();}else{strcts.back().usings.push_back(st);return &strcts.back().usings.back();};};
    template <bool Strct>stmt<q>::DefType* _pushStmt<stmt<q>::DefType,Strct>(stmt<q>::DefType&& st ){if constexpr (Strct){nss.back().tdefs.push_back(st);return &(nss.back().tdefs.back());}else{strcts.back().tdefs.push_back(st);return &(strcts.back().tdefs.back());} ;};
    template <bool Strct>stmt<q>::FuncDef* _pushStmt<stmt<q>::FuncDecl,Strct>(stmt<q>::FuncDef&& st ){
        if constexpr (!Strct){nss.back().funcs.push_back(st);return &(nss.back().funcs.back());}
        else{strcts.back().methods.push_back(st);return &(strcts.back().methods.back());};
    }
    template <>stmt<q>::FuncDef* _pushStmt<stmt<q>::OperatorDecl,Strct>(stmt<q>::OperatorDecl&& st ){
        if constexpr (Strct){nss.back().operators.push_back(st);return &(nss.back().operators.back());}
        else{strcts.back().operators.push_back(st);return &(strcts.back().operators.back());};
    }    
    
    
     template <template StmtTy,bool Strct>StmtTy* __pushStmt(StmtTy&& st ){StmtTy* ptr=_pushStmt<StmtTy,Strct>(st);curBl.push_back(ptr->body);return ptr ;};
    template <bool Strct> __pushStmt<stmt<q>::block,Strct>(stmt<q>::block&& st){StmtTy* ptr = _pushStmt<stmt<q>::block,Strct>(st);curBl.push_back(ptr);return ptr;}
    template <typename StmtTy,bool Strct>
    StmtTy* pushStmt(StmtTy&& st ){return _pushStmt<StmtTy,Stct>(st);};
    
template <bool Strct> stmt<q>::block* pushStmt<stmt<q>::block,bool Strct>(stmt<q>::block&& st){return __pushStmt<stmt<q>::block,Strct>(st);}
template <bool Strct> stmt<q>::If* pushStmt<stmt<q>::If,bool Strct>(stmt<q>::If&& st){return __pushStmt<stmt<q>::If,Strct>(st);}
template <bool Strct> stmt<q>::Else* pushStmt<stmt<q>::Else,bool Strct>(stmt<q>::Else&& st){return __pushStmt<stmt<q>::Else,Strct>(st);}
template <bool Strct> stmt<q>::ElseIf* pushStmt<stmt<q>::ElseIf,bool Strct>(stmt<q>::ElseIf&& st){return __pushStmt<stmt<q>::ElseIf,Strct>(st);}
template <bool Strct> stmt<q>::Switch* pushStmt<stmt<q>::Switch,bool Strct>(stmt<q>::Switch&& st){return __pushStmt<stmt<q>::Switch,Strct>(st);}
template <bool Strct> stmt<q>::Case* pushStmt<stmt<q>::Case,bool Strct>(stmt<q>::Case&& st){return __pushStmt<stmt<q>::Case,Strct>(st);}
template <bool Strct> stmt<q>::Default* pushStmt<stmt<q>::Default,bool Strct>(stmt<q>::Case&& st){return __pushStmt<stmt<q>::Default,Strct>(st);}
template <bool Strct> stmt<q>::For* pushStmt<stmt<q>::For,bool Strct>(stmt<q>::For&& st){return __pushStmt<stmt<q>::For,Strct>(st);}
template <bool Strct> stmt<q>::While* pushStmt<stmt<q>::While,bool Strct>(stmt<q>::While&& st){return __pushStmt<stmt<q>::While,Strct>(st);}
template <bool Strct> stmt<q>::ForRange* pushStmt<stmt<q>::ForRange,bool Strct>(stmt<q>::ForRange&& st){return __pushStmt<stmt<q>::ForRange,Strct>(st);}
template <bool Strct> stmt<q>::Do* pushStmt<stmt<q>::Do,bool Strct>(stmt<q>::Do&& st){return __pushStmt<stmt<q>::Do,Strct>(st);}



    template <typename T,typename A>
    T* pushStmtTo(A* to, T&& st){to->push(st);};


    template <typename T>
    T::specty* pushSpec(T* ptr,T::specty&& s,param_list& plist ){ptr->push(s,plist);};

    stmt<q>& lastStmt(){return curBlock().back();}


    template <> pushStmt(stmt<q>::StmtLayout&& ly){laoyuts.push_back(l);}
    template <qual Q,qual... Qs>
    bool pushQual(){curBlock().back().push_qual(Q);pushQual<Qs...>()};
    template <qual Q>
    bool pushQual(){curBlock().back().push_qual(Q);};

    template <typename StmtTy>
    StmtTy& curStmt(){
        pri::get<StmtTy>(curBlock().back());
    };
    bool pushQual(qual ql){return curBlock().back().push_qual(t);};
    template <typename astNd>
    pri::list<astNd>& get(){pri::get<pri::list<astNd>>(tup);};

    type<temp::inst> getTp(type<temp::meta>* t, stmt<temp::inst>::param_list pl={}){
        return t->get<temp::inst,temp::inst>(pl);
    };

    template <typename T>
    T* findWhat(accMember_list name){

    };


    template <typename W ,typename F>
    void findWhatFrom(std::string name,T* what, F* from){
        if constexpr (std::is_same<F,stmt<q>::NS>::value){  what = from->find<T>(name,,res);}
        else if constexpr (std::is_same<F,stmt<q>::Enum>::value){
            static_assert(std::is_same<T,Enum::member>::value,"Enum has only enum member")
            what = from->find<T>(name,r,res);}
        else if constexpr (std::is_same<F,stmt<q>::Union>::value){what=from->find<T>(name,r,res);}
        throw NameNotFound();
    } ;
    template <typename F>
    void findFrom(std::string name,result* r,resty<q>* res, F* from){
        if constexpr (std::is_same<F,stmt<q>::NS>::value){cur->find(name,r,res);}
        else if constexpr (std::is_same<F,stmt<q>::Enum>::value){*r =result::rEnumMem; cur->findWhatFrom<Enum::member,Enum>(name,pri::get<Enum::member*>(res),from);}
        else if constexpr (std::is_same<F,stmt<q>::Union>::value){cur->find(name,r,res);}
        else if constexpr (std::is_same<F,DeclType<q>>::value){cur->tp.find(name,r,res);}
        else if constexpr (std::is_same<F,type<q>>::value){cur->find(name,r,res);}
        throw NameNotFound();
    };

    template <typename T >
    void findInNs(std::string name,result* r,resty<q>* res){
        for(pri::deque<param<q>>& it : pri::reverse(curTemp)){
                for(param<q>& ite : it){
                if(ite->name == name){*r=rParam;pri::get<param<q>*>(*res)=&ite;return;}
            };
        }
        throw NameNotFound();
    };

    void findInTp(std::string name,result* r,resty<q>* res){

    };
    bool find(accMember_list<q>& res){
        accMember_list<q>::iter last;
        accMember_list<q>::iter it = res.begin();
        result last;
        if(res.globalAcc){
            res.push_front(accMember());res.front().r=result::rNs;
            get<stmt<q>::NS*>(res.front().inst)=&global;last=res.begin();
        }
        else {find(res.front().name,&res.front().r,&res.front().inst);}
        for(++it;it!=res.end();++it){
            switch(last->r){
                case result::rType : {try{findFrom<type<q>>(it->name,&it->r,&it->inst,pri::get<type<q>*>(it->inst));}

                case result::rType : {try{findFrom<type<q>>(it->name,&it->r,&it->inst,pri::get<type<q>*>(it->inst));}
                    catch(const MemberNotFound& e){return false;};return true;};
                case result::rNs : {{try{findFrom<stmt<q>::NS>(it->name,&it->r,&it->inst,pri::get<stmt<q>::StmtNS*>(last->inst));}
                                catch(const MemberNotFound& e){return false;};return true;};}
                case result::rEnum : {
                    try {findFrom<stmt<q>::NS>(it->name,&it->r,&it->inst,pri::get<stmt<q>::Enum*>(last->inst));}
                    catch(const MemberNotFound& exc){ return false;err::e(*this,exc);}
                    return true;};
                
                case result::rParam : {return ;}                
            }
            last = it;
        }
    };
    bool include(ast&& s){
        if(!s.curBl.empty()){return false;}
        this->layouts.concat(s.layouts);
        global.includeNs(s.global);
    };

    stmt<q>::NS* findNs(bool glbl,pri::deque<std::string> nsname){
        if(glbl){
            stmt<q>::NS* cur=&global;
            for(const std::string str : nsname ){
                cur=cur->findNs(str);
            };
            return cur;
        }   
    }
    void pushUsingNS(bool& glbl,pri::deque<std::string>& nsName){
        nss.back()->useNs(findNs(glbl,nsName))
    };
    bool name(std::string name){
        while()
    };
    bool exists(accMember_list<q>& aclist){
        if(aclist.globalAcc){
            stmt<q>::NS* cur = global;
            for(accMember<q> it : aclist){
                cur->find
            };
        }
        else {
            stmt<q>::NS
        }
    };
    
    void includeNS(pri::deque<std::string>& nsName){nss.back()->includeNs(nsName);};
    
    template <typename StmtT,typename... Ts>
    void emplace_back(Ts... args){push_back(StmtT(args...));};

    

    ast() {nss.push_back(&global);};
};

#endif