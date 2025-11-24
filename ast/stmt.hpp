#ifndef STMSL_STMT
#define STMSL_STMT
#include "ast.hpp"
#include <petri/list.hpp> 
#include <type_traits>

namespace stmsl {

enum AttribT {
    LayoutOnly,LayoutExcl,Every,
    ClassMember,Func,Method
};
enum qual {qExtern=0,qStatic=1,qConstExpr=2,qConstEval,qConst=3,qVolatile,qVirtual=4,qOverride=5,qExplicit=6,qFinal=7,qNoexcept=9,qInline=10,qExplicit=11,qin=32,qinout=33,qout=34,qflat=35};
#define QUALS_EN qExtern,qStatic,qConstExpr,qConstEval,qConst,qVolatile,qin,qinout,qout,qflat,qVirtual,qOverride,qExplicit,qFinal,qNoexcept
template <qual... Qs >
    struct Qualifiable {
        struct one_of<qual q ,qual qo, qual... Qs>{static constexpr bool value = q==q?true: one_of<q,Qs...>::value; }
        struct one_of<qual q ,qual qo>{static constexpr bool value = q==q; }

        std::enable_if<one_of<qExtern,Qs...>::value,bool>::type Extern=false;
        std::enable_if<one_of<qStatic,Qs...>::value,bool>::type Static=false;
        std::enable_if<one_of<qConstExpr,Qs...>::value,bool>::type ConstExpr=false;
        std::enable_if<one_of<qConstEval,Qs...>::value,bool>::type ConstEval=false;
        std::enable_if<one_of<qConst,Qs...>::value,bool>::type Const=false;
        std::enable_if<one_of<qVolatile,Qs...>::value,bool>::type Volatile=false;
        std::enable_if<one_of<qin,Qs...>::value,bool>::type in=false;
        std::enable_if<one_of<qinout,Qs...>::value,bool>::type inout=false;
        std::enable_if<one_of<qout,Qs...>::value,bool>::type out=false;
        std::enable_if<one_of<qflat,Qs...>::value,bool>::type flat=false;
        std::enable_if<one_of<qVirtual,Qs...>::value,bool>::type Virtual=false;
        std::enable_if<one_of<qOverride,Qs...>::value,bool>::type Override=false;
        std::enable_if<one_of<qExplicit,Qs...>::value,bool>::type Explicit=false;
        std::enable_if<one_of<qFinal,Qs...>::value,bool>::type Final=false;
        std::enable_if<one_of<qNoexcept,Qs...>::value,bool>::type Noexcept =false; 
        template <qual q>
        struct ptr {bool Qualifiable<Qs...>::*  p;}
template <> struct ptr<qExtern>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Extern;};
template <> struct ptr<qStatic>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Static;};
template <> struct ptr<qConstExpr>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::ConstExpr;};
template <> struct ptr<qConstEval>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::ConstEval;};
template <> struct ptr<qConst>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Const;};
template <> struct ptr<qVolatile>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Volatile;};
template <> struct ptr<qin>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::in;};
template <> struct ptr<qinout>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::inout;};
template <> struct ptr<qout>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::out;};
template <> struct ptr<qflat>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::flat;};
template <> struct ptr<qVirtual>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Virtual;};
template <> struct ptr<qOverride>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Override;};
template <> struct ptr<qExplicit>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Explicit;};
template <> struct ptr<qFinal>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Final;};
template <> struct ptr<qNoexcept>{static constexpr bool Qualifiable<Qs...>::* p=&Qualifiable<Qs...>::Noexcept;};

        template <qual Q>
        static constexpr bool hasQual(){return one_of<Q,Qs...>::value;}
        template <qual Q>
        bool& getQual(){return this->*ptr<Q>::ptr;}

        template <qual _qs,qual _qss>
        void _pushQual(qual& q){if(q==_qs){this->*ptr::<_qs>::p =true;return;}
            if constexpr(sizeof...(_qss) >0){return _pushQual<_qss...>(q);}   
            throw QualifierNotAllowed();
        };
        void push(std::vector<qual>& quals){
            for(qual& it : quals){_pushQual<Qs...>(it);}
        }
    };


