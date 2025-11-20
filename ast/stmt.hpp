#ifndef STMSL_STMT
#define STMSL_STMT
#include "ast.hpp"
#include <petri/list.hpp> 
#include <type_traits>
enum AttribT {
    LayoutOnly,LayoutExcl,Every,
    ClassMember,Func,Method
};
enum qual {qExtern=0,qStatic=1,qConstExpr=2,qConst=3,qVirtual=4,qOverride=5,qExplicit=6,qFinal=7,qFriend=8,qNoexcept=9,qInline=10,qExplicit=11,qin=32,qinout=33,qout=34,qflat=35};
template <AttribT atT>
struct AttribS {static constexpr AttribT at = atT;}
template <qual q>
struct _Qual{static constexpr AttribType at;};

template <>struct _Qual<qual::qStatic> : public AttribS<AttribT::ClassMember> {};
template <>struct _Qual<qual::qConstExpr> : public AttribS<AttribT::Every> {};
template <>struct _Qual<qual::qConst> : public AttribS<AttribT::LayoutExcl> {};
template <>struct _Qual<qual::qin> : public AttribS<AttribT::Every> {};
template <>struct _Qual<qual::qinout> : public AttribS<AttribT::Every> {};
template <>struct _Qual<qual::qout> : public AttribS<AttribT::Every> {};
template <>struct _Qual<qual::qflat> : public AttribS<AttribT::LayoutOnly> {};
template <>struct _Qual<qual::qExplicit> : public AttribS<AttribT::ClassMember> {};
template <>struct _Qual<qual::qVirtual> : public AttribS<AttribT::Method> {};
template <>struct _Qual<qual::qOverride> : public AttribS<AttribT::Method> {};
template <>struct _Qual<qual::qFinal> : public AttribS<AttribT::Method> {};
template <>struct _Qual<qual::qFriend> : public AttribS<AttribT::ClassMember> {};
template <>struct _Qual<qual::qInline> : public AttribS<AttribT::Func> {};
template <>struct _Qual<qual::qNoexcept> : public AttribS<AttribT::Func> {};

template <Str s,qual q>
struct Qual {static constexpr qual quality = q; static constexpr AttribT at = _Qual::at; }

using QStatic = Qual<qual::qStatic>;
using QConstExpr = Qual<qual::qConstExpr>;
using QConst = Qual<qual::qConst>;
using QIn  =Qual<qual::qin>;
using QInout =Qual<qual::qinout>;
using QOut =Qual<qual::qout>;
using QFlat =Qual<qual::qflat>;
using QVirtual =Qual<qual::qVirtual>;
using QOverride =Qual<qual::qOverride>;
using QExplicit =Qual<qual::qExplict>;
using QFinal =Qual<qual::qFinal>;
using QFriend = Qual<qual::qFriend>;
using QNoexcept = Qual<qual::qNoexcept>;
using QExtern = Qual<qual::qExtern>;
using QInline = QUal<qual::qInline>;

template <qual... Qs >
    struct Qualifiable {
        struct one_of<qual q ,qual qo, qual... Qs>{
            static constexpr bool value = q==q?true: one_of<q,Qs...>::value; 
        }
        struct one_of<qual q ,qual qo>{
            static constexpr bool value = q==q; 
        }
        std::enable_if<one_of<qExtern,Qs...>::value,bool>::type Extern;
        std::enable_if<one_of<qStatic,Qs...>::value,bool>::type Static;
        std::enable_if<one_of<qConstExpr,Qs...>::value,bool>::type ConstExpr;
        std::enable_if<one_of<qConst,Qs...>::value,bool>::type Const;
        std::enable_if<one_of<qin,Qs...>::value,bool>::type in;
        std::enable_if<one_of<qinout,Qs...>::value,bool>::type inout;
        std::enable_if<one_of<qout,Qs...>::value,bool>::type out;
        std::enable_if<one_of<qflat,Qs...>::value,bool>::type flat;
        std::enable_if<one_of<qVirtual,Qs...>::value,bool>::type Virtual;
        std::enable_if<one_of<qOverride,Qs...>::value,bool>::type Override;
        std::enable_if<one_of<qExplicit,Qs...>::value,bool>::type Explicit;
        std::enable_if<one_of<qFinal,Qs...>::value,bool>::type Final;
        std::enable_if<one_of<qFriend,Qs...>::value,bool>::type Friend;
        std::enable_if<one_of<qNoexcept,Qs...>::value,bool>::type Noexcept;  
        template <qual q>
        struct ptr {bool Qualifiable<Qs...>::*  p;}
template <> struct ptr<qExtern>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Extern;};
template <> struct ptr<qStatic>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Static;};
template <> struct ptr<qConstExpr>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::ConstExpr;};
template <> struct ptr<qConst>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Const;};
template <> struct ptr<qin>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::in;};
template <> struct ptr<qinout>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::inout;};
template <> struct ptr<qout>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::out;};
template <> struct ptr<qflat>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::flat;};
template <> struct ptr<qVirtual>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Virtual;};
template <> struct ptr<qOverride>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Override;};
template <> struct ptr<qExplicit>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Explicit;};
template <> struct ptr<qFinal>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Final;};
template <> struct ptr<qFriend>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Friend;};
template <> struct ptr<qNoexcept>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Noexcept;};

        template <qual Q>
        static constexpr bool hasQual(){return one_of<Q,Qs...>::value;}
        template <qual Q>
        bool& getQual(){return this->*ptr<Q>::ptr;}
        void push(std::vector<qual>& quals){
            for(qual it : quals)
        }
    };



