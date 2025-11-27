#ifndef STMSL_AST
#define STMSL_AST
#include <initializer_list>
#include <petri/templates.hpp>
#include <petri/list.hpp>
#include <petri/stack.hpp>
#include <petri/variant.hpp>
#include "sys.cpp"
#include "lex.hpp"
#include "intrinsics.hpp"
#include <functional>
#include "stmt.hpp"
namespace stmsl{

#include <string>
enum struct temp {meta,inst};

struct op {
    enum ty {opType,opdq=lex::ty::dq,
        opNoExcept,opdot=lex::ty::dot,oppack=lex::ty::pack,
        opcond=lex::ty::cond,opcolon=lex::ty::colon,
        opdotptr=lex::ty::dotptr,
        opNew,opDelete,opNewArr,opDeleteArr,opSizeof,opSizeofPock,
        StaticCast,ReinterpretCast,DynamicCast,ConstCast,
        opbnot=lex::ty::bnot,
        oparrow=lex::ty::arrow,
        opthree=lex::ty::three,
        opeq = lex::ty::eq,
        opcomma = lex::ty::comma
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
    op& operator=(lex& l){oper = l.t;pos=l.pos;}
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
std::string getName(type& ty);
using nstype = pri::deque<type>;
template <temp q>
using type_list<q> = pri::deque<type> ;


using constraint = expr;

template <temp q>
struct param ;
template <temp q>
struct param_list : public pri::deque<param<q>> {
    std::enbale_if<q==temp::meta , constraint> Constraint;
};

    
    struct ptrMember {
        type* tp;stmt::VarDecl* mem;
        void get(stmt::VarDecl* inst){
            auto iti = inst->variables.begin();
            auto itm = mem->getType().;
            for(tp->variables)
        };
        stmt::VarDecl* member(stmt::VarDecl& vd) { 
            for(type::accMem<stmt:VarDecl>& i  : vd.tp){      if(i->data == mem){return &i->data;}
            };
            throw MemberNotFound();
        };
        ptrMember(type* _t, stmt::VarDecl* _mem ) : tp(_t),mem(_mem) {} 
    };

struct dtype : Qualifiable<qual::QStatic,qConst,qVolatile,qExtern,qConstexpr> {type* t; size_t refNum=0; size_t ptrNum=0;expr arrSize={};dtype()=default;
    dtype(type* _t) :t(_t){}
};


enum class result {rNs,rFunc,rOperator,rEnum,rEnumMem,rVar,rType,rUsing,rTypeDef,rParam,rLambda,rConcept,rSwizzle,rErr};
using resty = pri::variant<param<temp::meta>*,stmt::NS*,stmt::FuncDecl*,stmt::OperatorDecl*,stmt::Enum*,stmt::Enum::member*,stmt::VarDecl*,stmt::DeclType*,stmt::Using*,stmt::rTypeDef*,stmt::Concept*,value>;
struct accMember {
        bool Template;param_list<temp::inst> plist;lex::ty acc = lex::ty::none;op::ty oprt;
        std::string name;
        result r=result::rErr;
        resty inst;

        stmt::init_args args;
        accMember(result _r){r=r;}
        accMember(std::string str){name=str;acc=lex::ty::dcolon;}
        accMember(type& _a){r=result::rType;pri::get<value>;}
        
        accMember(value& vl) { pri::get<value>(inst)=vl;r = result::rValue;}
    };
struct accMember_seq : public pri::deque<accMember> ,Qualifiable<qConst,qVolatile,qin,qout,qflat> {dtype dt;bool globalAcc;bool resolved;posit pos;bool Auto;
    
    
    accMember_seq& operator=(value& vl){accMember.emplace_back(vl);}
            void resolve(ast& a);
            
            bool isType(){};
            bool isPtrMember(){};
            bool isVariable(){};
    std::ofstream& operator<<(std::ofstream& of){
            if(globalAcc){of<<"&";}
        for(auto& it : *this){
            of<<it.name;if(it.Template){of<<plist;}
            if(!it.args.empty()){of<<"("<<args<<")";}
            of<<lex::getStr(acc);
        };
        return of;
    };
    
};

    
template <temp q>
struct param : Qualifiable<QUALS_EN>{
    enum ty {Typename,Type,PtrToMember};
    bool pack=false;
    ty t;//in param<inst> a value of Typename signfies the qualification of dependent type
    bool Template=false;  bool Concept =false ; stmt::Concept* cncp; 
    param_list<q> pl;
    std::enable_if<q==temp::meta, accMember_seq>::type tp ;
    std::enable_if<q==temp::meta, accMember_seq>::type  memberList;
    std::enable_if<q==temp::meta, param_list<temp::meta>>::type args; // Template args
    std::enable_if<q==temp::metea,pri::deque<accMember_seq::iter>>::type refs ;
    std::enable_if<q==temp::inst,expr>::type vl;
    std::string name;
    std::string str(){
        std::string str = Template ? "Template" :"";
        switch(t){
            case ty::Typename : {return str+std::string("Type");}
            case ty::Type : {return str+std::string("Type") }
            case ty::PtrToMember {return str+std::string("memberPtr")}
        }
    };
    template <ty _t>void __push(accMember* i);
    template <>void __push<ty::Typename>(accMember_seq::iter& i,param<temp::inst>& p){i->r==result::rType;*(pri::get<stmt::DeclType*>(i->inst))=vl.Typename();};
    template <>void __push<ty::Type>(accMember_seq::iter& i,param<temp::inst>& p){i->r==result::rType;pri::get<value>(i->inst)=vl.Value();};
    template <>void __push<ty::PtrToMember>(accMember_seq::iter& i,param<temp::inst>& p){i->r==result::rType;pri::get<ptrMember>(i->inst)=vl.PtrMember();};
    template <ty _t>
    void _push(param<temp::inst>& p){for(accMember_seq::iter& it : refs){__push<_t>(it,p);}};
    void push(param<temp::inst>& p){
        if(p.t!=t){err::e(*this,ParamMismatch(p));}
        switch(p.t){
            case ty::Typename : {_push<ty::Typename>(p);}
            case ty::Type : {_push<ty::Type>(p);}
            case ty::PtrToMember {_push<ty::PtrToMember>(p);}
        }
    };
static constexpr temp other_q = q==temp::meta?temp::inst:temp::meta;
    bool operator==(param<other_q>& rhs){ if(t==ty::Type){ return (t==rhs.t)and (rhs.val<=pri::get<type(rhs.arg)) and (pack==t.pack);}
        return t==rhs.t and (pack==t.pack) };
    bool operator==(param<q>& rhs){
        if(t==ty::Type){ return (t==rhs.t)and (pri::get<type>(arg)==pri::get<type(rhs.arg)) and (pack==t.pack);}
        return t==rhs.t and (pack==t.pack) };
    
    param<q>& operator=(param<temp::meta>& rhs);
    void operator=(expr<q>& r ){
        if constexpr(q==temp::inst){if(t==ty::Type){pri::get<expr<q>>(varg)=r;return;}}
        std::string strs="Expr not assignable for";strs+=str();strs+= "parameter";
         throw ValueError(str);
    };

    bool operator<(param<temp::meta>& prm){
        if constexpr(q==temp::inst){

        }else {return prm}
    };
    bool operator<(param<temp::inst>& prm){
        if constexpr(q==temp::inst){return *this==prm;}else {return prm==name;}
    };

    
    void getName(ast& a){
        if constexpr(q== temp::meta){
            tp.getName(a);if(t==ty::PtrToMember){memberList.getName(a);}
        }
        else {vl.getName();}
    };
    std::ofstream& operator<<(std::ofstream& of){
         if(Template){of<<"template";}
        if constexpr (q==temp::meta){
           
            of<<tp<<" "<<memberList<<name;
        };
    };