struct OperatorNotFound : Warning {};
struct stmt {
    using param_list=param_list;
    enum stmtty {
eblock=0,eNS=1,
eOperator=2,
eDeclType=3,eDefType=4,
eGoto=5,eLabel=6,
eExpr=7,
eDo=8,
eWhile=10,eFor=11,eForRange=12,
eSwitch=13,eCase=14,eDefault=15,
eIf=16,eElse=17,
eReturn=19,eContinue=20,
eFuncDecl=21,eFuncDef=22,
eVarDecl=23,
eUsing=24,eTypeDef=25,
eLayout=26,eEnum=27,eEnMember,
eTry=28,eCatch=29,eThrow=30,
eAsm=31,eLayout,
eConcept,eStructuredBinding,eNullStmt;
    };
    stmtty t;
struct NS;
struct Operator;
struct DeclType;
struct DefType ;

struct Label ;
struct Goto;
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

using Return = Expr;
struct FuncDecl;
struct FuncDef ;
struct VarDecl;
struct Using;
struct TypeDef;
struct Layout;
struct Enum : Attribs ;
struct Try;
struct Catch ;
struct Throw;
struct Asm;
struct Concept;
struct StructuredBinding;
struct NullStmt;

    
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
template<>struct getTy<stmtty::eReturn>{using ty=Return;}
template<>struct getTy<stmtty::eFuncDecl>{using ty=FuncDecl;}
template<>struct getTy<stmtty::eFuncDef>{using ty=FuncDef;}
template<>struct getTy<stmtty::eFuncDefSpec>{using ty=FuncDefSpec;}
template<>struct getTy<stmtty::eVarDecl>{using ty=VarDecl;}
template<>struct getTy<stmtty::eUsing>{using ty=Using;}
template<>struct getTy<stmtty::eTypeDef>{using ty=TypeDef;}
template<>struct getTy<stmtty::eLayout>{using ty=Layout;}
template<>struct getTy<stmtty::eOperator>{using ty=Operator;}
template<>struct getTy<stmtty::eStructuredBinding>(){using ty=StructuredBinding;}
template<>struct getTy<stmtty::eNullStmt>(){using ty=NullStmt;}
template<>struct getTy<stmtty::eConcept>(){using ty=Concept;}



    template <typename StmtT>stmtty constexpr getTyf();
template<> constexpr stmtty getTyf<block>(){return stmtty::eBlock;}
template<> constexpr stmtty getTyf<NS>(){return stmtty::eNS;}
template<> constexpr stmtty getTyf<DeclType>(){return stmtty::eDeclType;}
template<> constexpr stmtty getTyf<DefType>(){return stmtty::eDefType;}
template<> constexpr stmtty getTyf<DefTypeSpec>(){return stmtty::eDefTypeSpec;}
template<> constexpr stmtty getTyf<Expr>(){return stmtty::eExpr;}
template<> constexpr stmtty getTyf<Assign>(){return stmtty::eAssign;}
template<> constexpr stmtty getTyf<While>(){return stmtty::eWhile;}
template<> constexpr stmtty getTyf<For>(){return stmtty::eFor;}
template<> constexpr stmtty getTyf<ForRange>(){return stmtty::eForRange;}
template<> constexpr stmtty getTyf<Do>(){return stmtty::eDo;}
template<> constexpr stmtty getTyf<Switch>(){return stmtty::eSwitch;}
template<> constexpr stmtty getTyf<Case>(){return stmtty::eCase;}
template<> constexpr stmtty getTyf<Default>(){return stmtty::eDefault;}
template<> constexpr stmtty getTyf<If>(){return stmtty::eIf;}
template<> constexpr stmtty getTyf<Else>(){return stmtty::eElse;}
template<> constexpr stmtty getTyf<Return>(){return stmtty::eReturn;}
template<> constexpr stmtty getTyf<FuncDecl>(){return stmtty::eFuncDecl;}
template<> constexpr stmtty getTyf<FuncDef>(){return stmtty::eFuncDef;}
template<> constexpr stmtty getTyf<FuncDefSpec>(){return stmtty::eFuncDefSpec;}
template<> constexpr stmtty getTyf<VarDecl>(){return stmtty::eVarDecl;}
template<> constexpr stmtty getTyf<Using>(){return stmtty::eUsing;}
template<> constexpr stmtty getTyf<TypeDef>(){return stmtty::eTypeDef;}
template<> constexpr stmtty getTyf<Layout>(){return stmtty::eLayout;}
template<> constexpr smttty getTyf<Operator>(){return stmtty::eOperator;}
template<> constexpr stmtty getTyf<StructuredBinding>(){return stmtty::eStructuredBinding;}
template<> constexpr stmtty getTyf<NullStmt>(){return stmtty::eNullStmt;}
template<> constexpr stmtty getTyf<Concept>(){return stmtty::eConcept;}


struct Enum : public pri::deque<enmember> {
    
    struct EnMember {
        std::string name;
        expr<temp::meta> cexprval;  
        void resolve(ast& a){cexprval.resolve(a);
            if(!cexprval.isConstExpr()){err::e::EnumValueMustBeCexpr();}
        };
        enmember(std::string str) : name(str){}
    };
    bool enclass=false;attrib_list ats;
    integralT intt; bool SpeqSeq=false;
    using member = enmember;