struct stmt {
    using param_list=param_list;
    enum stmtty {
eBlock,eNS,
eOperator,
eDeclType,eDefType,
eDeclUnion,eDefUnion,
eExpr,
eDo,
eWhile,eFor,eForRange,
eSwitch,eCase,eDefault,
eIf,eElse,eElseIf,
eReturn,eContinue,
eFuncDecl,eFuncDef,
eVarDecl,
eUsing,eTypeDef,
eLayout,eSEnum,
eTry,eCatch,Throw
        
    };
    stmtty t;
struct NS;
struct Operator;
struct DeclType;
struct DefType ;
#ifdef CXX_C
struct DeclUnion ;
struct DefUnion;
#endif

using Expr = expr;
struct While;
struct For;
struct Do;
struct ForRange;
struct Switch;
struct Case;
using Default = block;
struct If;
struct Else;
struct ElseIf;
using Return = Expr;
struct FuncDecl;
struct FuncDef ;
struct VarDecl;
struct Using;
struct TypeDef;
struct Layout;
using Enum=EnumT ;
struct Try;
struct Catch ;
struct Throw;
using  block =pri::deque<funcvar>;
    using funcvar = pri::variant<Expr,While,For,Do,ForRange,Switch,If,Else,ElseIf,Return,Try,Catch,Throw,block>;
    
    struct NS {
        attrib_list atlist;

        std::string name;
        pri::deque<NS*> usingNS;
        pri::deque<NS*> inlineNS;

        pri::deque<NS> nss; 
        pri::deque<NS> inline_nss;
        // std::enable_if<q==temp::inst,type<temp::> >::type types;// TODO make correct
        pri::deque<VarDecl> vars;
        pri::deque<FuncDecl> funcs;
        pri::deque<Operator> operators;
        pri::deque<DeclType> types;
        pri::deque<Enum> Enums;
#ifdef CXX_C
         pri::deque<UnionDecl> unions; 
#endif
        pri::deque<Using> usings;
        pri::deque<TypeDef> tdefs;



        template <typename T>
        struct ptrmem ;
        template <> struct ptrmem<NS>          {static constexpr pri::deque<NS> NS::* ptr=nss;};
        template <> struct ptrmem<VarDecl>     {static constexpr pri::deque<VarDecl> NS::* ptr=vars;};
        template <> struct ptrmem<FuncDecl>    {static constexpr pri::deque<FuncDecl> NS::* ptr=funcs;};
        template <> struct ptrmem<Operator>{static constexpr pri::deque<Operator> NS::* ptr=operators;};
        template <> struct ptrmem<DeclType>    {static constexpr pri::deque<DeclType> NS::* ptr=types;};
        template <> struct ptrmem<Enum>        {static constexpr pri::deque<Enum> NS::* ptr=Enums;};
        template <> struct ptrmem<UnionDecl>   {static constexpr pri::deque<UnionDecl> NS::* ptr=unions;};
        template <> struct ptrmem<Using>       {static constexpr pri::deque<Using> NS::* ptr=usings;};
        template <> struct ptrmem<TypeDef>     {static constexpr pri::deque<TypeDef> NS::* ptr=tdefs;};

        
        void useNs(NS* n){usingNS = }
        NS& includeNS(std::string name){
            for(const NS& it : nss){
                if(it.name==name){return it;};
            };
            nss.push_back(NS());
            return nss.back();
        };
        NS& findNs(std::string name){
            for( NS& it : nss){if(it.name==name){return it;}};
            throw NameNotFound();
        };