    void resolve(ast& a){
        if constexpr(q==temp::meta){
            tp.resolve(a);
            if(t==ty::Type){tp.resolve(a);if(tp.back().r==result::rConcept){;}}
            if(t==ty::PtrToMember){tp.resolve(a);memberList.resolve(a);}
            else if(t==ty::Template){args.resolve(a);}
        }
        else {vl.resolve(a);}
    }
    param(type& _Ty) {ptr=&_Ty;if(q==temp::meta){
        if(ty.Template){t=ty::Template;}
        else{t=ty::Type;}}
    }

    param(std::string str,ty _t) :name(str) t(_t) {};
    template <temp Q>
    param(std::string str,type& tp) : name(str) { pri::get<type>(arg)=tp;};
    template <temp Q>
    param(type& tp) : name(str) { pri::get<type>(arg)=tp;name=getName<Q>(tp);};

    void update(param_list<temp::meta>& old , param_list<temp::meta>& cur);

param(uint ref) {*this=param(_UInt);construct_type(&_UInt,ref);}
param(int ref) {*this=param(_Float);construct_type(&_Float,ref);}
param(float ref) {*this=param(_Int);construct_type(&_Int,ref);}
param(bool ref) {*this=param(_Bool);construct_type(&_Bool,ref);}
param() = default ;

};

param<temp::inst> param<temp::inst>::operator==(param<temp::meta>& rhs){this->name=rhs.name;this->ptrb=true; this->ptr=&rhs;return *this;};

template <temp q>
struct param_list : public pri::deque<param<q>> {
        void update(param_list<temp::meta>& old , param_list<temp::meta>& cur);