    void resolve(ast& a){for(enmember& i : *this){i.resolve(a);}};
    expr<temp::meta> find(std::string name){
        for(enmember& it : *this){if(it.name==name){return &it;}}
        throw MemberNotFound<EnumT>("Enum Member Not Found");
    };
    operator bool (){return isDependent;}
} ;

template constexpr stmtty getTyf<Enum::Enmember>(){return stmtty::eEnMember;}
template <>struct getTy<stmtty::eEnMember>{using ty=Enum::EnMember;}



struct Concept {
    param_list<temp::meta> plist;
    arg_list reqs;
    struct Exprs {
        expr e;
        expr reslt;
    };
    pri::deque<Exprs> es;
};


using funcvar = pri::variant<Goto,Label,Expr,While,For,Do,ForRange,Switch,If,Else,Return,Try,Catch,Throw,block,Asm>;
struct funcStmt{funcvar inst;stmtty t;
    template <typename T>
    funcStmt(stmtty i,T& _d) : t(i){pri::get<T>(inst)=d;};
    template <typename T>
    void _resolve(ast& a){pri::get<T>(inst).resolve(a);}
    
    template <stmtty st,stmtty... sts>
    void Which(stmtty s,ast& a){
        if (s==st){_resolve<getTy<st>::ty>(inst).resolve(a);}
        else if constexpr (sizeof...(sts)>0){Which<sts...>(s,a);}
    };
    void resolve(ast& a){Which<eGoto,eLabel,eExpr,eWhile,eFor,eDo,eForRange,eSwitch,eIf,eElse,eReturn,eTry,eCatch,eThrow,eblock,eAsm>(t,a);}
};
    
    struct  block :pri::deque<funcStmt>{
    template <typename T>
    void push_back(T& inst){emplace_back(getTyf<T>(),inst); };
    void resolve(ast& a){
        for( funcStmt it : *this){is.resolve(a);         };
    };
};


struct Label{
    std::string name;
    Label(std::string n)  : name(n);
} ;
struct Goto{funcStmt* lbl;std::string nameLbl;
    Goto(std::string n ) : nameLbl(n){};
    void resolve(ast& a){
        for( Block  itb : pri::reverse(a.curBl)){
            for(funcStmt& it : itb ) {
                if(it.t=stmtty::eLabel){
                    if(pri::get<Label>(inst).name==nameLbl){lbl=&pri:get<Label>(inst);return;}
                }
            }
        };
        throw LabelNotFound();
    }
};
    struct Attribs {
        ;
        attribt_list atlist;
        std::vector<EnAt> atse;
    };
    struct NS: Attribs {
        
        std::string name;
        pri::deque<NS*> usingNS;
        pri::deque<NS*> inlineNS;

        pri::deque<NS> nss; 
        pri::deque<NS> inline_nss;
        // std::enable_if<q==temp::inst,type<temp::> >::type types;// TODO make correct
        pri::deque<VarDecl> vars;
        pri::deque<FuncDecl> funcs;
        pri::deque<Operator> operators;
        pri::deque<DeclType> types; pri::list<DefType*> anons;
        pri::deque<Enum> Enums;
        pri::deque<Using> usings;
        pri::deque<TypeDef> tdefs;
        pri::deque<Concept> concepts;


        template <typename T>
        struct ptrmem ;
        template <> struct ptrmem<NS>          {static constexpr pri::deque<NS> NS::* ptr=&NS::nss;};
        template <> struct ptrmem<VarDecl>     {static constexpr pri::deque<VarDecl> NS::* ptr=&NS::vars;};
        template <> struct ptrmem<FuncDecl>    {static constexpr pri::deque<FuncDecl> NS::* ptr=&NS::funcs;};
        template <> struct ptrmem<Operator>{static constexpr pri::deque<Operator> NS::* ptr=&NS::operators;};
        template <> struct ptrmem<DeclType>    {static constexpr pri::deque<DeclType> NS::* ptr=&NS::types;};
        template <> struct ptrmem<Enum>        {static constexpr pri::deque<Enum> NS::* ptr=&NS::Enums;};
        template <> struct ptrmem<Using>       {static constexpr pri::deque<Using> NS::* ptr=&NS::usings;};
        template <> struct ptrmem<TypeDef>     {static constexpr pri::deque<TypeDef> NS::* ptr=&NS::tdefs;};
        template <> struct ptrmem<Concept>     {static constexpr pri::deque<Concept> NS::* ptr=&NS::concepts;};

        
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
        };

        template <typename T, pri::deque<T> NS::* qt>
        T* find(std::string name){
            for(T& it : this->*qt){if(it->name==name){return &it;}}
            throw NameNotFound<T>();
        };
        