         NS& includeNs(pri::deque<std::string> name){
            NS& cur = *this;
            for(std::string str : name){
                NS& res;
                try {res=cur.findNs(str);
                    cur=res;
                }
                catch (const NameNotFound& e){
                    cur=cur->includeNs(str);
                };
            };
            return cur;
        };
        template <typename T,pri::deque<T> NS::* ptr>
        bool _exists(std::string name){
            for(T&  it: this->ptr ){if(it->name==name){return true;}}
            return false;
        };
        bool exists(std::string name,stmtty* res){
            if(_exists<NS,&NS::nss>(name)){return stmtty::eNS;}
            if(_exists<VarDecl,&NS::vars>(name)){return stmtty::eVarDecl;}
            if(_exists<FuncDecl,&NS::funcs>(name)){return stmtty::eFuncDecl;}
            if(_exists<DeclType,&NS::types>(name)){return stmtty::eDeclType;}
            if(_exists<Enum,&NS::Enums>(name)){return stmtty::eEnum;}
            if(_exists<Using,&NS::usings>(name)){return stmtty::eUsing;}
            if(_exists<TypeDef,&NS::tdefs>(name)){return stmtty::eTypeDef;}
            #ifdef CXX_C
            if(_exists<,&NS::unions>(name)){return stmtty::eNS;}
            #endif
        };

        template <typename T, pri::deque<T> NS::* qt>
        T* find(std::string name){
            for(T& it : this->*qt){if(it->name==name){return &it;}}
            throw NameNotFound<T>();
        };
        
        void find(std::string name,result* r,resty* res){
            for(NS& it : inline_nss){try {find(name,r,res) ;return; }                catch (const NameNotFound e){}}
            try { pri::get<NS*>(*res)=find<VarDecl,&NS::nss>(name);*r=result::rVarDecl;return;}          catch (const NameNotFound<NS>& e){}
            try { pri::get<VarDecl*>(*res)=find<NS,&NS::vars>(name);*r=result::rNS;return;}         catch (const NameNotFound<VarDecl>& e){}
            try { pri::get<FuncDecl*>(*res)=find<FuncDecl,&NS::funcs>(name);*r=result::rFuncDecl;return;} catch (const NameNotFound<FuncDecl>& e){}
            try { pri::get<TypeDecl*>(*res)=find<NS,&NS::types>(name);*r=result::rNS;return;}       catch (const NameNotFound<TypeDecl>& e){}
            try { pri::get<Enum*>(*res)=find<Enum,&NS::Enums>(name);*r=result::rEnum;return;}         catch (const NameNotFound<Enum>& e){}
            try { pri::get<Using*>(*res)=find<Using,&NS::usings>(name);*r=result::rUsing;return;}      catch (const NameNotFound<Using>& e){}
            try { pri::get<Union*>(*res)=find<Union,&NS::unions>(name);*r=result::rUnion;return;}      catch (const NameNotFound<Union>& e){}
            try { pri::get<TypeDef*>(*res)=find<Union,&NS::tdefs>(name);*r=result::rUnion;return;}     catch (const NameNotFound<TypeDef>& e){}
            throw NameNotFound();
        }


        NS* findNs(std::string name) {return find<NS,&NS::nss>(name);}
        
        FuncDecl& findFunc(std::string s,pri::deque<expr>& args){
            for(FuncDecl& f : funcs){
                if(f==s and  (args <= f)){return f;}
            }
        };


        
        operator std::string(){return this->name;}
        
    };
    using tyty = accMember_list<temp::meta> ;
    
    struct VarDecl : public  Qualifiable<qExtern,qConst,qin,qout,qflat,qConstExpr,qStatic>{
        