    param<q>* findByName(std::string name){for(param<q>& it : *this){if(name==it.name){return &it;}}
    throw MemberNotFound<param_list<q>>("In Param List");
}  
} ;

void param<temp::inst>::update(param_list<temp::meta>& old , param_list<temp::meta>& cur){
    auto oit = old.begin();auto cit=cur.begin();
    for(;oit!=old.end();){
        if(*oit==*this){*this=*cit;return;}
        ++oit;++cit;
    }

};
void param_list<temp::inst>::update(param_list<temp::meta>& old , param_list<temp::meta>& cur){for(param<temp::inst>& it : *this){
    if(it.ptrb){it.update(old,cur);};}
};


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
accSpec getAcc(stmt& st);

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
type getName(std::string name,param_list<q>& plist){
    for(param<q> it : plist){if(it.name==name){
        if(it.t=param::type::Typename){
            return pri:get<type(it.arg);
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
    bool is(type& ref){};
    // integralT(type)
};


struct attrib{
    using argList = pri::deque<epxr>;
    std::string name;std::string nsacc;bool Ns=false;
    stmt::init_args e;bool pack=true;
    void (*ptr)(attrib& at,stmt::stmtty& sttys,stmt::allvar& v);
    template <typename StmtT>
    void procAttrib(StmtT& s){stmt::allvarptr v;pri::get<StmtT>(v)=&s;
        for(attrib it : attribs){
            if(it.name==name){ptr = it.func(stmt::getTyf<StmtT>(),v);return;}
        }
        err::e(AttribNotFound(*this));
    };
    attrib(std::string str) : name(str){};
};
struct attrib_list : public pri::deque<attrib>{
    std::string nsUse;bool useNs;
};
struct lambda ;

struct NoArgsExc;
struct value {
    enum ty {anyvalue,
        prvalue=0b100 // operator expressions
        xvalue=0b01,member,ptrmember,swizzle,arr // member.access,swizzle,arr[]
        lvalue=0b10,varRef,nsmember,thisRef,funcCall,literal,initlist // variable name , function call,literal
        rvalue, // prvalue or xvalue // If set to this that means we have paren;
        glvalue=0b11,// xvalue or lvalue
        typeValue,DeclT,lambdav,funcPtr
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
    
    type* tp;
    dtype dt;
    int ref;
    bool isPtr(){return ref>0;} 

    using init_list = stmt::init_args;
    using variable =stmt::StmtVarDecl*;
    struct literal {
        pri::variant<float,uint,int,bool,std::string> lit;enum ty{FltLit,UintLit,IntLit,BoolLit,StrLit,CharLit};ty t;
        template <typename T> // op::ty or type*
        stmt::Operator* findOperator(T& o){
            switch(t) {
                case ty::FltLit : {return &get_FltOperator(o);}
                case ty::UintLit : {return &get_UintOperator(o);}
                case ty::IntLit : {return &get_IntOperator(o);}
            }
        };
        literal(char num):t(ty::CharLit) {pri::get<char>(lit)=num;} 
        literal(std::string num):t(ty::StrLit) {pri::get<std::string>(lit)=num;}         
        literal(float num):t(ty::FltLit) {pri::get<float>(lit)=num;} 
        literal(uint num):t(ty::UintLit) {pri::get<uint>(lit)=num;}
        literal(int num):t(ty::IntLit) {pri::get<int>(lit)=num;}
    };



    #ifdef CXX_C
    using ty = pri::variant<bool,int,uint,float,std::string,char>;
    #elif
    using ty = pri::variant<bool,int,uint,float,std::string>;
    #endif
    using valty=typename pri::variant<accMember_seq,literal,lambda,init_list,expr,stmt::FuncDef*>;
    valty val;
    enum truTy {eaccList,eptrMember,eliteral,eExpr,elambda,einit_list,func};
    tryTy tt;
    
        template <truTy tty>
        void set(accMember_seq& s);
    template <>void setFuncDef<truTy::func>(accMember_seq& s){
        pri::get<stmt::FuncDef*>(val) = new pri::get<stmt::FuncDecl>(s.back().inst).get(s.back().plist);
        pri::get<stmt::FuncDef*>(val) = new pri::get<stmt::FuncDecl>(s.back().inst).get(s.back().plist);

    };


    template <truTy tty>
    void resolute();
    template <> void resolute<truTy::eaccList>(){
        auto it = pri::get<accMember_seq>(val).tail();
        if(it->args.empty()){
            --it;
            switch(it->acc){
                case lex::ty::dot : {t=ty::member;}
                case lex::ty::dcolon : {t=ty::nsmember;}
                case lex::ty::arrow : {t=ty::member;}
                case lex::ty::
            }
        }
        else {t=ty::funcCall;}

    };
    template<> void resolute<truTy::eliteral>(){t=ty::literal;}

    template <truTy... tts>
    struct trut {
        template <truTy  T>
        void applyFunc()
        
        void _sw(value& vl,)
        void sw(){

        }
    };
    template <typename truT , typename RT,typename... args>
    void applyFunc(RT (*f)(args...),args... as){
        switch(tt){
            case truTy::eaccList : {pri::get<accMember_seq>(res)}
        }
    };

    template <typename T>
    stmt::Operator* findOperator(T& op){
        try {
            switch(tt){
                case truTy::eaccList : {pri::get<accMember_seq>(val).findOperator(op);}
                case truTy::eliteral : {pri::get<literal>(val).findOperator(op);}
                case truTy::eExpr : {pri::get<expr>(val).ti->findOperator(op);}
                case truTy::elambda : {pri::get<lambda>(val).findOperator(op);}
                case truTy::einit_list : {throw OperatorNotFound();}
                case truTy::func : {return pri::get<stmt::FuncDecl*>(val).Def; }
            }


        }
    };
    bool hadOperator(op::ty op, stmt::init_args& args );

    void resolve(ast& a ){}
    void resolve(ast& a){
        switch(tt){
            case truTy::eExpr : {pri::get<expr>(val).resolve(a);pri::get<expr>(val).valueResolve(*this);}
            case truTy::einit_list : {pri::get<init_list>(val).resolve(a);}
            case truTy::eliteral : {resolute<truTy::eliteral>();}
            case truTy::eptrMember : {pri::get<accMember_seq>(val).resolve(a);break}
            case truTy::efuncCall : { auto& i=pri::get<accMember_seq>(val).back() ;  pri::get<stmt::FuncDef>(val)= pri::get<Stmt::FuncDecl*>(i.inst).get(i.plist);}
            case truTy::eaccList :{pri::get<accMember_seq>(val).resolve(a);resolute<eaccList>();break;}
        }   
    }
    void addArgs(stmt::arg_list&& argl){
        if(tt!=truTy::eaccList or (pri::get<accMember_seq>(val).back().args.empty())){
            throw NoArgsExc();
        }
        else pri::get<accMember_seq>(val).back().args = argl;
    };
    template <ty tp>void set(){t=tp;};
    param<temp::inst> get_prm(){param<temp::inst> res;}
    int ptrNum=0;int refNum=0;
    template <typename T>
    void setTruT();
    template <> setTruT<stmt::FuncDecl>(){tt=truTy::func;}
    template <> setTruT<accMember_seq>(){tt=truTy::eaccList;}
    template <> setTruT<ptrMember<q>>(){tt=truTy::eptrMember;}
    template <> setTruT<funcCall>(){tt=truTy::efuncCall;}
    template <> setTruT<lambda<q>>(){tt=truTy::elambda;}
    template <> setTruT<init_list>(){tt=truTy::einit_list;}
    template <typename T>
    value(T&& v,ty p){pri::get<T>(val)=v;t=p;setTruT<T>();}
    value(T&& v,ty p,truTy pt){pri::get<T>(val)=v;t=p;tt=pt;}
    value(type& _tp){tp=&_tp;}
    value(type& _tp,ty _t){tp=&_tp;t=_t;}
    
    template <typename T>
    value(T v){t=ty::literal;tt=truTy::eliteral;
        if constexpr (std::is_same<T,float>::value){tp=&_Float;pri::get<literal>(val)=literal(num);}
        if constexpr (std::is_same<T,uint>::value){tp=&_UInt;pri::get<literal>(val)=literal(num);}
        if constexpr (std::is_same<T,bool>::value){tp=&_Bool;pri::get<literal>(val)=literal(num);}
        if constexpr (std::is_same<T,char>::value){tp=&_Char;pri::get<literal>(val)=literal(num);}
        if constexpr (std::is_same<T,std::string>::value){tp=&_Str;pri::get<literal>(val)=literal(num);}
    };
    value(expr<q>&& e) : {t=ty::rvalue;val=e;}  
};


FuncDef* findOperator(ast& a, op::ty& o , value& vl, value& vr);
template <bool pre>
FuncDef* findOperator(ast& a, op::ty& o , value& vl);
struct expr {
    posit pos;
    attrib_list atlist;
    
    bool isConstExpr();
    bool isConstExpr(value& vl);
    struct node {
        enum ConstVal {ConstExpr,Const,no,value,unknown}; ConstVal cval=ConstVal::unknown;
        enum opty {binary,ternary,unary};
        op::ty o=op::ty::none;            opty t;
        op::ty prefix=op::ty::none;op::ty postfix=op::ty::none;
        value val;bool valset=false;
        stmt::init_args args;bool index=false;

        stmt::FuncDef* pref;
        stmt::FuncDef* postf;
        stmt::FuncDef* binaryf;


        bool cexpr(){
            for(cval::ConstVal::unknown){
                switch (val.tt){
                    case value::truTy::eaccList {
                        accMember_seq& s= pri::get<accMember_seq>(val.val) ; 
                        switch(s.r){
                            case result::rVar : {auto res = pri::get<stmt::VarDecl* >(s.inst);  
                                if(res->getQual<qConstExpr>()){
                                    if(res->DefaultValue.isConstExpr())
                                    cval==ConstVal::ConstExpr;}
                                
                            }
                        }
                    }
                }
            }
            
            if(cval==ConstVal::ConstExpr ){return true;}
        };
        bool TrailOp(){return pri::OneOf<opty, opty::binary,opty::ternary>(t);}        
        bool ternary(){return t==opty::ternary;}
        bool par =false; 
        void parens(){par=true;}
        bool P(){return par;}
        node() = default;
        node(node& arg) {*this=arg;}
        node(value&& vt) : val(vt){o=op::ty::none;valset=true;}; 
        node(expr& e)  {val=value(e);valset=true;};
        node(expr&& e)  {val=value(std::forward<expr>(e));valset=true;};
    
    };
    template <op::ty tyOp,node::opty Op>
    
    type* ti ;
    using ExprTy = pri::deque<node>;
    ExprTy e;


    
        template <bool _preb,bool _postb> struct btype {static constexpr ternary=false;static constexpr bool pre = _preb;static constexpr bool post = _postb;static constexpr bool binary=false;} 
        struct ternary { static constexpr ternary=true;static constexpr bool pre = false;static constexpr bool post=false;  static constexpr bool binary=false;}
        template <op::ty o > struct fix  {static constexpr ternary=false;static constexpr bool pre = false;static constexpr bool post=false;  static constexpr bool binary=true;}

        template <> struct fix<op::ty::Not> : btype{};
        template <> struct fix<op::ty::opbnot> : btype{};
        template <> struct fix<op::ty::opp> : btype{};
        template <> struct fix<op::ty::omm> : btype{};
        template <> struct fix<op::ty::oppack> : btype{};
        template <> struct fix<op::ty::opcond> :ternary {};

    bool isConstExpr();
    bool isConstExpr(value& vl){
        switch (vl.tt){
            case value::truTy::eaccList: {
                accMember_seq* p = pri::get<accMember_seq>(vl.val);
                for(accMember& it : *p){
                    if(!it.args.empty()){
                        for( expr& ite : it.args){if(ite.isConstExpr()==false){return false;}} 
                    };
                    switch(it.r){
                        case result::rEnumMem : {return true;}
                        case result::rVar : {stmt::VarDecl& v = pri::get<stmt::VarDecl>(it.inst);return(v.Const and !v.Volatile);}
                        case result::rTypeDef : {return true;}
                        case result::rUsing : {return true;}
                        case result::rNS : {return true;}
                        case result::rDeclType : {return true;}
                    };
                    return true;
                }
            };
            case value::truTy::eptrMember:{return true;} 
            case value::truTy::eliteral: {return true;}
            case value::truTy::eExpr : { return pri::get<expr>(vl.val).isConstExpr();}
            case value::truTy::elambda: {return true;}            if(it.prefix!=op::ty::none){v.t==value::ty::prvalue;}

            case value::truTy::einit_list:{
                for(expr ite : pri::get<value::init_list>(vl.val) ){
                    if(ite.isConstExpr()==false){return false;}
                }
                return true;} 
            case value::truTy::func: {return true;}
        } ;
        
    };
    void valueResolve(value& v){
        for(auto& it : e){
            if(it.prefix!=op::ty::none){v.t==value::ty::prvalue;return;}
            if(it.postfix!=op::ty::none){v.t==value::ty::prvalue;return;}
            if(it.o!=op::ty::none){v.t==value::ty::prvalue;return;}
        };
    };  
    void isConstExpr(){
        for(node it : e){if(!e.val.isConstExpr()){return false;}}
        return true;
    };
    template <bool funcCall>
    void addArgs(stmt::init_args&& argl){
        try {e.back().val.addArgs(std::forward<stmt::init_args>(argl))}
        catch (const NoArgsExc& e){e.back().args=argl;}
        e.back().index=!funcCall;
    };
    value& val(){return val;}
        template <op::ty OpT,node::opty cur=node::opty::binary>
        void add(){e.back().opTy=cur;e.back().o=OpT;};
        template <op::ty OpT> add<OpT,node::opty::prefixUnary>(){e.back().prefix=opT;e.back().Prefix=true;}
        template <op::ty OpT> add<OpT,node::opty::prefixUnary>(){e.back().postfix=opT;e.back().Postfix=true;}


        template <op::ty OpT>
        void push(){   
            if constexpr (fix<OpT>::binary ){
                if(e.back().o==op::ty::none){add<OpT,node::opty::binary>();}
                else{e.push_back(node());add<OpT,node::opty::binary>();}
            }
            else if constexpr (fix<OpT>::ternary ){
                if(e.back().o==op::ty::none){add<OpT,node::opty::ternary>();}
                else{e.push_back(node());add<OpT,node::opty::ternary>();}
            }
            else if constexpr (fix<OpT>::pre ){
                if constexpr (!fix<OpT>::post){if(e.back().prefix==op::ty::none and (e.back().valset)){e.back().prefix=OpT;return;}; }
            };
            if constexpr (fix<OpT>::post){
                if constexpr (!fix<OpT>::pre){if(e.back().postfix==op::ty::none)}{e.back().postfix=OpT;}
            }
            else {err::e(ThrowExpressionParse(*this));}
        };

    template <op::ty Opt=op::ty::ocall>
    void pushExpr(expr&& e){
        e.emplace_back(e);
        if constexpr (OpT==op::ty::ocall){e.back().parens();}
    };
    void emplace(value<q>&& args){
        if(e.back().o==none){e.back().val=args;}
        else e.emplace_back(args);
    };
    void emplace(expr<q>&& args){
         if(e.back().o==none){e.back().val=value<q>(args);}
        else e.emplace_back(args);
    };
    void addFuncCall(){};
    template <typename T>
    void pushLiteral(T val){
        if(e.back().o!=op::ty::none and !e.back().valset){
            e.back().val=value(val);
        }else e.emplace_back(value(val));
    }
    type getType(){return tree.val.getType()};
    operator type(){return getType();}
    expr Not(){
        expr<q> res;
        res.tree.opT=node::opty::prefixUnary;
        res.tree.o=op::ty::opNot;
        res.tree.rhs=new node(tree);
        return res;
    };

    operator bool(){
        if(e.vt==value::truTy::eliteral and (pri::get<value::literal>(e.val.val).t==value::literal::ty::BoolLit) ){return pri::get<bool>((pri::get<value::literal>(e.val.val).lit)); }
        else throw ExprTypeError();
    }
    bool cexpr(){return e.cexpr();}
    bool ceval(){return e.ceval();}


    
    void resolve(ast& a){
        auto it = e.begin();
        auto lit = it;lit.resolve(a);

        
        if(lit.prefix!=op::ty::none){stmt::FuncDef& ores= findOperator<true>(a,let->o,lit->val);}
        if(lit.postfix!=op::ty::none){stmt::FuncDef& ores= findOperator<false>(a,let->o,lit->val);}
        for( ++it; it!=e.end();++it){
            if(lit->o==op::ty::none){
                if(lis->val.t==value::ty::literal){
                    if(it->val.tt = value::truTy::eaccList){
                        accMember_seq& res = pri::get<accMember_seq>(it->val.val) ; 
                        if(res.front().name[0]=='_'){
                            try {lit->postF=findOperatorLit(a,res.front().name);}
                            catch (const OperatorNotFound& e){}
                        }

                    }
                }
                else throw ExprOper(*this,lit);}

                it->resolve(a);lit->binaryF= findOperator(a,let->o,lit->val,it->val);
            
            lit=it;
        };
    };
    
    
    expr(bool s) : type(ty::literal) {bvalue=s;}
    expr(uint s) : type(ty::literal) {};
    expr(float flt) : type(ty::literal) {};
    expr(int s) : type(ty::litreal) {};
    expr(posit _pos) : pos(_pos) {}
};

struct lambda {

    stmt::arg_list args;
    struct  capture {
        bool byRef;
        value<q> vl;
        capture(bool br,value<q>& value) : byRef(br), vl(value);
    } ;
    struct captureList : public pri::deque<value>{bool byCopy=false;
        void push(bool byr,value& vl){
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
    using argList = stmt::arg_list;
    using tyty= std::conditional<temp::meta==q,value<q>,type>::type
    tyty rettp;bool trailing=false;
    attrib_list attribs;
    stmt::block body;

};
enum accSpec{Public=3,Private=2,Protected=1};
struct type {
    static constexpr temp tempState = q;
    bool Template ;
    enum ty {//constructor=0,expr=1,
        vec=0,mat=1,Buffer=3,strct=4,arr=5,
        Image2D=6,Image3D=7,Sampler=8,
        // Param,Ns,Alias,enum,
        // // Vec=24,Mat=25,Ivec=26,Imat=27,Uvec=28,Umat=29,Bvec=30,Bmat=31
    };
    enum prim {Void=9,Float=10,Int=11,Uint=12,Bool=13,pod,ctype}
    template <prim PrimT>struct primType {using type =void;}
    template <>struct primType<prim::Float> {using type =float;};
    template <>struct primType<prim::Int> {using type =int;};
    template <>struct primType<prim::Uint> {using type =uint;};
    template <>struct primType<prim::Bool> {using type =bool;};

        
    std::string name ; 
    param_list<q> prms;
    std::enable_if<q==temp::meta,bool>::type tempTy;
    std::enable_if<q==temp::meta,pri::deque<type>::type insts;
    ty t;
    // prim pt;
    // accSpec acc=accSpec::public;
    struct spec {
        type q;
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
    struct inher  {
        accSpec acc;
        using tyty  = typename std::conditional<temp::meta==q,value<q>::accMember_seq,type*>::type;
        tyty t;
        inher(accSpec spc) : acc(spc);
        inher(tyty r) : t(r){} 
    };

    accSpec curacc;
    template <typename T> struct accMem { accSpec acc ; T data;
        template <typename... Ts>
        accMem(accSpec acc,Ts... args) :  acc(_acc) { data = T(args...); }
        accMem(accSpec& _acc,T&& d ) : acc(_acc) , data(d) {} 
        accMem(accSpec& _acc,T& d ) : acc(_acc) , data(d) {} 
    };
    template <typename T>
    using  acc_list : public pri::list<accMem<T>> ;

        using inher_list=acc_list<tyty>;      
        acc_list<tyty> inherits;
        pri::deque<stmt::DeclType*> frndsTypes;// DeclType
        pri::deque<stmt::FuncDecl*> frndsFunctions; // DeclFunction
        pri::deque<stmt::OperatorDecl*> frndsOperators; // DeclFunction
    
        acc_list<stmt::Using> usings;// Usings stmts
        acc_list<stmt::TypeDef> tdefs;// 
        
        acc_list<stmt::DeclType> types;// 

        pri::list<accMem<stmt::DeclType>*> anons;

        acc_list<stmt::Enum> Enums; 
        
        acc_list<stmt::VarDecl> variables;
        
        acc_list<stmt::FuncDecl> methods;// Has type FuncDecl
        acc_list<stmt::Operator> operators;
        
        acc_list<stmt::Constructor> constructors; 
        stmt::block destructor;
        
        acc_list<stmt::Concept> concepts ;
        template <temp Q>
        static void _procInherits();
        template<> static void _procInherits<temp::inst>(){

        };
        template<> static void _procInherits<temp::meta>(){};

        static void procInherits(){inline _procInherits<q>();}

        stmt::VarDecl getInst(arg_list argl){
            VarDecl vdecl ; 
        };

        
        bool isPod(){   
            if(!constructors.empty() or !destructors.empty()){return false;}
            for(stmt::FuncDecl* it :methods){if(it->Virtual){return false;}}
            else return true;
        };

    template <typename T ,pri::acc_list<T> type::* ptr >
    accMem<T>& find(std::string name ){
        for(T& it : this->*ptr ){if(it->data.name ==name){return &it;}}
        throw NameNotFound<T>;
    };
    
     void find(std::string name,result* r,resty* res){
            try { pri::get<VarDecl*>(*res)=find<VarDecl,&type::variables>(name);*r=result::rVar;return;} catch (const NameNotFound<VarDecl>& e){}
            try { pri::get<FuncDecl*>(*res)=find<FuncDecl,&type::methods>(name);*r=result::rFunc;return;} catch (const NameNotFound<FuncDecl>& e){}
            try { pri::get<TypeDecl*>(*res)=find<TypeDecl,&type::types>(name);*r=result::rType;return;}         catch (const NameNotFound<TypeDecl>& e){}
            try { pri::get<Enum*>(*res)=find<Enum,&type::Enums>(name);*r=result::rEnum;return;}           catch (const NameNotFound<Enum>& e){}
            try { pri::get<Using*>(*res)=find<Using,&type::usings>(name);*r=result::rUsing;return;}        catch (const NameNotFound<Using>& e){}
            try { pri::get<TypeDef*>(*res)=find<Union,&NS::tdefs>(name);*r=result::rTypeDef;return;}            catch (const NameNotFound<TypeDef>& e){}
            #ifdef CXX_C
            try { pri::get<Union*>(*res)=find<Union,&type::unions>(name);*r=result::rUnion;return;}        catch (const NameNotFound<Union>& e){}
            #endif
                        throw NameNotFound();
    }


    template <typename T>
    struct ptrmem ;
    template <>struct ptrmem<stmt::DeclType> {static constexpr pri::acc_list<stmt::DeclType> type::* ptr=&type::types ;}
    template <>struct ptrmem<stmt::VarDecl> {static constexpr pri::acc_list<stmt::VarDecl> type::* ptr=&type::vars ;}
    template <>struct ptrmem<stmt::Using> {static constexpr pri::acc_list<stmt::Using> type::* ptr=&type::variables ;}
    template <>struct ptrmem<stmt::FuncDecl> {static constexpr pri::acc_list<stmt::FuncDecl> type::* ptr=&type::methods ;}
    template <>struct ptrmem<stmt::Enum> {static constexpr pri::acc_list<stmt::Enum> type::* ptr=&type::Enums ;}
    template <>struct ptrmem<stmt::TypeDef> {static constexpr pri::acc_list<stmt::TypeDef> type::* ptr=&type::tdefs ;}
    template <>struct ptrmem<stmt::OperatorDecl> {static constexpr pri::acc_list<stmt::OperatorDecl> type::* ptr=&type::operators ;}
    template <>struct ptrmem<stmt::Concept> {static constexpr pri::acc_list<stmt::Concept> type::* ptr=&type::concepts ;}
    
    
    template <typename T>
    accMem<T>& findWhat(std::string name){
        for( accMem<T>&  it: this->*ptrmem<T>::ptr ){
            if(name == it.name){return it;}
        }; throw NameNotFound();
    };
        
    void find(std::string name,result* r,resty* res ){
        if(!anons.empty()){
         for(stmt::DeclType* it : anons){try{it->find(name,r,res)} catch(const NameNotFound& e){continue;};return;};
        };
        try{pri::get<stmt::DeclType*>(*res) = findWhat<stmt::DeclType>(name);return;}catch(const NameNotFound& e){}; 
        try{pri::get<stmt::VarDecl*>(*res) = findWhat<stmt::VarDecl>(name);return;}catch(const NameNotFound& e){}; 
        try{pri::get<stmt::Using*>(*res) = findWhat<stmt::Using>(name);return;}catch(const NameNotFound& e){}; 
        try{pri::get<stmt::FuncDecl*>(*res) = findWhat<stmt::FuncDecl>(name);return;}catch(const NameNotFound& e){}; 
        try{pri::get<stmt::Enum*>(*res) = findWhat<stmt::Enum>(name);return;}catch(const NameNotFound& e){}; 
        try{pri::get<stmt::TypeDef*>(*res) = findWhat<stmt::TypeDef>(name);return;}catch(const NameNotFound& e){}; 
        throw NameNotFound()
    };

    template <typename T,acc_list<T> type::* ptr, accSpec at,typename... Ts>
    T* find(std::functional<bool(accMem<T>& it,Ts...)>&& func,Ts... args){
        for(accMem<T>& it : this->*ptr){
            if(func(args...)){
                if(at<it.acc){throw AccSpec(at,it.acc,*this,it.data);}
                return &it.data;
            }
        };
        throw OperatorNotFound();

    };

    template <accSpec at = accSpec::Public>
    stmt::Operator* findOperator(op::ty o,std::string& name){
        find<stmt::Operator*,&type::operators,at>([&](accMem<stmt::Operator> it,op::ty& o , std::string& name) -> bool {
            if(it.data.name==name and (it.data.o==o)){return true};return false},o,name;
        );
        throw OperatorNotFound();
    };
    template <accSpec at = accSpec::Public>
    stmt::Operator* findOperator(op::ty o){
        for(accMem<Operator>& it : operators){
                                        if(at<it.acc){throw AccSpec(at,it.acc,&it);}

            if(it.data.opt == o){return &it;}
        };
        throw OperatorNotFound();
    };
    template <accSpec at = accSpec::Public>
    stmt::Operator* findOperatorLit(std::string name){
        for(accMem<Operator>& it : operators){
            if(it.data.opt == op::ty::opdq and (it.data.name=name);){return &it;}
        };
        throw OperatorNotFound();
    };
    template <accSpec at = accSpec::Public>
    FuncDef* findOperator(type* q){
        stmt::Operator* op = findOperator(op::ty::opType);
        for(stmt::Operator::specN& it : op->specs){
            if(it.t.args.front().tpi){return &(it.t);}
        }
        throw OperatorNotFound();
    };
       template <bool pre,accSpec at=accSpec::Public>
        FuncDef* findOperator(op::ty& o,type& tp){
            Operator* r = findOperator(o);
            for(Operator::SpecN& it : r->specs){
                if constexpr (pre){if(it.t.args.empty()){return it.t ;}}
                else if(!it.t.args.empty()){return it.t ;}
            }
            throw OperatorNotFound(o);
        };
            template <accSpec at = accSpec::Public>
        FuncDef* findOperator(op::ty& o,init_args& args){

        };

        template <accSpec at = accSpec::Public>
        FuncDef* findOperator(op::ty& o,type& tl,type& tr){
            Operator* r = findOperator(o);
            for(Operator::SpecN& it : r->specs){
                if(it.t.args.size()==2){
                    auto& i  =it.t.args.begin();
                    if(*(i.tpi) != tl ){break;}
                    ++i;
                    if(*(i.tpi) != tr ){break;}
                    return it.t;
                }
            }
            throw OperatorNotFound(o);
        };


    FuncDecl* findFunc(std::string name ){accMem<T>& r =  findWhat<FuncDecl>(name);return &r.data;}


    template <temp Q>
    stmt::OperatorDecl* findOperator(op::ty op,arg_list args,param_list<Q>& prms,type* tyt);
    template <>
    stmt::OperatorDecl* findOperator<temp::inst>(op::ty op,arg_list args,param_list<Q>& prms,type* tyt){
        for(stmt::OperatorDecl it : operators){if(it->typeConv and (tyt== it.opv)){return &it;} };
    };

    template <temp Q>
    ConstructorDecl* findConstructors(op::ty op,arg_list args,param_list<Q>& prms) ;
        template <>
    ConstructorDecl* findConstructors<temp::ins>(op::ty op,arg_list args,param_list<Q>& prms) {

    };

    template <typename T>
    void push(T&& p){this->*ptrmem<T>::ptr.push_back(p);};

    
    void pushConstructor(stmt::ConstructorDecl&& c ){constructors.push_back(c);}
    void pushDesctructors(stmt::ConstructorDecl&& c ){destructors.push_back(c);}


    bool exists(std::string name,result* r = result::rErr,resty* res=nullptr){
        try {find(name,r,res);} catch (const  NameNotFound& e){return false}
        return true;
    };


    bool isFuncObj(){
        for(accMem<Operator>& it : operators){
            if(  (it->data.opt == op::ty::ocall )){return true;}
        };return false;
    };
    
    size_t alignment=1;size_t size;
    void calcAlignment(){alignment=1;
        for(type it : ts  ){it.calcAlignment();if(it.alignment>alignment){alignment=it.alignment;}}};
    void calcSize(){size=0;
        for(type it : ts  ){it.calcAlignment();size+=it.size;}};};


    template <temp Qr,temp Qs>
    type get(parameter_list<Qs>& plist){
        for(spec& it : this->specs){
            if(it==plist){return it;}
        };
        for(spec& it : this->specs){
            if(it<=plist){return it;}
        };
    };

    template <bool Name>
    bool approxEq(type& s){
        if(t != s.t){return false;}
        if(t>=24){if(dims!=s.dims){return false;}}
        if constexpr(Name){
            if(s.name != name){return false;}
        };
        return true;
    };
    bool tyEq(type& s){
            if(ts.size()!= s.ts.size()){return false;}
        pri::list<type>::iter tit = ts.begin();
        pri::list<type>::iter sit = s.ts.begin();
        while(tit!=ts.end() and sit != s.ts.end() ){
            if(*tit != *sit){return false;}
            ++tit;++sit;
        };
        return true;
    };    
    bool operator<=(type& s){return approxEq<true>(s)}
    bool operator<(type& s){
        if(!approxEq<true>(s)){return false;}
        return ts.size()<s.ts.size();
    };
    bool operator>=(type& s){return approxEq<false>(s)}
    bool operator>(type& s){
        if(!approxEq<false>(s)){return false;}
        return ts.size()<s.ts.size();
    };
    bool operator==(type& s){
        if(!approxEq<true>(s)){return false;}
        return tyEq(s);
    };
    bool operator!=(type& s){
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
    type get(param_list<Qp>& plist){
        
        if(Qp==temp::meta){
            if(Qp==temp::meta)
            type h ;h.prms=plist;
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
    
    type(std::initializer_list<stmt::VarDecl>& inList){

    };
    
    constexpr type(std::string str,stmt::DeclType* dt, param_list<temp::inst>& plist ){
        *this = dt->get(plist);this->name = str;
    };
    type(std::string str, prim _tp) : name(str) , pt(_tp)  {}
    type(std::string str, ty _tp) : name(str) , t(tp)  {}


    type(type __ty,pri::deque<size_t> _dims){*this=__ty;dims=_dims;}

    type(std::string _name,prim _ty,param_list<q> _prms) :t(_t) {prms=_prms;tempTy=true;pt=_ty;};
    type(std::string _name,ty _ty,param_list<q> _prms) :t(_t) {prms=_prms;tempTy=true;t=_ty;};  

    template <temp Qt,temp Qp>
    type(std::string _name,type _t,param_list<Qp> _prms) :t(_t) {*this=_t.get(_prms);name=_name;};


size_t dim;pri::deque<size_t> dims;

    struct swizzleVec {
        size_t s ; size_t* inds;
        swizzleVec(size_t _s, size_t* _inds){s=s;inds=_inds}
    };
    // operator. for swizlle proto : operator.(std::string n){}
    swizzleVec swizzle(size_t s , size_t* inds){

    };
    swizzle
    bool hasSwizzle(){}
    // bool hasSwizzle(){if(t==ty::vec or(t==ty::mat)){return true;}else return false;}
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
  
     swizz(std::string sw){
        if(sw.size()>dim){throw SwizzleError("Swizzle ")};
        std::vector<size_t> res;
        for(char c : sw){res.push_back(swizz(c));}
        return res;
    };
    size_t size;
    std::vector<size_t > swi;
    bool operator==(type& f){
        if(s.size()<=f.dim)
    };
     
    swizzleTy(std::string s,type& i){
        for(char c : s){
            size_t h = siwzz(c);
            if(h!=swiz_err){
                if(h<=i.dim){swi.push_back(h);}
                else{err::e<err::t::swizzle_fortype>()}
            }
            else{err::e<err::t::swizzle_notexist>()}
        };
    };
    template <size_t s, size_t... Ss>
    void pushSize(size_t s,size_t... Ss){
        dims.push_back(s);
        if(sizeof...(Ss)>0){pushSize(Ss);}
    }

    type(std::string str,size_t size) : t(ty::vec) name(str)  {dim=size;}
    template <size_t... Ss> 
    type(std::string str,type& arrtemp,type* ty,size_t ss,size_t... Ss) :t(ty::vec) dim(arrtemp=dim)  {*this=*ty;pushSize(ss,Ss...);}
    type(std::string str,size_t ss) : dim(ss) t(ty::vec) {name=str;dim=ss;}
};


template <temp q>
struct tyty {
    std::conditional<q==temp::meta,accMember_seq>* t;
    size_t refN;size_t ptrN;
    bool fixArr ; 
};

template <temp Q>
std::string getName(type& tp){return tp.name;}


stmt::tyty getTy(stmt::tyty& r){};


stmt::block getBlock(stmt::block& r){
    stmt::block res;
    for(stmt& rhs: r){res.push_back(getNode(rhs));}
    return res;
};
template <> stmt getNode<stmt>(stmt& l){
    switch(l.t){

    };
};


template <temp q>
bool operator==(stmt::arg_list& argsl, pri::deque<expr<q>>& argsr){
    stmt::arg_list::iter itl = argsl.begin();
    pri::deque<expr<q>>::iter itr= argsr.begin();
    for(;itl!=argsl.end() and (itr!=argsr.end);++itl){
        if(*itr!=*itl){return false;}
        ++itr;
    };
} ;
template <temp q>
accSpec getAcc(stmt&  st){return st.acc;}


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
bool ConstructorAvail(type lhs,value<temp::inst>& vl,stmt::StmtFuncDecl* res=nullptr){// Checks if there is a constructor for value
    for(stmt& pr : lhs.constructors ) {
        stmt::StmtFuncDecl& ref = pri::get<stmt::StmtFuncDecl>(pr.var);
        if(CEXPR and ref.ConstExpr or !CEXPR){
            if (ref.args.size()==1 and (vl.ty==ref.args.front().tp)){
                res=&ref;return true;}
        };
    }
    return false;
};
template <temp q>
bool operator<(type& lhs,value<temp::inst>& vl){// Checks if there is a constructor for value
    return ConstructorAvail<q,false>(lhs,vl);};
bool operator<=(type& lhs,value<temp::inst>& vl){// Same as < but with constexpr
    return ConstructorAvail<q,true>(lhs,vl);};


    template <temp q>
    StmtVarDecl& findVarName(stmt::block& b,std::string name){
        for(stmt& it : b){
            if(it.t==stmtty::VarDecl){return it;}
        };
    };
struct mStmtVersion;

    template <temp q>
    bool isPtr(result* r,resty* res){
        switch(*r){
            case result::rValue : {return pri::get<value<q>>(*res).isPtr();}
            case result::rVar : {return pri::get<stmt::StmtVarDecl*>(*res)->isPtr()}
            default :{return false;}
        }
    };
    template <temp q>
    bool isCompound(result* r,resty* res){
        switch(*r){
            case result::rValue : {return pri::get<value<q>>(*res).tp->isCompound();}
            case result::rVar : {return pri::get<stmt::StmtVarDecl*>(*res)->tp->isCompound()}
            case result::rType : {return pri::get<type*>(*res)->isCompound();}
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
            case result::rVar : {return pri::get<stmt::StmtVarDecl*>(*res)->tp->hasSwizzle()}
            default :{return false;}
        }
    };
    template <temp q>
    bool hasMember(result* r,resty* res){return isCompound<q>(r,res) || hasSwizzle<q>(r,res);};


    enum cntxt {strct,func,ns}
struct ast{
    uint version;
    pri::deque<stmt::StmtLayout> layouts;
    stmt::StmtNS global;
    
    
    
    pri::list<type> Types;
    pri::list<stmt::StmtFuncDecl> Funcs;
    
    bool nsblck=true;bool fblck;bool swBlock;bool IfChain;
    pri::deque<stmt::block*> curBl;
    pri::deque<stmt::NS*> nss;
    pri::deque<stmt::DefType*> strcts;
    pri::deque<stmt::FuncDef*> funcs;
    pri::deque<param_list<temp::meta>*> curtemp;
    param_list<temp::inst>* curSpec;
    pri::stack<stmt::StmtTry*> tries;
    stmt::FuncDef* fdef;
    type* curtp;

    stmt::FuncDef* entry_pt;bool foundEntry=false;

    stmt::block& curBlock(){return *(curBl.back());};
    param_list<q>& curParams(){return *(curtemp.back());};

    stmt::TypeDef& getTypeDef(){if(nsblck){nss.back().tdefs.back()}else{strcts.back()->tdefs.back()};}
    stmt::TypeDef& getUsing(){if(nsblck){nss.back().usings.back()}else{strcts.back()->usings.back()};}
    stmt* getStmt(){return curBl.back().back();}
    stmt::VarDecl* getRef(std::string name){
        for(stmt::VarDecl& it :  fdef->args){
            if(it.name==name){return it;}
        };
        for(stmt::VarDecl& it :  fdef->args){
            if(it.name==name){return it;}
        };

    };
    template <bool Inline=false>
    bool pushNS(std::string name){
        if constexpr (Inline)
        curBl.push_back(nss.back()->includeNS(ns).body);};
    
    void pushbl(stmt::block& bl){curBl.emplace(&bl);}
    void pushbl(stmt::block& bl,param_list<q>& pl){curBl.emplace(&bl,&pl)}
    void popbl(){
        if(curBl.back().bl==&strcts.back().body){strcts.pop_back();}
        if(curBl.back().ty==bl::ty::Template){curtemp.pop_back();}
        if(curBl.back().ty==bl::ty::ns){nss.pop_back();}
        curBl.pop();
    };
    
    template <typename StmtTy,cntxt c = cntxt::ns> StmtTy* _Push(StmtTy&& st){
        if constexpr (c==cntxt::func){funcs.back()->(stmt::FuncDef::ptrmem<StmtTy>::ptr).push_back(st);return;}
        else if constexpr (c==cntxt::strct){strcts.back()->*type::ptrmem<StmtTy>::ptr.emplace_back(strcts.back().curacc,st);return &(strcts.back()->*type::ptrmem<StmtTy>::ptr.back());}
        else {nss.back()->*(stmt::NS::ptrmem<StmtTy>::ptr).push_back(st);return &(nss.back()->*(NS::ptrmem<StmtTy>::ptr).back());}
    };
    template <template StmtTy,cntxt c>StmtTy* _pushStmt(StmtTy&& st ){curBl.back().push_back(ptr->body);return pri::get<StmtTy>(curBl.back().back().inst) ;};

    template <typename T,cntxt c> T* pushStmt(T&& st){

        if constexpr(pri::is_one_of<T,stmt::block,stmt::If,stmt::Else,stmt::Switch,stmt::Case,stmt::Default,stmt::For,stmt::While,stmt::ForRange,stmt::Do,stmt::Return,stmt::Try,stmt::Catch,stmt::Throw,stmt::Asm>::value ){
            if constexpr (c!=cntxt::func){throw StmtNotAllowed<T>();}
            return _pushStmt(st);
        }else {_Push<T,Strct,Func>(st);};
    };     
    template <typename T,typename A>
    T* pushStmtTo(A* to, T&& st){to->push(st);};
    template <typename T>
    T::specty* pushSpec(T* ptr,T::specty&& s,param_list& plist ){ptr->push(s,plist);};

    stmt& lastStmt(){return curBlock().back();}


    template <> pushStmt(stmt::StmtLayout&& ly){laoyuts.push_back(l);}
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

    type getTp(type* t, stmt::param_list pl={}){
        return t->get<temp::inst,temp::inst>(pl);
    };

    template <typename T>
    T* findWhat(accMember_seq name){
        if(strcts.empty()){return nss.back()->find<T>(name);}
        return strcts.back()->findWhat<T>(name);
    };
    template <typename W ,typename F>
    void findWhatFrom(std::string name,T* what, F* from){
        if constexpr (std::is_same<F,stmt::NS>::value){  what = from->find<T>(name,,res);}
        else if constexpr (std::is_same<F,stmt::Enum>::value){
            static_assert(std::is_same<T,Enum::member>::value,"Enum has only enum member")
            what = from->find<T>(name,r,res);}
        else if constexpr (std::is_same<F,stmt::Union>::value){what=from->find<T>(name,r,res);}
        throw NameNotFound();
    } ;
    template <typename F>
    void findFrom(std::string name,accMember& m, F* from){
        if constexpr (std::is_same<F,stmt::NS>::value){from->find(name,r,res);}
        else if constexpr (std::is_same<F,stmt::Enum>::value){*r =result::rEnumMem;pri::get<stmt::Enum::EnMember*>(res)= &from->find(name);}
        else if constexpr (std::is_same<F,stmt::Union>::value){from->find(name,r,res);}
        else if constexpr (std::is_same<F,DeclType<q>>::value){fromr->tp.find(name,r,res);}
        else if constexpr (std::is_same<F,type>::value){cur->find(name,r,res);}
        else if constexpr (std::is_same<F,stmt::Using>::value){cur}
        throw NameNotFound();
    };

    template <typename T >
    void findInNs(std::string name,result* r,resty* res){
        for(pri::deque<param<q>>& it : pri::reverse(curTemp)){
                for(param<q>& ite : it){
                if(ite->name == name){*r=rParam;pri::get<param<q>*>(*res)=&ite;return;}
            };
        }
        throw NameNotFound();
    };
    template <typname T ,typename... Args>
    void findN(accMember_seq& res);

    stmt::FuncDecl* findFunc(accMember_seq& res){
        if(strcts.empty){return nss.back().findFuncDecl(res);}
        else {return strcts.back().t.findFuncDecl(res);}
    };

    void find(accMember_seq& res){
        accMember_seq::iter last;
        accMember_seq::iter it = res.begin();
        if(res.globalAcc){
            res.push_front(accMember());res.front().r=result::rNs;
            get<stmt::NS*>(res.front().inst)=&global;last=res.begin();
        }
        else {find(res.front().name,&res.front().r,&res.front().inst);}
        for(++it;it!=res.end();++it){
            switch(last->r){
                case result::rVar : {try{
                    auto lr  =pri::get<stmt::VarDecl*>(last->inst);lr->resolve();
                    if(lr.tpi.hasSwizle()){}// TODO
                    findFrom<stmt::NS>(it->name,&it->r,&it->inst,pri::get<stmt::VarDecl*>(last->inst));};break;}
                case result::rNs : {try{findFrom<stmt::NS>(it->name,&it->r,&it->inst,pri::get<stmt::StmtNS*>(last->inst));};break;}
                case result::rEnum : {try {findFrom<stmt::rEnum>(it->name,&it->r,&it->inst,pri::get<stmt::Enum*>(last->inst))};return;}
                case result::rType : {try{findFrom<type>(it->name,&it->r,&it->inst,&(pri::get<stmt::DeclType*>(last->inst).get(last->plist);));};break;}
                case result::rUsing : {try{ pri::get<stmt::Using*>(last->inst).get(last->plist).find(it->name,&it->r,&it->inst);}; break;};
                case result::rTypeDef : {try{findFrom<stmt::TypeDef>(it->name,&it->r,&it->inst,pri::get<stmt::TypeDef*>(last->inst));}; break;};
                case result::rParam : {try{findFrom<value>(it->name,&it->r,&it->inst,pri::get<stmt::param<temp::inst>);} }
            }
            if(it->t==result::rParam){pri::get<param<temp::meta>>(it->inst).refs.push_back(&(*it));return;}
            last = it;it->resolve(*this);
        };
    };
    
    bool include(ast&& s){
        if(!s.curBl.empty()){return false;}
        this->layouts.concat(s.layouts);
        global.includeNs(s.global);
    };

    stmt::NS* findNs(bool glbl,pri::deque<std::string> nsname){
        if(glbl){
            stmt::NS* cur=&global;
            for(const std::string str : nsname ){
                cur=cur->findNs(str);
            };
            return cur;
        }   
    }
    void pushUsingNS(bool& glbl,pri::deque<std::string>& nsName){  nss.back()->useNs(findNs(glbl,nsName));};
    bool exists(accMember_seq& aclist){
        accMember_seq::iter it = aclist.begin();
        stmt::TypeDef* ptr;
        if(strcts.empty()){
            stmt::NS* pt=nss.back();
            for(it ;it!=aclist.end();++it){
                try{pt->find(it->name,&it->r,&it->inst);}
                catch(const NameNotFound& e){return false;}
                if(it.r== result::rNs) {pt=pri::get<stmt::NS*>(it.inst);continue;}
                else if(it.r== result::rType) :{ptr = pri::get<stmt::DeclType*>(it.inst);break;}
                else {}
            }
        };
        else {ptr= strcts.back();}
        for(it ;it!=aclist.end();++it){
            try{ptr->find(it->name,&it->r,&it->inst);}
            catch(const NameNotFound& e){return false;}
        }
        aclist.resolved=true;
        return true;
    };
    stmt::Operator* findOperator(op::ty& Op, type& tl, type& tr){
        if(!funcs.empty()){
            for(  stmt::FuncDef* res : pri::reverse(funcs)){
                try{stmt::Operator* r= res->findOperator(Op,tl,tr);return r;}
                catch(const OperatorNotFound& e){}
            };
        }
        if(!strcts.empty()){
            for(  stmt::FuncDef* res : pri::reverse(strcts)){
                try{stmt::Operator* r= res->findOperator(Op,tl,tr);return r;}
                catch(const OperatorNotFound& e){}
            };
        }
            for(  stmt::FuncDef* res : pri::reverse(nss)){
                try{stmt::Operator* r= res->findOperator(Op,tl,tr);return r;}
                catch(const OperatorNotFound& e){}
            };
            throw OperatorNotFound();
    };
    stmt::FuncDef* findOperatorLit(std::string name, dtype& t){stmt::Operator* r ;bool found=true;
        
        if(!funcs.empty()){
            for(  stmt::Operator* res : pri::reverse(funcs)){
                try{ r= res->findOperator(op::ty::opdq,name);break;}
                catch(const OperatorNotFound& e){found=false;}
            };
        }
        else if(!strcts.empty()){
            for(  stmt::Operator* res : pri::reverse(strcts)){
                try{ r =res->findOperator(op::ty::dq,name);break;}
                catch(const OperatorNotFound& e){found=false;}
            };
        }
        else {
            for(  stmt::Operator* res : pri::reverse(nss)){
                try{r=res->findOperator(op::ty::dq,name);break;}
                catch(const OperatorNotFound& e){found=false;}
            };
        }
        if(found){
            try{res->get(name);}
        }

        
    };
    template <bool Pre>
    stmt::Operator* findOperator(op::ty& Op, type& tl){

    };
    bool declared(accMember_seq& a){
        pri::variant<stmt::NS*, stmt::DeclType* > cur;bool strct;
        if(strcts.empty()){
            cur = findFrom(,&strct);

        };
        for(accMember& it : a){
            
        };
    };
    bool ShadowCheck(std::string& name){
        for( param_list<temp::meta>& it : pri::reverse(curtemp)){
            for(param<temp::meta>& ite  : it ){
                if(ite.name == name ){return true;}}
        };
        return false;
    };
    void procFinal(){
        if(!foundEntry){err::e(EntryPtNotFound());return;}
        entry_pt->resolve(*this);};

    void includeNS(pri::deque<std::string>& nsName){nss.back()->includeNs(nsName);};
    
    template <typename StmtT,typename... Ts>
    void emplace_back(Ts... args){push_back(StmtT(args...));};

    

    ast() {nss.push_back(&global);
        if(sys.l ==langauge::stmsl){addGlobal<language::stmsl>(*this);}
        else {addGlobal<language::cpp>(*this);}
    };
};

void accMember_seq::resolve(ast& a){
    try{a.find(*this);}
    catch (const MemberNotFound& e){err::e(*this,e);};
    catch (const NameNotFound& e){err::e(*this,e);};
    catch (const MemberNotFound<Enum>& e){err::e(*this,e);}
};

FuncDef* findOperator(ast& a, op::ty& o , value& vl, value& vr){
    a.findOperator(o,vl.tp,vl.tp)
};
template <bool pre>
FuncDef* findOperator(ast& a, op::ty& o , value& vl);a->findOperator(o,vl.t);
};
FuncDef* findOperatorLit(ast& a,std::string name){

};

#endif