        void find(std::string name,result* r,resty* res){
            if(!anons.empty()){
                for(stmt::DeclType* it : anons){try{it->find(name,r,res)} catch(const NameNotFound& e){continue;};return;};
            };
            for(NS& it : inline_nss){try {find(name,r,res) ;return; }                catch (const NameNotFound e){}}
            try { pri::get<NS*>(*res)=find<VarDecl,&NS::nss>(name);*r=result::rVarDecl;return;}          catch (const NameNotFound<NS>& e){}
            try { pri::get<VarDecl*>(*res)=find<NS,&NS::vars>(name);*r=result::rNS;return;}         catch (const NameNotFound<VarDecl>& e){}
            try { pri::get<FuncDecl*>(*res)=find<FuncDecl,&NS::funcs>(name);*r=result::rFuncDecl;return;} catch (const NameNotFound<FuncDecl>& e){}
            try { pri::get<TypeDecl*>(*res)=find<NS,&NS::types>(name);*r=result::rNS;return;}       catch (const NameNotFound<TypeDecl>& e){}
            try { pri::get<Enum*>(*res)=find<Enum,&NS::Enums>(name);*r=result::rEnum;return;}         catch (const NameNotFound<Enum>& e){}
            try { pri::get<Using*>(*res)=find<Using,&NS::usings>(name);*r=result::rUsing;return;}      catch (const NameNotFound<Using>& e){}
            try { pri::get<TypeDef*>(*res)=find<TypeDef,&NS::tdefs>(name);*r=result::rTypeDef;return;}     catch (const NameNotFound<TypeDef>& e){}
            throw NameNotFound();
        }


        NS* findNs(std::string name) {return find<NS,&NS::nss>(name);}

        template <typename T, T NS::* ptr>
        void _resolvePost(ast& a){for(T& i : vars){i.resolve(a);}}
        template <typename T,typename... Ts>
        void __resolvePost(ast& a) {
            _resolvePost<T,ptrmem<T>::ptr>(a);
            if constexpr (sizeof...(Ts)>0){__resolvePost<Ts...>(a);}
        };
        void resolvePost(ast& a){__resolvePost<VarDecl,FuncDecl,Operator,DeclType,DefType,Enum,Using,TypeDef,Concept>(a);};
        Operator* findOperatorLit(std::string name){
            for(Operator it : operators){
                if(it.opt == lex::ty::dq and (it->name=name);){return &it;}
            };
            throw OperatorNotFound();
        };

        Operator* findOperator(op::ty o){
            for(Operator it : operators){
                if(it.opt == o){return &it;}
            };
            throw OperatorNotFound();
        };
        