        tyty tp;std::string name;size_t refNum=0;size_t ptrNum=0;
        bool memberPtr;tyty ptrmem;// In case of ptrToMember or funcPtr
        bool Default;expr DefaultValue;
        bool isPtr(){return ptrNum>0;}
        bool isRef(){return refNum<0;}
        bool isURef(){return refNum==2;}
        VarDecl() = default;
        VarDecl(tyty& _tp) : tp(_tp){};
        VarDecl(value& _vl) : vl(_vl){};
        VarDecl(tyty& tpp, std::string n , size_t r=0 , size_t p=0):tp(tpp),name(n),refNum(r),ptrNum(p){};
        VarDecl(tyty& tpp,tyty p ,std::string n , size_t r=0 , size_t p=0):tp(tpp),ptrmem(p),name(n),refNum(r),ptrNum(p){};
    };
    
    struct arg_list : public pri::deque<VarDecl>{bool pack;};


    template <typename SpecT>
    struct SpecIncl {
        using ty = SpecT;
        struct specN {
            ty t;
            param_list<temp::meta> tprms;
            param_list<temp::inst> spec;
            ty& get(){return t;}
            bool operator==(param_list<temp::inst>& p){return spec==p;}
            specN(param_list<temp::meta>& pr,param_list<temp::inst&& PR):tprms(pr), spec(PR){};
            specN() = default;
        };
        pri::deque<specN> specs;
        param_list<temp::meta> plist;


        specN* push(param_list<temp::meta>& m, param_list<temp::inst>& ptemp){
            param_list<temp::meta>::iter it = ptemp.begin();
            pri::deque<specN<temp::inst>>* ptr ;
            for(specN& it : specs) {
                if(it==ptemp){throw AlreadyDefdSpec<SpecT>();}
            };
            specs.emplace_back(m,ptemp);
        };
        SpecT& get( param_list<temp::inst>& plist){
            specN<temp::meta>* tr=tree;
            param_list<temp::inst>::iter it = plist.begin();
            for(;it!=plist.end();++it) {
                for(specN<temp::inst>& i :  tr->Insts){
                    if(i->prm==*it){tr=&i;break;}
                }
            };
            return tr->t;
        };
        SpecT& operator[](param_list<temp::inst>& plist){return get(plist);};
        SpecT& top(){return tree.get();};
        delete specTree();
        specTree() = default; 
        specTree(param_list& plt) {plist=plt;specs.push_back(specN())};
    };

    struct FuncDef :  public  Qualifiable<qExplicit,qFinal,qVirtual,qConstExpr,qOverride,qStatic> {
        attrib_list atlist;
        arg_list args;block body;
    };

    struct Operator : public  Qualifiable<qExtern,qExplicit,qFinal,qConstExpr,qOverride,qStatic>, public SpecIncl<FuncDef> {
        attrib_list atlist;
        bool typeConv;
        tyty opv;op::ty opt; 
        arg_list<temp::inst> args;
        tyty rett;
        template <op::ty oT>
        struct requ {static constexpr size_t argc_inclass = 1;static constexpr size_t argc_outclass = 2;};
        template <> requ <op::ty::opnot> {static constexpr size_t argc_inclass = 0;static constexpr size_t argc_outclass = 1;}
    };
    using init_args = pri::deque<expr>;
    struct init {
        init_args args;
        VarDecl* member;
        bool brace=false;
        init() = default;
        init(VarDecl* vdecl,bool&& br,arg_list&& arg);
    };  
    struct Constructor  : public Qualifiable<qExplicit,qConstExpr,qOverride,qStatic,qNoexcept>, SpecIncl<ConstructorDef> {
        attrib_list atlist;
        // param_list<temp::meta> plist;
        arg_list args;pri::deque<init> init_list; block body;  bool Default=  false;
        void get(arg_list& argl,param_list<temp::inst>& pl){}
        void get(param_list<temp::inst>& pl){}
        

        ConstructorDecl() = default;
    };
    struct DefType : Qualifiable<qExtern>{ 
        attrib_list atlist;
        type<temp::meta> t;
        void addType(type<temp::meta>& argt){t.variables.push_back(argt);};
        type<temp::meta>& findName(std::string s){
            for(type<temp::meta>& it : t.dependnents){if(it.name==s){return it;}};
            // for(type<temp::inst>& it : t.variables){if(it.name==s){return it;}};
            for(type<temp::meta>& it : t.methods){if(it.name==s){return it;}};
            for(type<temp::meta>& it : t.constructors){if(it.name==s){return it;}};
            throw  NameNotFound();
        };
        type<temp::meta>& get(param_list<temp::inst> pl){// TODO
            if constexpr (q==temp::meta){
            if(obj->t.tempTy){
                if(ps.size()!=obj->t.prms.size()){err::err<err::t::template_param_list_incomplete>()}
                auto t = ps.begin();
                auto temp = obj->t.prms.begin();
                for(;t!=ps.end() and (temp!=obj->t.prms.begin())){
                    if(t->ty!=temp->ty){
                        err::err<err::t::template_param_mismatch>();
                        
                        return;}
                    
                    ++t;++temp;
                };
            }
            else return *this;
        }
        else {return *this;}
        }
        DefType(std::string name) : t(name) {}
    };
        
    struct DeclType : Qualifiable<qExtern>,public SpecIncl<DefType> {
        std::string name;
        bool isUnion=false;bool anon=false;
        DeclType(std::string n) : name(n){}
    };
    struct While{Expr condition;block body;}
    struct For {Expr init; Expr condition;Expr incr;
            block body;
            attrib_list atlist;
    };
    struct ForRange {
        VarDecl vardecl;
        Expr ref;
        block body; 
        attrib_list atlist;
        decltype(*this) operator=(For& rhs){vardecl=rhs.init;return *this;}
    };
    struct Do : public While{bool d;};
    struct Case {expr ex;block body;
        Case(expr&& e){ex=e;}
    };
    
    struct Switch : Qualifiable<qConstExpr> {
        bool assign;value<temp::inst> vl;
        pri::deque<Case> css;block dflt; 
        expr ex; bool Init;expr inexpr;
        block body;
                attrib_list atlist;

    } ;
    struct If : Qualifiable<qConstExpr> { 
        Expr condition;
        block body;
                attrib_list atlist;bool Else=false;bool If=false;
        pri::deque<Else*> Elses;

        If(expr e) : condition(e) {}
    };
    struct Else {
        pri::variant<If*,Else*> IfS;bool elIf;
        block body;
        attrib_list atlist;

        Else(If& If) : elIf(false) {pri::get<If*>(Ifs)=&If;}
        Else(ElseIf& If) : elIf(true) {pri::get<ElseIf*>(Ifs)=&If;}
    };
    
    struct FuncDecl : public  public  Qualifiable<qExplicit,qVirtual,qFinal,qConstExpr,qOverride,qStatic>,public SpecIncl<FuncDef> {
        
        std::string name;
        param_list prms;
        using tyret = std::conditional<temp::meta==q,expr<temp::meta>,type*>::type;
        using tycond = expr ; tycond pre; tycond post ; // C++ 26
        
        tyty ret;
        arg_list args;
        attrib_list atlist;

        operator FuncDef(){
            FuncDef d;d.prms=prms;d.ret = ret;d.args=args;
            return d;
        };
        bool operator<=(pri::deque<expr> ar){
            pri::deque<expr>::iter ita=ar.begin();
            arg_list::iter itt=args.begin();
            for(;ita!=ar.end() and itt != args.end();){
                if(ita->getType() != itt->tp ){return false;}
                ++ita;++itt;
            }
            if(ita==ar.end()){
                for(itt!=args.end();++itt){
                    if(!itt->Default){return false;}
                };
            }
            else if(itt==args.end()){
                if(!args.pack){return false;}
                else {
                    for(;ita!=ar.end();++ita){
                        if(ita->getType!=args.back().tp){return false;}
                    }
                }
            };
            return true;
        };
        bool operator==(pri::deque<type<temp::meta>*> argts){// TODO
            if(args.size()!=args.size()){return false;}
            auto it = args.begin();
            for(type<meta>*&  ite : argts){
                if(*ite!=it->tp){return false;}
                ++it;
            };
            return true;
        };
        VarDecl& searchArg(sdt::string n){
            for(varDecl& it : args){if(it.name==n){return it;}}
        };
    };
    struct Using {
        bool Typename;
        param_list prms;
        std::string name ;

        tyty expr;
        attrib_list atlist;

        type<temp::> get(param_list<temp::inst> pl={}){};
    };
    struct Concept { // C++20
        param_list plist;

    };