        FuncDecl* findFunc(std::string s){FuncDecl* r = find<FuncDecl* , &NS::funcs>(name);return r;};
        operator std::string(){return this->name;}
    };
    using tyty = accMember_seq ;
    
    struct VarDecl : public Qualifiable<qExtern,qConst,qin,qout,qflat,qConstExpr,qConstEval,qStatic>{
        
        tyty tp;std::string name;expr arrSize=0;
        bool memberPtr;tyty ptrmem;// In case of ptrToMember or funcPtr
        bool Default;expr DefaultValue;
        
        type getTypePtrMem(){
            return ptrmem.back().Template ?
            pri::get<stmt::TypeDecl*>(ptrmem.back().inst)->get(ptrmem.back().plist) : pri::get<stmt::TypeDecl*>(ptrmem.back().inst).top(); 

        }
        type getType(){
            return tp.back().Template ?
            pri::get<stmt::TypeDecl*>(tp.back().inst)->get(tp.back().plist) : pri::get<stmt::TypeDecl*>(tp.back().inst).top(); 
        };

        bool isPtr(){return ptrNum>0;}
        bool isRef(){return refNum<0;}
        bool isURef(){return refNum==2;}
        VarDecl() = default;

        VarDecl(tyty& _tp) : tp(_tp){};
        VarDecl(value& _vl) : vl(_vl){};
        constexpr VarDecl( std::string n,std::vector<qual>& quals, type&& tpp) : name(n) {
            push(quals);
        };

        constexpr VarDecl( type<temp::inst>& tp,std::string str) : name(n) {
            push(quals);pri::get<type<temp::meta>>()
        };
        

        VarDecl(tyty& tpp, std::string n , size_t r=0 , size_t p=0):tp(tpp),name(n),refNum(r),ptrNum(p){};
        VarDecl(tyty& tpp,tyty p ,std::string n , size_t r=0 , size_t p=0):tp(tpp),ptrmem(p),name(n),refNum(r),ptrNum(p){};
    };
    
    struct arg_list : public pri::deque<VarDecl>{bool pack;};
    using init_args =  pri::deque<expr>;

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
        param_list<temp::meta> plist;ty Def;

        specN* push(param_list<temp::meta>& m, param_list<temp::inst>& ptemp){
            param_list<temp::meta>::iter it = ptemp.begin();
            pri::deque<specN>* ptr ;
            for(specN& it : specs) {
                if(it==ptemp){throw AlreadyDefdSpec<SpecT>();}
            };
            specs.emplace_back(m,ptemp);
        };
        virtual SpecT makeSpec(param_list<temp::inst>& plist,specN& sp);
        virtual SpecT make(param_list<temp::inst>& plist);

        
        int operator<=>(specN& is,specN& than){ // Returns 0 if no meaningful comparison or is the same amount.
            // (-)n by how many params are more specialized, 
            auto isit = is.spec.begin();auto thanit=than.spec.begin();
            int cm;
            for(;isit!= is.end() ;){
                int isc=0;
                switch(isit->vl.val.tt ){
                    case ==value::truTy::eaccList : {
                        if(pri::get<accMember_seq>(isit->vl.val)).back().t!=result::rParam){isc++;};
                    }
                    case value::truTy::eptrMember : {ics++;}
                    case value::truTy::efuncCall : {ics++;}
                    case value::truTy::einit_list : {ics++;}
                    case value::truTy::eliteral : {ics++;}
                }
                int thc=0;
                                if(isit->vl.val.tt==value::truTy::eaccList ){
                    if(pri::get<accMember_seq>(isit->vl.val)).back().t!=result::rParam){isc++;};
                }
                if (thanit==than.tail()){if(thanit->pack){continue}else break;}
                else {++thanit;}
                ++isit;
            };

        };
        bool resolved=false;
        void specsValidate(ast& a)
        void SpecsResolve(ast& a){if(resolved){return;}
            a.curtemp.push_back(&plist);Def.resolve(a);a.curtemp.pop_back();
            for(specN& it : specs){
                a.curtemp.push_back(&it.tprms);
                it.t.resolve(a);
                a.curTemp.pop_back();
            };
        };
        SpecT get( param_list<temp::inst>& plist){
            for(const specN& it : specs){
                if(plist==it.spec){return makeSpec(plist,it);}
            }
            return make(plist);
        };
        SpecT& operator[](param_list<temp::inst>& plist){return get(plist);};
        SpecT& top(){return Def;};
        SpecIncl() = default; 
        SpecIncl(param_list& plt) {plist=plt;resolved=false};
    };

    struct FuncDef :  Attribs,public  Qualifiable<qExplicit,qFinal,qVirtual,qConstExpr,qOverride,qStatic> {
        
        accMember_seq rett;
        arg_list args;block body;           

        struct preC {expr e;}
        struct postC {expr e;std::string res_name; };

        preC Pre; postC Post ; bool preb=false;bool postb=false;// C++ 26
tyty arrowRet;


        pri::deque<NS*> usingNS;
        pri::deque<FuncDecl> funcs;
        pri::deque<Operator> operators;
        pri::deque<DeclType> types; pri::list<DeclType*> anons;
        pri::deque<Enum> Enums;
        pri::deque<Using> usings;
        pri::deque<TypeDef> tdefs;
        pri::deque<Concept> concepts;
     template <typename T>
        struct ptrmem ;
        template <> struct ptrmem<NS>          {static constexpr pri::deque<NS> NS::* ptr=&NS::usingNS;};
        template <> struct ptrmem<FuncDecl>    {static constexpr pri::deque<FuncDecl> NS::* ptr=&NS::funcs;};
        template <> struct ptrmem<Operator>    {static constexpr pri::deque<Operator> NS::* ptr=&NS::operators;};
        template <> struct ptrmem<DeclType>    {static constexpr pri::deque<DeclType> NS::* ptr=&NS::types;};
        template <> struct ptrmem<Enum>        {static constexpr pri::deque<Enum> NS::* ptr=&NS::Enums;};
        template <> struct ptrmem<Using>       {static constexpr pri::deque<Using> NS::* ptr=&NS::usings;};
        template <> struct ptrmem<TypeDef>     {static constexpr pri::deque<TypeDef> NS::* ptr=&NS::tdefs;};
        template <> struct ptrmem<Concept>     {static constexpr pri::deque<Concept> NS::* ptr=&NS::concepts;};
 template <typename T, pri::deque<T> NS::* qt>
        T* find(std::string name){
            for(T& it : this->*qt){if(it->name==name){return &it;}}
            throw NameNotFound<T>();
        };
        
        void find(std::string name,result* r,resty* res){
            if(!anons.empty()){
                for(stmt::DeclType* it : anons){try{it->find(name,r,res)} catch(const NameNotFound& e){continue;};return;};
            };
            for(NS& it : inline_nss){try {find(name,r,res) ;return; }                catch (const NameNotFound e){}}
            try { pri::get<NS*>(*res)=find<VarDecl,&NS::nss>(name);*r=result::rVarDecl;return;}          catch (const NameNotFound<NS>& e){}
            try { pri::get<VarDecl*>(*res)=find<NS,&NS::vars>(name);*r=result::rNS;return;}         catch (const NameNotFound<VarDecl>& e){}
            try { pri::get<FuncDecl*>(*res)=find<FuncDecl,&NS::funcs>(name);*r=result::rFuncDecl;return;} catch (const NameNotFound<FuncDecl>& e){}
            try { pri::get<TypeDecl*>(*res)=find<NS,&NS::types>(name);*r=result::rNS;return;}       catch (const NameNotFound<TypeDecl>& e){}
            try { pri::get<Enum*>(*res)=find<Enum,&NS::Enums>(name);*r=result::rEnum;return;}         catch (const NameNotFound<Enum>& e){}
            try { pri::get<Using*>(*res)=find<Using,&NS::usings>(name);*r=result::rUsing;return;}      catch (const NameNotFound<Using>& e){}
            try { pri::get<TypeDef*>(*res)=find<TypeDef,&NS::tdefs>(name);*r=result::rTypeDef;return;}     catch (const NameNotFound<TypeDef>& e){}
            throw NameNotFound();
        }


        NS* findNs(std::string name) {return find<NS,&NS::nss>(name);}
        
        
        Operator* findOperatorLit(std::string name){
            for(Operator it : operators){
                if(it.opt == lex::ty::dq and (it->name=name);){return &it;}
            };
            throw OperatorNotFound();
        };

        Operator* findOperator(op::ty o){
            for(Operator it : operators){
                if(it.opt == o){return &it;}
            };
            throw OperatorNotFound();
        };

        FuncDecl* findFunc(std::string s){FuncDecl* r = find<FuncDecl* , &NS::funcs>(name);return r;};
     
        void setContracts(preC& pr,bool prb,postC& poc,bool pob){Pre=pr;preb=prb;Post=poc;postb=pob;}
        FuncDef(attrib_list _atlist,std::vector<qual> _quals,arg_list args,block _body) : atlist(_atilst),body(_body{ push(quals);}; 
        
        void resolve(ast& a){rett.resolve(a);args.resolve(a);body.resolve(a);};
        FuncDef() = default;
    };

    struct Operator : Attribs,public  Qualifiable<qExtern,qExplicit,qFinal,qConstExpr,qOverride,qStatic>, public SpecIncl<FuncDef> {
        
        bool typeConv;
        tyty opv;op::ty opt; std::string lit;
        arg_list<temp::inst> args;
        tyty rett;


        template <size_t _argcIn,size_t _argcOut>
        struct requ {static constexpr size_t argcIn = _argcIn;static constexpr size_t argcOut = _argcIn;};
        template <bool inC,bool oC>
        struct bType {static constexpr bool inC = true;static constexpr bool otClass = oC;}
        template <op::ty o>
        struct inoClass ;
template<> inoClass<opType>:  bType<true,false>,requ<0,0>{}
template<> inoClass<opcomma>: bType<true,true>,requ<1,2>{};
template<> inoClass<opNew> : bType<true,true>, requ<0,1>{};
template<> inoClass<opDelete> : bType<true,true>, requ<0,1>{};
template<> inoClass<opNewArr> : bType<true,true>, requ<1,2>{};
template<> inoClass<opDeleteArr> : bType<true,true>, requ<1,2>{};
template<> inoClass<opbnot> : bType<true,true>, requ<0,1>{};
template<> inoClass<oparrow> : bType<true,true>, requ<0,1>{};
template<> inoClass<opthree> : bType<true,true>, requ<1,2>{};
template<> inoClass<opeq> : bType<true,true>, requ<1,2>{};
template<> inoClass<oppeq> : bType<true,true>, requ<1,2>{};
template<> inoClass<opxoreq> : bType<true,true>, requ<1,2>{};
template<> inoClass<opmeq> : bType<true,true>, requ<1,2>{};
template<> inoClass<opandeq> : bType<true,true>, requ<1,2>{};
template<> inoClass<oporeq> : bType<true,true>, requ<1,2>{};
template<> inoClass<opNoteq> : bType<true,true>, requ<1,2>{};
template<> inoClass<opmuleq> : bType<true,true>, requ<1,2>{};
template<> inoClass<opdiveq> : bType<true,true>, requ<1,2>{};
template<> inoClass<opNot> : bType<true,true>, requ<1,0>{};
template<> inoClass<oplus> : bType<true,true>, requ<1,2>{};
template<> inoClass<opmod> : bType<true,true>, requ<1,2>{};
template<> inoClass<opmodeq> : bType<true,true>, requ<1,2>{};
template<> inoClass<ominus> : bType<true,true>, requ<1,2>{};
template<> inoClass<oband> : bType<true,true>, requ<1,2>{};
template<> inoClass<obor> : bType<true,true>, requ<1,2>{};
template<> inoClass<obxor> : bType<true,true>, requ<1,2>{};
template<> inoClass<omul> : bType<true,true>, requ<1,2>{};
template<> inoClass<odiv> : bType<true,true>, requ<1,2>{};
template<> inoClass<olt> : bType<true,true>, requ<1,2>{};
template<> inoClass<olteq> : bType<true,true>, requ<1,2>{};
template<> inoClass<strml> : bType<true,true>, requ<1,2>{};
template<> inoClass<strmleq> : bType<true,true>, requ<1,2>{};
template<> inoClass<ogt> : bType<true,true>, requ<1,2>{};
template<> inoClass<ogteq> : bType<true,true>, requ<1,2>{};
template<> inoClass<strmg> : bType<true,true>, requ<1,2>{};
template<> inoClass<strmgeq> : bType<true,true>, requ<1,2>{};
template<> inoClass<opp> : bType<true,true>, requ<0,1>{};
template<> inoClass<omm> : bType<true,true>, requ<0,1>{};
template<> inoClass<opand> : bType<true,true>, requ<1,2>{};
template<> inoClass<opor> : bType<true,true>, requ<1,2>{};
template<> inoClass<opxor> : bType<true,true>, requ<1,2>{};
template<> inoClass<opoxoreq> : bType<true,true>, requ<1,2>{};
template<> inoClass<oindex> : bType<true,true>, requ<0,1>{}; // Ca
template<> inoClass<ocall> : bType<true,true>, requ<0,1>{};

        

        Operator(op::ty o) : opt(o){};
        Operator(accMember_seq& r) : opv(r){};

    };


    
    struct FuncDecl : Attribs,  public  Qualifiable<qExplicit,qVirtual,qFinal,qConstExpr,qConstExpr,qOverride,qStatic>,public SpecIncl<FuncDef> {
        
        std::string name;
        using tyret = std::conditional<temp::meta==q,expr<temp::meta>,type*>::type;
        
        tyty ret;
        arg_list args;
        
        
tyty arrowRet;

        preC Pre; postC Post ; bool preb=false;bool postb=false;// C++ 26
        void setContracts(preC& pr,bool prb,postC& poc,bool pob){Pre=pr;preb=prb;Post=poc;postb=pob;}

        FuncDef make(param_list<temp::inst>& plist){
            
        };

        type getType(){
            return ret.back().Template ?
            pri::get<stmt::TypeDecl*>(ret.back().inst)->get(ret.back().plist) : pri::get<stmt::TypeDecl*>(ret.back().inst).top(); 
        };
        operator FuncDef(){return top();};
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
        bool operator==(arg_list& argl){// TODO
            auto it = args.begin();
            for(type<meta>&  ite : argl){
                if(*ite!=it->tp){return false;}
                ++it;
            };
            return true;
        };

        FuncDecl(std::string n,accMember_seq& accl,arg_list& prml): name(n){ret=accl;args=prml;};
        FuncDecl(FuncDef&& fdef){specs.push_back(fdef);}
        VarDecl& searchArg(sdt::string n){
            for(varDecl& it : args){if(it.name==n){return it;}}
        };
    };
    struct init {
        init_args args;
        VarDecl* member;
        bool brace=false;
        init() = default;
        init(VarDecl* vdecl,bool&& br,arg_list&& arg);
    };  
    struct Constructor  : Attribs,public Qualifiable<qExplicit,qConstExpr,qConstEval,qOverride,qStatic,qNoexcept>, SpecIncl<ConstructorDef> {
        
        // param_list<temp::meta> plist;
        arg_list args;pri::deque<init> init_list; block body;  bool Default=  false;
        void get(arg_list& argl,param_list<temp::inst>& pl){}
        void get(param_list<temp::inst>& pl){}
        
        void resolve(ast& a){args.resolve(a);init_list.resolve(a);body.resolve(a);}
        ConstructorDecl() = default;
    };
    struct DefType : Attribs,Qualifiable<qExtern>{ 
        
        type<temp::meta> t;
        void addType(type<temp::meta>& argt){t.variables.push_back(argt);};
        type<temp::meta>& findName(std::string s){
            for(type<temp::meta>& it : t.dependnents){if(it.name==s){return it;}};
            // for(type<temp::inst>& it : t.variables){if(it.name==s){return it;}};
            for(type<temp::meta>& it : t.methods){if(it.name==s){return it;}};
            for(type<temp::meta>& it : t.constructors){if(it.name==s){return it;}};
            throw  NameNotFound();
        };
        void resolve(ast& a){t.resolve(a);}
        DefType(std::string name) : t(name) {}
    };
        
    struct DeclType : Attribs,Qualifiable<qExtern>,public SpecIncl<DefType> {
        std::string name;
        bool isUnion=false;bool anon=false; bool dependentType;

        void resolve(ast& a){SpecsResolve(a);}
        DefType make(param_list<temp::inst>& plist){

        };
        type getType(){return top();}
        DeclType(std::string n) : name(n){}
    };
    struct While{Expr condition;block body;}
    struct For : Attribs{Expr init; Expr condition;Expr incr;
            block body;
            
    };
    struct ForRange :Attribs{
        VarDecl vardecl;
        Expr ref;
        block body; 
        
        decltype(*this) operator=(For& rhs){vardecl=rhs.init;return *this;}
    };
    struct Do : public While{bool d;};
    struct Case {expr ex;block body;
        Case(expr&& e){ex=e;}
    };
    
    struct Switch : Attribs,Qualifiable<qConstExpr,qConstEval> {
        bool assign;value<temp::inst> vl;
        block body;
        pri::deque<Case> css;block dflt; 
        expr ex; bool Init;expr inexpr;
        
    } ;
    struct If : Attribs,Qualifiable<qConstExpr,qConstEval> { 
        Expr condition;
        block body;
        bool Else=false;bool If=false;
        Else* el;
        void resolve(ast& a){condition.resolve(a);body.resolve();for( Else* it : Elses){it->resolve(a);}};
        If(expr e) : condition(e) {}
    };
    struct Else : Attribs{
        pri::variant<If*,Else*> IfS;
        bool elIf;
        Else* el;bool cond;expr condition;
        block body;
        
        void resolve(ast& a){
            if(cond){condition.resolve(a);}body.resolve(a);
        }
        Else(If* If) : elIf(false) {pri::get<If*>(Ifs)=If;}
        Else(Else* If) : elIf(false) {pri::get<If*>(Ifs)=If;}
    };
    
    struct Using : Attribs{
        bool Typename;
        param_list prms;
        std::string name ;

        tyty expr;
        

        type<temp::> get(param_list<temp::inst> pl={}){};
    };
    struct Concept : Attribs { // C++20
        param_list plist;
        union {
            expr e;
            struct {arg_list argl;};
        };
    
        
        bool reqr=false;// If reqr  second member of union;
        struct condition{expr e;expr reslt;}
        pri::deque<condition>  cds;
        Concept(param_list& pl) : plist(pl){};
    };

    struct TypeDef {
        tyty tp;bool anon ;type<temp::inst> anontp;bool dtype=false;expr e;
        std::string name ;
        TypeDef() = default ; 
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
        void resolve(ast& a){body.resolve(a);}
    };
    struct Catch  : Attribs{
    VarDecl var;
    block body;
    
        void resolve(ast& a){body.resolve(a);}
    };
    struct  Throw {
        expr val;
        void resolve(ast& a){val.resolve(a);}
        Throw(expr&& _val)  {val=_val;}
    };
    struct Asm : Attribs{
        
        std::string strlit;
        struct balancedToken {
            std::string cc;
            std::string ref;
            balancedToken(std::string c, std::string r) : cc(c), ref(r){};
        };
        pri::list<balancedToken> blt_seq;
    };
    struct StructuredBinding : Attribs{
        struct binding {
            bool pack;std::string name;
        };
        pri::list<binding> bn;
        accMember_seq ty;bool Auto;
        expr e;

    };
    struct NullStmt : Attribs {};
    
    accSpec acc=accSpec::Public;
    using allvar =pri::variant<block,Layout,NS,FuncDecl,FuncDef,VarDecl,Using,TypeDef,Enum,DeclType,DefType,Expr,While,For,Do,ForRange,Switch,Case,Default,
    If,Else,Return,Try,Catch,Throw,Goto,Label,StructuredBinding,NullStmt>;

    using allvarptr = pri::variant<block*,Layout*,NS*,FuncDecl*,FuncDef*,VarDecl*,Using*,TypeDef*,Enum*,DeclType*,DefType*,Expr*,While*,For*,Do*,ForRange*,Switch*,Case*,Default*,
    If*,Else*,Return*,Try*,Catch*,Throw*,Goto*,Label*,StructuredBinding,NullStmt>;
    allvar var;
    using nsvar =  pri::variant<Layout,NS,FuncDecl,FuncDef,VarDecl,Using,TypeDef,Enum,DeclType,DefType,Expr,While,For,Do,ForRange,Switch,Case,Default,
    If,Else>;
  
  
    template <stmtty ty,stmtty... tys>
    bool isOneOf(stmtty st){if(st==ty){return true;}else {return isOneOf<tys...>(st);}};
    template <stmtty ty>
    bool isOneOf(stmtty st){if(st==ty){return true;}else {return false;}};
    
  
    template <typename StmtT>
    stmt(StmtT&& s) : acc(accSpec::public){
        pri::get<StmtT>(var)=s;
        set<StmtT>();
    };
    stmt() = default ;
};
}
#endif