    struct TypeDef {
        tyty tp;bool anon ;type<temp::inst> anontp;
        std::string name ;
        TypeDef(type<temp::inst> ty,std::string n) : t(ty),name(n) {}
    };
    struct Layout {
        enum stand {std430,std140};
        enum ty {location,binding};
        enum tyT { buffer,var,uniform};
        stand st;
        ty t;
        tyT vart;
        size_t loc;bool in=false;bool out=false;
        bool uniform;bool flat;
        pri::variant<VarDecl,TypeDef> data;bool in=false,bool out=false;
        std::conditional<temp::meta==q,attrib_list,Atts> atlist;

        Layout() = default;
    };
    struct Try{
        block body;
        pri::deque<Catch> catches;
    };
    struct Catch  {
    VarDecl var;
    block body;
attrib_list atlist;
    };
    struct  Throw {
        expr val;
        Throw(expr&& _val)  {val=_val;}
    };
    
    accSpec acc=accSpec::Public;
    using allvar =pri::variant<block,Layout,NS,FuncDecl,FuncDef,VarDecl,Using,TypeDef,Enum,DeclType,DefType,Expr,While,For,Do,ForRange,Switch,Case,Default,
    If,Else,ElseIf,Return,Try,Catch,Throw>;
    allvar var;
    using nsvar =  pri::variant<Layout,NS,FuncDecl,FuncDef,VarDecl,Using,TypeDef,Enum,DeclType,DefType,Expr,While,For,Do,ForRange,Switch,Case,Default,
    If,Else,ElseIf>;
    template <stmtty ty,stmtty... tys>
    bool isOneOf(stmtty st){if(st==ty){return true;}else {return isOneOf<tys...>(st);}};
    template <stmtty ty>
    bool isOneOf(stmtty st){if(st==ty){return true;}else {return false;}};
    
    template <qual ql>
    bool eval();
    template <> bool eval<qual::qStatic>(){if(!isOneOf<VarDecl>(t)){return false;}else{Static=true;;return true;}};
    template <> bool eval<qual::qConstExpr>(){if(!isOneOf<stmtty::VarDecl,stmtty::If,smtty::ElseIf,stmtty::Switch>(t);){return false;}else{ConstExpr=true;;return true;}};
    template <> bool eval<qual::qConst>(){Const=true;;return true;};
    template <> bool eval<qual::qin>(){if(!isOneOf<stmtty::VarDecl,stmtty::Layout>(t)){return false;}else{in=true;return true;}};
    template <> bool eval<qual::qinout>(){if(!isOneOf<stmtty::VarDecl,stmtty::Layout>(t)){return false;}else{in=true;out=true;return true;}};
    template <> bool eval<qual::qout>(){if(!isOneOf<stmtty::VarDecl,stmtty::Layout>(t)){return false;}else{out=true;return true;}};
    template <> bool eval<qual::qflat>(){if(!isOneOf<stmtty::Layout>(t)){return false;}else{in=true;return true;}};
    template <> bool eval<qual::qFriend>(){if(!isOneOf<stmtty::FuncDef,stmtty::TypeDecl>(t)){return false;}else{return true;}};
    template <> bool eval<qual::qExplicit>(){if(!isOneOf<stmtty::FuncDef>(t)){return false;}else{return true;}};
    template <> bool eval<qual::qVirtual>(){if(!isOneOf<stmtty::FuncDef>(t)){return false;}else{return true;}};
    template <> bool eval<qual::qFinal>(){if(!isOneOf<stmtty::FuncDef,Operator>()){return false;}else{;return true;}};
    template <> bool eval<qual::qNoexcept>(){if(!isOneOf<stmtty::FuncDecl,Operator>()){return false;}else{return true;}}
    bool push_qual(qual ql){
        switch(ql){
case qual::qStatic :{return eval<qual::qStatic>();}
case qual::qConstExpr :{return eval<qual::qConstExpr>();}
case qual::qConst :{return eval<qual::qConst>();}
case qual::qin :{return eval<qualq::in>();}
case qual::qinout :{return eval<qual::qinout>();}
case qual::qout :{return eval<qual::qout>();}
case qual::qflat :{return eval<qual::qflat>();}
case qual::qExplicit:{return eval<qual::qExplicit>();}
case qual::qFriend :{return eval<qual::qFriend>();} 
case qual::qVirtual :{return eval<qual::qVirtual>();} 
case qual::qFinal :{return eval<qual::qFinal>();} 
        }
        return false;
    };
    
    template <stmtty Ty>
    struct getTy {using ty = typename ty ;}
template<>struct getTy<stmtty::eBlock>{using ty=block;}
template<>struct getTy<stmtty::eNS>{using ty=NS;}
template<>struct getTy<stmtty::eDeclType>{using ty=DeclType;}
template<>struct getTy<stmtty::eDefType>{using ty=DefType;}
template<>struct getTy<stmtty::eDefTypeSpec>{using ty=DefTypeSpec;}
template<>struct getTy<stmtty::eExpr>{using ty=Expr;}
template<>struct getTy<stmtty::eAssign>{using ty=Assign;}
template<>struct getTy<stmtty::eDo>{using ty=Do;}
template<>struct getTy<stmtty::eWhile>{using ty=While;}
template<>struct getTy<stmtty::eFor>{using ty=For;}
template<>struct getTy<stmtty::eForRange>{using ty=ForRange;}
template<>struct getTy<stmtty::eSwitch>{using ty=Switch;}
template<>struct getTy<stmtty::eCase>{using ty=Case;}
template<>struct getTy<stmtty::eDefault>{using ty=Default;}
template<>struct getTy<stmtty::eIf>{using ty=If;}
template<>struct getTy<stmtty::eElse>{using ty=Else;}
template<>struct getTy<stmtty::eElseIf>{using ty=ElseIf;}
template<>struct getTy<stmtty::eReturn>{using ty=Return;}
template<>struct getTy<stmtty::eFuncDecl>{using ty=FuncDecl;}
template<>struct getTy<stmtty::eFuncDef>{using ty=FuncDef;}
template<>struct getTy<stmtty::eFuncDefSpec>{using ty=FuncDefSpec;}
template<>struct getTy<stmtty::eVarDecl>{using ty=VarDecl;}
template<>struct getTy<stmtty::eUsing>{using ty=Using;}
template<>struct getTy<stmtty::eTypeDef>{using ty=TypeDef;}
template<>struct getTy<stmtty::eLayout>{using ty=Layout;}
template<>struct getTy<stmtty::eOperator>{using ty=Operator;}
        
    template <stmtty Ty>
    getTy<Ty>::ty get(){return pri::get<getTy<Ty>::ty>(var);};
    std::enable_if<q==temp::meta,parList>::type paramters;
    template <typename StmtT>stmtty getTy();
template<> void getTy<block>(){return stmtty::Block;}
template<> void getTy<NS>(){return stmtty::NS;}
template<> void getTy<DeclType>(){return stmtty::DeclType;}
template<> void getTy<DefType>(){return stmtty::DefType;}
template<> void getTy<DefTypeSpec>(){return stmtty::DefTypeSpec;}
template<> void getTy<Expr>(){return stmtty::Expr;}
template<> void getTy<Assign>(){return stmtty::Assign;}
template<> void getTy<While>(){return stmtty::While;}
template<> void getTy<For>(){return stmtty::For;}
template<> void getTy<ForRange>(){return stmtty::ForRange;}
template<> void getTy<Do>(){return stmtty::Do;}
template<> void getTy<Switch>(){return stmtty::Switch;}
template<> void getTy<Case>(){return stmtty::Case;}
template<> void getTy<Default>(){return stmtty::Default;}
template<> void getTy<If>(){return stmtty::If;}
template<> void getTy<Else>(){return stmtty::Else;}
template<> void getTy<ElseIf>(){return stmtty::ElseIf;}
template<> void getTy<Return>(){return stmtty::Return;}
template<> void getTy<FuncDecl>(){return stmtty::FuncDecl;}
template<> void getTy<FuncDef>(){return stmtty::FuncDef;}
template<> void getTy<FuncDefSpec>(){return stmtty::FuncDefSpec;}
template<> void getTy<VarDecl>(){return stmtty::VarDecl;}
template<> void getTy<Using>(){return stmtty::Using;}
template<> void getTy<TypeDef>(){return stmtty::TypeDef;}
template<> void getTy<Layout>(){return stmtty::Layout;}

template <typename StmtT> set(){t=getTy<StmtT>();};

    template <typename StmtT>
    stmt(StmtT&& s) : acc(accSpec::public){
        pri::get<StmtT>(var)=s;
        set<StmtT>();
    };
    stmt() = default ;
};

#endif