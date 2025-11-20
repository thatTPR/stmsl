#ifndef STMSL_PARSER_CPP
#define STMSL_PARSER_CPP
#include <string>
#include <filesystem>
#include <petri/list.hpp>
#include <petri/stack.hpp>
#include <petri/queue.hpp>
#include <petri/templates.hpp>
#include <ifstream>
#include "sys.cpp"
#include "ast.hpp"
#include "lex.hpp"
#include "keywords.hpp"

// #include <petri/regex.hpp>

namespace stmsl {
struct macro {
    std::string name ;
    pri::deque<lex> value;
    size_t filePos,ln,col;

    size_t argc; bool pack;
    pri::deque<std::string> args;
    bool function(){return argc>0 || pack;}
    void argc(pri::deque<lex> val){bool open =false;size_t s=0;
        pri::deque<lex>::iter it = val.begin();
        for(;it;++it){lex& l=*it;
            if( l.t==lex::ty::rparen){++it;break;};
            if(open ){
                if(l.t==lex::ty::comma){s++;};
                if(l.t==lex::ty::Name){args+=l.u.name;}
                continue;}
            if(l.t==lex::ty::lparen){open=true;continue;}
            else{break;}
        }
        for(;it;++it){value.push_back(*it);}
        argc=s;
    };
    bool checkName(std::string s,size_t* s){
        for(size_t i=0;i<args.size();i++){if(s==args[i]){ *s=i;return true;}}
        return false;
    };
    macro(std::string _name,pri::deque<lex> _val) : name(_name)  {
        pri::deque<lex> val;
        for(lex l : _val){
            if(l.t==lex::ty::Name){
                size_t p=0;
                size_t pp=0
                for(p = l.u.name.find_first_of("#",p);;p = l.u.name.find_first_of("#",p)){
                    if(p!=std::string::npos  ){
                         if(l.u.name[p+1] == lex::ty::str){
                            val.back(l.u.name.substr(pp,p-pp+1));
                            val.emplace_back(lex::ty::tokpaste);
                            p+=2;pp+=2;
                         }
                         else {
                            val.emplace_back(l.u.name.substr(pp,p-pp+1));
                            val.emplace_back(lex::ty::str);
                            p++;pp++;
                         }
                    }
                    else {
                        val.emplace_back(l.u.name.substr(pp));break;
                    }
                }
                continue;
            };
            val.push_back(l);
        };
        argc(val);
    };
    
    
};
      
#define LEX_ACC lex::ty::arrow,lex::ty::dot,lex::ty::dotptr,lex::ty::arrowptr,lex::ty::dcolon,lex::ty::dcolptr

struct macrosl {
    template <Str s>
    struct builtinMacro : s {

    };
    using mVA_ARG = builtinMacro<"__VA_ARGS__">;
    pri::list<macro> mlist ;
    pri::stack<expr> condition;
   
    void push(macro m ){
        for(macro mit : mlist){
            if(m.name == mit.name){
                mit.val=m.val;return;
            };
        }
        mlist.push_back(m);
    }
    template <typename... T>
    void emplace(T... arg){push(macro(arg...));};
    template <bool func>
    bool exists(std::string name){
        for(macro m : mlist){
            if(m.name==name){
                return true;}
        };
        return false;
    };
    bool get(std::string name,macro* ma){
        for(macro m : mlist){
            if(m.name==name){*ma=m;
                return true;}
        };
        return false;
    };


    
    bool expand(macro& m,pri::deque<lex>* ptr,pri::deque<std::string> s={}){
        if(s.length()!=m.argc or (m.pack)){return false;};
        for(lex l : m.value){
            if(l.t==lex::ty::Name){
                size_t i ;
                if(mVA_ARG::get()==l.u.name){
                    for(size_t j=m.args.size();j<s.size();s++){ptr->emplace_back(s[j]);}
                } 
                else if(m.checkName(l.u.name,&i)){
                    ptr->emplace_back(s[i]);
                }
                else {ptr->emplace_back(l.u.name);}
                continue;
            }
            if(l.t==lex::ty::tokpaste){continue;}
        };
        return true;
};
}
macrosl macros;



    class cphParser {
        public:
        cphAst fromFile(ast* ast){

        };
    };
    template <language lang,version<lang>::ty ver>
    class parser {
        public:

        std::string line;
        
        pri::stack<posit> pos;
        posit& posb(){return pos.back()};
        ast<temp::meta>* cast;     
        pcntxt cntxt ;
                pri::stack<std::ifstream> f;pri::stack<std::filesystem::path> curFilePath;

        bool interval(char c,char a,char b){return c>=a and c<=b ;}
        bool isdigit(char c){return interval(c,'0,'9);}
        bool alnum(char c){return interval(c,'a','z') or interval(c,'A','Z') or isdigit(c) or (c=='_');}
        size_t alnum(std::string s){size_t s=0;
            for(char c : s){if(!alnum(c)){return s;};s++;}
            return std::string::npos;
        }


        
        using tylexq=pri::deque<lex>;
        pri::deque<lex> lexq;
// size_t Mag=0;
        pri::stack<pri::deque<lex>::iter> itPtr;
        pri::stack<lex::ty> opens;
        void erase(){
            itPtr.pop();
            lexq.eraseFromEnd(itPtr.back());
        };

        template <lex::ty t>struct openedt{static constexpr lex::ty l;}
        template <>struct openedt<lex::ty::rbrace>{static constexpr lex::ty l=lex::ty::lbrace;}
        template <>struct openedt<lex::ty::rbrack>{static constexpr lex::ty l=lex::ty::lbrack;}
        template <>struct openedt<lex::ty::rparen>{static constexpr lex::ty l=lex::ty::lparen;}
        template <>struct openedt<lex::ty::rangle>{static constexpr lex::ty l=lex::ty::langle;}
        template <lex::ty t>
        void open(){
            opens.push(t);
            // switch constexpr (t){
            //     case lex::ty::lparen : {opens.push(t)};
            //     case lex::ty::lbrace : {opens.push(t)};
            //     case lex::ty::lbrack : {opens.push(t)};
            //     default 
            // };
        };
        template <lex::ty t>
        void close(){if(opens.top()==openedt<t>::l){opens.pop();}else{err::e<unexpectedToken>(*this);}};
        template <>
        void close<lex::ty::rbrace>(){if(opens.top()==openedt<lex::ty::rbrace>::l){opens.pop();cast->popbl();}else{err::e<unexpectedToken>(*this);}};


        pri::deque<lex>::iter& lexptrback(){return itPtr.back();}
        lex& lexitback(){return *(lexptrback());}

         template <lex::ty tokT>void up_cntxt();
         template <>void up_cntxt<lex::ty::NumUint>(){};
         template <typename... TS >
         void lexEmplace(TS... args){lexq.emplace_back(args...);lexptrback()=lexq.tail();};
         void lexMod(lex::ty t,size_t s=1){lexq.back().t=t;lexq.back().pos.ecol+=s;}

        void NewLine(){posb()++;if(std::getline(f.back(),line)){return }; throw FileEnd();}

        void untilRBLcom(){
            do{
                for(;posb()<line.length();++posb()){
                    if(line[posb()]==lex::ty::mul and line.length()>(posb()+1)){
                        if(line[posb()+1]==lex::ty::div){++posb();return;}
                    }
                }
            }while(NewLine());
        };
  

               char getCharLit(){
            char res;
            ++posb();
           if(line[posb()]=='\\'){
if(line[posb()+1]=='\''){res+='\''}//	single quote	byte 0x27 in ASCII encoding
else if(line[posb()+1]=='\"'){res+='\"'}//	double quote	byte 0x22 in ASCII encoding
else if(line[posb()+1]=='?'){res+='\?'}//	question mark	byte 0x3f in ASCII encoding
else if(line[posb()+1]=='\\'){res+='\\'}//	backslash	byte 0x5c in ASCII encoding
else if(line[posb()+1]=='a'){res+='\a';}//	audible bell	byte 0x07 in ASCII encoding
else if(line[posb()+1]=='b'){res+='\b';}//	backspace	byte 0x08 in ASCII encoding
else if(line[posb()+1]=='f'){res+='\f';}//	form feed - new page	byte 0x0c in ASCII encoding
else if(line[posb()+1]=='n'){res+='\n';}//	line feed - new line	byte 0x0a in ASCII encoding
else if(line[posb()+1]=='r'){res+='\r';}//	carriage return	byte 0x0d in ASCII encoding
else if(line[posb()+1]=='t'){res+='\t';}//	horizontal tab	byte 0x09 in ASCII encoding
else if(line[posb()+1]=='v'){res+='\v';}//	vertical tab
}
else {res=line[posb()];}
return res;
}
        std::string getStrLit(){
            std::string res;
            for(++posb();line[posb()]!='\'';++posb()){if((posb()==line.length())){NewLine();}
            if(line[posb()]=='\\'){
                if(line[posb()+1]=='\''){res+='\''}//	single quote	byte 0x27 in ASCII encoding
                else if(line[posb()+1]=='\"'){res+='\"'}//	double quote	byte 0x22 in ASCII encoding
                else if(line[posb()+1]=='?'){res+='\?'}//	question mark	byte 0x3f in ASCII encoding
                else if(line[posb()+1]=='\\'){res+='\\'}//	backslash	byte 0x5c in ASCII encoding
                else if(line[posb()+1]=='a'){res+='\a';}//	audible bell	byte 0x07 in ASCII encoding
                else if(line[posb()+1]=='b'){res+='\b';}//	backspace	byte 0x08 in ASCII encoding
                else if(line[posb()+1]=='f'){res+='\f';}//	form feed - new page	byte 0x0c in ASCII encoding
                else if(line[posb()+1]=='n'){res+='\n';}//	line feed - new line	byte 0x0a in ASCII encoding
                else if(line[posb()+1]=='r'){res+='\r';}//	carriage return	byte 0x0d in ASCII encoding
                else if(line[posb()+1]=='t'){res+='\t';}//	horizontal tab	byte 0x09 in ASCII encoding
                else if(line[posb()+1]=='v'){res+='\v';}//	vertical tab
                else {++posb();}
            }
        }
    }

        
        bool spaced= false;size_t Mag=0;
        [[likely ]] void lexName(){
                    if(lexq.back().t==lex::ty::Name and !spaced){lexq.back().addname(line[posb()]);return;}
                    else if(lexq.back().t==lex::ty::NumUint and lexq.back().u.unum==0){
                        if(line[posb()]=='x'){lexMod(lex::ty::lithex);spaced=false;return;}
                        else if(line[posb()]=='b'){lexMod(lex::ty::litbin);spaced=false;return;};        
                    }
                    lexEmplace(posb(),line[posb()]);spaced=false;
        };    
        void lexsw(){
                if(OneOfLex<lex::ty::space,lex::ty::nl>(line[posb()])){spaced=true;
                    for(;OneOfLex<lex::ty::space,lex::ty::nl>(line[posb()]) ;++posb()){if((posb()==line.length())){NewLine();};}}

                if(isdigit(line[posb()])){
                    if(lexq.back().t==lex::ty::NumFlt){lexq.back().setFlt(Mag);Mag=0;};
                        lexEmplace(posb(),lex::ty::NumUint);lexq.back().setUnum(line[posb()]);Spaced=false;return;

                    else {if(lexq.back().t==lex::ty::Name){lexq.back().addname(line[posb()]);return;}
                        switch(lexq.back().t){
                            case lex::ty::Name : {lexq.back().u.name+=line[posb()];break;}
                            case lex::ty::NumUint : {lexq.back().addUnum(line[posb()]);break;}
                            case lex::ty::NumFlt : {lexq.back().addUnum(line[posb()]);Mag++;break;}
                            default : {lexEmplace(posb(),lex::ty::NumUint);lexq.back().setUnum(line[posb()]);};break;;}
                    }
                }
                else {if(lexq.back().t==lex::ty::NumFlt){lexq.back().setFlt(Mag);Mag=0;};}
                
                

                switch(line[posb()]){
                    case lex::ty::ltangle :{
                        if(line.length()>(posb()+1)){
                            if(line[posb()+1]==lex::ty::ltangle){
                                if(line.length()>(posb()+2)){if(line[posb()+2]==lex::ty::eq){lexEmplace(posb(),lex::ty::strmlteq);return;}}
                                lexEmplace(posb(),lex::ty::strmlt);}
                            if(line[posb()+1]==lex::ty::eq){
                                if(line.length()>(posb()+2)){
                            if(line[posb()+1]==lex::ty::gtangle){lexEmplace(posb(),lex::ty::three)}}
                                
                                lexEmplace(posb(),lex::ty::lteq);return;}}

                        lexEmplace(posb(),lex::ty::ltangle);return;}

                    case lex::ty::gtangle :{ if(line.length()>(posb()+1)){
                            if(line[posb()+1]==lex::ty::gtangle){
                                if(line.length()>(posb()+2)){if(line[posb()+2]==lex::ty::eq){lexEmplace(posb(),lex::ty::strmgteq);return;}}
                                lexEmplace(posb(),lex::ty::strmgt);}
                            if(line[posb()+1]==lex::ty::eq){lexEmplace(posb(),lex::ty::gteq);return;}}

                        lexEmplace(posb(),lex::ty::gtangle);return;}
                    case lex::ty::lparen :{lexEmplace(posb(),lex::ty::lparen);return;}
                    case lex::ty::rparen :{lexEmplace(posb(),lex::ty::rparen);return;}
                    case lex::ty::lbrace :{lexEmplace(posb(),lex::ty::lbrace);return;}
                    case lex::ty::rbrace :{lexEmplace(posb(),lex::ty::rbrace);return;}
                    case lex::ty::lbrack :{
                        if(line.length()>(posb()+1)){
                            if(line[posb()+1]==lex::ty::lbrack){lexEmplace(posb(),lex::ty::ldi);return;}}
                        lexEmplace(posb(),lex::ty::lbrack);return;}
                    case lex::ty::rbrack :{
                        if(line.length()>(posb()+1)){
                            if(line[posb()+1]==lex::ty::rbrack){lexEmplace(posb(),lex::ty::rdi);return;}}
                        lexEmplace(posb(),lex::ty::rbrack);return;}
                    case lex::ty::bnot : {
                        if(line.length()>(posb()+1)){
                            if(line[posb()+1]==lex::ty::eq){lexEmplace(posb(),lex::ty::Noteq);return;}}
                        lexEmplace(posb(),lex::ty::bnot);return;}
                    case lex::ty::eq : {
                        if(line.length()>(posb()+1)){
                            if(line[posb()+1]==lex::ty::eq){lexEmplace(posb(),lex::ty::eqeq);return;}}
                        lexEmplace(posb(),lex::ty::eq);return;}
                    case les::ty::mod : {
                        if(line.length()>(posb()+1)){
                            if(line[posb()+1]==lex::ty::mod){lexEmplace(posb(),lex::ty::modeq);return;}}
                        lexEmplace(posb(),lex::ty::mod);return;}
               
                    case lex::ty::dq : {lexEmplace(posb(),lex::ty::dq);lexq.back().u.name=getStrLit();return;}
                    case lex::ty::sq : {lexEmplace(posb(),lex::ty::sq);lexq.back().u.chr=getCharLit();return;}
                    case lex::ty::dot :{
                            if(line.length()>(posb()+1)){if(line[posb()+1]==lex::ty::mul){lexEmplace(posb(),lex::ty::dotptr);return;}
                            if(line[posb()+1]==lex::ty::dot){
                                if(line.length()>(posb()+2)){
                                    if(line[posb()+2]==lex::ty::dot){lexEmplace(posb(),lex::ty::pack);return;}
                                }
                            }}
                            lexEmplace(posb(),lex::ty::dot);return;}
                    case lex::ty::comma :{lexEmplace(posb(),lex::ty::comma);return;}
                    case lex::ty::semicolon :{lexEmplace(posb(),lex::ty::semicolon);return ;}
                    case lex::ty::colon :{
                        if(line.length()>(posb()+1)){if(line[posb()+1]==lex::ty::colon){
                            if(line.length()>(posb()+2)){
                                if(line[posb()+2]==lex::ty::mul){lexEmplace(posb(),lex::ty::ptrmember);return;}
                            }
                            lexEmplace(posb(),lex::ty::dcolon);return;}}
                        lexEmplace(posb(),lex::ty::colon);
                        return;
                    }
                    case lex::ty::plus : {
                        if(line.length()>(posb()+1)){
                            switch(line[posb()+1]){
                                case lex::ty::plus : {lexEmplace(posb(),lex::ty::pp);return;}
                                case lex::ty::eq : {lexEmplace(posb(),lex::ty::peq);return;}
                            }};
                        lexEmplace(posb(),lex::ty::plus);return;
                    }
                    case lex::ty::minus : {
                        if(line.length()>(posb()+1)){
                            switch(line[posb()+1]){
                                case lex::ty::minus : {lexEmplace(posb(),lex::ty::mm);return;}
                                case lex::ty::eq : {lexEmplace(posb(),lex::ty::meq);return;}
                                case lex::ty::gtangle : {
                                    if(line.length()>(posb()+2)){if(line[posb()+2]==lex::ty::mul){lexEmplace(posb(),lex::ty::arrowptr);return;}}
                                    lexEmplace(posb(),lex::ty::arrow);return;
                                }
                            }}
                        lexEmplace(posb(),lex::ty::minus);return;}
                    case lex::ty::band :{
                        if(line.length()>(posb()+1)){
                            switch(line[posb()+1]){
                                case lex::ty::band : {lexEmplace(posb(),lex::ty::oand);return;}
                                case lex::ty::eq : {lexEmplace(posb(),lex::ty::andeq);return;}
                            }};
                        lexEmplace(posb(),lex::ty::band);return;}
                    case lex::ty::bor :{
                        if(line.length()>(posb()+1)){
                            switch(line[posb()+1]){
                                case lex::ty::bor : {lexEmplace(posb(),lex::ty::oor);return;}
                                case lex::ty::eq : {lexEmplace(posb(),lex::ty::oreq);return;}
                            }}
                        lexEmplace(posb(),lex::ty::bor);return;}
                    case lex::ty::bxor : {
                        if(line.length()>(posb()+1)){
                            if(line[posb()+1]==lex::ty::eq){lexEmplace(posb(),lex::ty::xoreq);return;}}
                        lexEmplace(posb(),lex::ty::bxor);return;}
                    case lex::ty::mul : {
                        if(line.length()>(posb()+1)){
                            if(line[posb()+1]==lex::ty::eq){lexEmplace(posb(),lex::ty::muleq);return;}}
                        lexEmplace(posb(),lex::ty::mul);return;}
                    case lex::ty::div : {
                        if(line.length()>(posb()+1)){
                            if(line[posb()+1]==lex::ty::eq){lexEmplace(posb(),lex::ty::diveq);return;}
                            if(line[posb()+1]==lex::ty::div){NewLine();return;}
                            if(line[posb()+1]==lex::ty::mul){untilRBLcom();return;}
                        }
                        lexEmplace(posb(),lex::ty::div);return;}

                    case lex::ty::cond : {lexEmplace(posb(),lex::ty::cond);return;}
                    case lex::ty::escape :{++posb();return;}
                    case lex::ty::str { if(line.length()>(posb()+1) ){if(line[posb()+1] == lex::ty::str){lexEmplace(posb(),lex::ty::tokpaste);return;}}
                            
                    }
                    case lex::ty::atsign : {lexEmplace(posb(),lex::ty::atsign)}
                    default : {lexName();};
                }
            }
                
        
        inline void Line(){
            if(NewLine()){for(;posb()<line.length();++posb()){lexsw<true>();}
        };
        class FileEnd {
            bool b;
            FIleEnd(bool bl) :b(bl){}
        };

        inline void next(){
            ++posb();
            if(posb()<line.length()){lexsw();}
            else NewLine();
        };
        bool nextTOK();
        bool checkName(){
            if(lexitback().t==lex::ty::Name){
                macro& m;
                if(macros.get(lexitback().u.name,&m)){
                    itPtr.push_back(lexptrback());
                    pri::deque<lex> q;
                    if(m.function()){
                        nextTOK();
                        if(lexitback().t!=lex::ty::lparen){--lexptrback();err::e<err::t::expectedToken,lex::ty::lparen>(*this);}
                        else {
                            pri::deque<std::string> args;bool arg=true;
                            for(nextTOK();lexitback().t!=lex::ty::rparen;nextTOK()){
                                if(OneOfLex<lex::ty::space>()){continue;}
                                if(OneOfLex<lex::ty::comma>()){arg=true;;}
                                if(checkName()){
                                    for(;lexitback().t==lex::ty::space;nextTOK()){}
                                    if(lexitback().t==lex::ty::Name){
                                        if(!arg){err::e<err::t::macro>(*this);}
                                        else {args.push_back(lexitback().u.name);arg=false;}
                                    }
                                };
                            }
                            macros.expand(m,&q,args);
                        }
                    }
                    else {macros.expand(m,&q);}
                    erase();
                    lexq.concat(q);
                    nextTOK();
                };
                return true;
            }
            return false;
        };
        bool nextTOK(){
            while(lexptrback()==lexq.tail()){next<true>();}
            ++lexptrback();if(it->t==lex::ty::Name){checkName();}
        };
            
    };
   
        template <lex::ty T,lex::ty... Ts>
        bool _OneOfLex(lex l){return pri::OneOf<lex::ty,T,Ts...>(l);}
        

        template <lex::ty T,lex::ty... Ts>
        bool OneOfLex(){return OneOfLex<lex::ty,T,Ts...>(lexitback().t);}
        

        template <typename Kw,typename... Kws>
        bool OneOfKw(){
            if(KW::check(lexitback().u.name)){return true;}
            else if constexpr(sizeof...(Kws)){return OneOfKw<Kws...>(str);}
            else return false;
        };
      
        using lexres=pri::deque<pri::deque<lex>::iter>;
       
        
        template <lex::ty to,lex::ty... tok>
        void until(){
            for(;lexptrback()->t!=to ;nextTOK()){
                if(!OneOfLex<tok...>()){throw UnexpectedToken(lexitback());}
            };
        };
        template <lex::ty to,lex::ty... tok>
        bool untilWarn(){bool wrn=false;
            for(;lexptrback()->t!=to ;nextTOK()){
                if(!OneOfLex<tok...>()){wrn=true;}
            };
            return wrn;
        };
        template <lex::ty t,lex::ty... ts>
        void untilDif(){while(OneOfLex<t,ts...>(lexptrback()->t)){nextTOK();}};
        template <lex::ty... ts>
        void untilOneOf(){while(!OneOfLex<t,ts>()){nextTOK();}}
        
   
        template <lex::ty fromTok,lex::ty toTok,template <temp q> typename T>
        T<temp::meta> _getFromUntil(){return getFromUntil<fromTok,toTok,T<temp::meta>();};



        template <lex::ty t,template <temp q> typename T>
        T<temp::meta> _getUntil(){return getUntil<t,T<temp::meta>();        };
        
        template <lex::ty tokFrom,lex::ty tokUntil , typename T>
        T getFromUntil(){
            itPtr.push_back(lexptrback());
            deque<lex> stripped = strippedFromUntil<t,by>(itr,found);
            return getFromUntilStripped<t,by,T>();

        };     
        template <lex::ty tokFrom,lex::ty tokUntil ,template <temp q> typename T>
        T<temp::meta> _getFromUntil(){return getFromUntil<tokFrom,tokUntil,T<temp::meta>>()};
        template <lex::ty l>
        void expectErr(){
            if(lexitback().t==l){return }
            else{err::e::(*this,UnexpectedToken());for(nextTOK();lexitback().t!=lex::ty::Name;nextTOK()){};}
        };
        void Stmt();
       
        
        template <accSpec acc>void access(){curAcc.back()=acc;};
        template <accSpec acc>void accessPush(){curAcc.push(acc);};
        template <lex::ty tok>
        stmt::block getBlockUntil(){
            for(;lexptrback().t!=tok;++(lexptrback())){
                getStmtUntil_EOSTMT()
            };
        };

        template <typename KW,typename... KWs>
        bool kwFound(){
            if constexpr (KW::check(lexitback().u.name)){KW::proc(*this);return true;}
            if constexpr (sizeof...(KWs)>0){return kwFound<KWs>();}
            return false;
        } ;
      
        template <bool Eval,typename KW,typename... KWs>
        void untilKW(){for(;!OneOfKw<KW,KWs...>();nextTOK()){};};
        
        // Handling Structs
        template <temp q,template<temp> typename T,lex::ty... l> T<q>& get();
        
         bool correct(accMember_list<q>& acc){
            switch(acc.acc){
                case lex::ty::none : {return true;}
                case lex::ty::arrow: {return isPtrCompound<temp::meta>(acc.r,acc.inst);}
                case lex::ty::dot: {return hasMember<temp::meta>(acc.r,acc.inst);}
                case lex::ty::dotptr: {return isCompound<temp::meta>(acc.r,acc.inst);}
                case lex::ty::arrowptr: {return isPtrCompound<temp::meta>(acc.r,acc.inst);}
                case lex::ty::dcolon: {return isCompound<temp::meta>(acc.r,acc.inst);}
                // case lex::ty::dcolptr: {return isCompound<temp::meta>(acc.r,acc.inst);}
                default :  {throw UnexpectedToken();}
            }
        }
        template <temp q>
        bool member(accMember_list<q>& acc){
            switch(acc.acc){
                case lex::ty::none : {return true;}
                case lex::ty::arrow: {return isMember<temp::meta>(acc.r,acc.inst);}
                case lex::ty::dot: {return isMember<temp::meta>(acc.r,acc.inst);}
                case lex::ty::dotptr: {return isMemberPtr<temp::meta>(acc.r,acc.inst);}
                case lex::ty::arrowptr: {return isMemberPtr<temp::meta>(acc.r,acc.inst);}
                case lex::ty::dcolon: {return *(r==result::rType);}
                // case lex::ty::dcolptr: {return *(r==result::rType);}
                default :  {throw UnexpectedToken();}
            }
        };
         op::ty getOperator(){
            op::ty op;
            nextTOK();
            if(lexitback().t!=lex::ty::Name){nextTOK();return lexitback().t;}
            else { if(kw_New::check(lexitback().u.name)) {nextTOK();if(lexitback().t==lex::ty::lbrack){nextTOK();nextTOK();return op::ty::opNewArr};return op::ty::opNew;}
            else if(kw_Delete::check(lexitback().u.name)){nextTOK();if(lexitback().t==lex::ty::lbrack){nextTOK();nextTOK();return op::ty::opDeleteArr};return op::ty::opDelete;}
            else {return op::ty::opType;} 
            }
        }

        template <bool Global,bool Res,temp q==temp::meta>
        void proc(accMember_list<q>& res){
            if constexpr (Global){res.globalAcc=true;nexTOK();}
            if(kw_Operator::check(lexitback().u.name)){
                res.emplace_back(accMember::result::rOperator);
                res.back().oprt=getOperator();}
                else {res.emplace_back(lexitback().u.name)
                if constexpr (Global){res.back().inst=cast->findNameFromNs(cast->global,lexitback().u.name,&res.back().r);}
                if constexpr (!Res){res.back().inst=cast->findName(lexitback().u.name,&res.back().r,&res.back().inst);}
                else {res.back().inst=cast->findName(lexitback().u.name,&res.inst,&res.back().r);}
            }
            else {res.back().name = lexitback().u.name;}
            nextTOK();
            if(OneOfLex<lex::ty::ltangle>()){res.back().Template=true;res.back().plist=get<temp::inst,param_list>();nextTOK();}
            if(OneOfLex<LEX_ACC>()){res.back().acc=lexitback().t};
            else {throw accMemberStop(res);}
        }
        template<temp q>
        void procName(accMember_list<q>& res){
            res.emplace_back(lexitback.u.name);nextTOK();
            if(OneOfLex<lex::ty::ltangle>()){res.back().plist=get<temp::inst,param_list>();nextTOK();}
            if(OneOfLex<LEX_ACC>()){res.back().acc=lexitback().t};
            else throw accMemberStop(res);}
        
        template <lex::ty... l> accMember_list get_accMember_list(){
            try {
            accMember_list<q> res;res.pos=lexitback().pos;res.acc=lex::ty::none;
            if(lexitback().t==lex::ty::dcolon){proc<true,false,q>(res);}
            else {proc<false,false,q>(res);}
            if(res.back().r==result::rParam){
                for(nextTOK();!OneLex<lex::ty::lparen,l...>();nextTOK()){if(lexitback().t==lex::ty::Name){procName(res);}else throw UnexpectedToken();}
            }
            else {
                for(nexTOK();!OneLex<lex::ty::lparen,l...>();nextTOK()){if(OneOfLex<lex::ty::Name>()){proc<false,true,q>(res);}else throw UnexpectedToken();}
            }
            return res;
            }catch(const NameNotFound& e){err::e(*this,UnexpectedToken<accMember_list<q>>());}
        };        
        template <temp q> value handleDeclType(){
            nextTOK();expectErr<lex::ty::lparen>();nextTOK();bool rValue=false;
            if(OneOfLex<lex::ty::lparen>()){/* Handle For rvalue;*/rValue=true;;nextTOK();}
            accMember_list<q> accmem;
            return value(getExpr<lex::ty::rparen>(),value::ty::DeclT;
            if(rValue){nextTOK();expectErr<lex::ty::rparen>();nextTOK();}
        };
        // These two functions are invalid
        template <value::ty vlt>
        value get_value();
        value& get_value<value::ty::typeValue>(){
            res.t=value::ty::typeValue;
            resty<temp::meta> rs;result r;
            if(kw_Decltype::check(lexitback().u.name)){ typ = handleDeclType<temp::inst>();}
            rs=cast->findName(lexitback().u.name,&r);
            for(nextTOK();OneOfLex<lex::ty::Name,lex::ty::dcolon,lex::ty::ltangle>();nextTOK()){
                if(OneOfLex<lex::ty::ltangle>()){if(r==ast<q>::result::rType){
                    pri::get<type<temp::meta>>(rs)=pri::get<type<temp::meta>>(rs).get(get<temp::inst,param_list>());};}
                if(OneOfLex<lex::ty::Name>(lexitback().u.name)){      
                    if(!cast->find(lexitback.u.name,&rs,&r)){err::e(*this,NameNotFound());}}
            };
            switch(r){
                case ast<q>::result::rType : {
                    if constexpr (q==temp::meta){return pri::get<stmt::Using>(rs).expr;}
                    else {return pri::get<stmt::Using>(rs).expr;}
                }
                default :{throw TypeNotFound();};
            }
        };
        template<>value get_value<value::ty::ptrmember>(){
            value res;res.t=value::ty::ptrmember;
            resty<temp::meta> rs;result r;
             if(kw_Decltype::check(lexitback().u.name)){ typ = handleDeclType<temp::inst>();}
            rs=cast->findName(lexitback().u.name,&r);
            for(nextTOK();OneOfLex<lex::ty::space,lex::ty::Name,lex::ty::dcolon,lex::ty::ltangle,lex::ty::eq>();nextTOK()){
                if(OneOfLex<lex::ty::space>()){continue;}
                if(OneOfLex<lex::ty::ltangle>()){if(r==ast<q>::result::rType){
                    pri::get<type<temp::meta>>(rs)=pri::get<type<temp::meta>>(rs).get(get<temp::inst,param_list>());};}
                if(OneOfLex<lex::ty::Name>(lexitback().u.name)){      
                    if(!cast->find(lexitback.u.name,&rs,&r)){err::e(*this,NameNotFound());}}
            };
            switch(r){
                case ast<q>::result::rVar : {
                    if constexpr (q==temp::meta){return pri::get<stmt::VarDecl>(rs).tp;}
                    else {return value(pri::get<stmt::VarDecl>(rs).tp);}
                }
                default throw NameNotFound();
            }
        };

        template <lex::ty... l> 
        expr getExprName();
         template <lex::ty... l> 
        expr getExpr();

        template <> param<temp::inst>& get<temp::inst,param>(){

            param<temp::inst> res;res.vl=getExpr<lex::ty::comma,lex::ty::gtangle>();return res;};
        template <temp q> param<temp::meta>& get<temp::meta,param>(){     
            param<temp::meta> res;
                auto packCheck = [&](){nextTOK();if(OneOfLex<lex::ty::pack>()){res.pack=true;nextTOK();}}
                if(OneOfKw<kw_Template>()){
                    until<lex::ty::ltangle,lex::ty::space,lex::ty::nl>();
                    param_list<temp::meta> pl=get<temp::meta,param_list>();
                    res.t=param<temp::meta>::ty::Template;
                    res.pl=pl;res.Template = true;
                    if(lexitback().t==lex::ty::Name){
                        if(OneOfKw<kw_Typename,kw_Class,kw_Struct>() ){
                            nextTOK();expectErr<lex::ty::Name>();
                            res.name=lexitback().u.name;packCheck();return res;    
                        }
                    }
                     err::e(*this);
                }
                else if(OneOfKw<kw_Typename,kw_Class,kw_Struct>(lexitback().u.name)){
                    nextTOK();res.t=param<temp::meta>::ty::Typename;
                    if(OneOfLex<lex::ty::Name>()){res.name=lexitback().u.name;packCheck();return res;}
                    else {throw UnexpectedToken(); }
                }
                else {
                     if(lexitback().t==lex::ty::Name){res.t=param<temp::meta>::ty::Type;
                        try{res.tp = get_accMember_list();}
                        catch(const accMemberStop& e){}
                        
                        try{res.memberList = get_accMember_list();}
                        catch(const accMemberStop& e){}
                        if(OneOfLex<lex::ty::mul>()){res.t=param<temp::meta>::ty::PtrToMember};
                        else {res.name=res.memberList.back().name; res.memberList.clear();}
                        packCheck();return res;
                    }
                    else {err::e(*this,UnexpectedToken());}
                }
        };
        
        template <temp q> param_list<q> get<q,param_list>(){param_list plist;
            itPtr.push_back(itPtr.back());
            if(nextTOK();!OneOfLex<lex::ty::rangle>();nextTOK()){
                if(OneOfLex<lex::ty::comma>()){continue;}
                    plist.push_back(get<q,param>());
            }
            erase();
            return plist;
        };
               
        arg_list<temp::meta> getFptrArgList(){
            arg_list<temp::meta> pl;
            for(;!OneOfLex<lex::ty::rparen>();nextTOK()){
                std::vector<qual> quals;
                for(;OneOfLex<lex::ty::Name>();nextTOK();){
                if(kw_in::check(lexitback().u.name)){quals.push_back(qual::qin);};
                else if(kw_out::check(lexitback().u.name)){quals.push_back(qual::qout);};
                else if(kw_out::check(lexitback().u.name)){quals.push_back(qual::qinout);};
                else if(kw_const::check(lexitback().u.name)){quals.push_back(qual::qConst);};
                else break;
                };
                accMember_list ls = get_accMember_list();
                pl.emplace_back(ls);
                for(nextTOK();OneOfLex<lex::ty::mul>();nextTOK()){pl.back().ptrNum++;}
                for(;OneOfLex<lex::ty::band>();nextTOK()){pl.back().refNum++;}
                for(qual it : quals){pl.back().push(quals);}
            }
            return pl;
        };
        template <lex::ty... l>
        void isFuncPtr(accMember_list acc){
            nextTOK();expectErr<lex::ty::mul>();
            nextTOK();expectErr<lex::ty::Name>();
            std::string name = lexitback().u.name;
            nextTOK();expectErr<lex::ty::rparen>()
            nextTOK();expectErr<lex::ty::lparen>()
            nextTOK();expectErr<lex::ty::Name>()
            arg_list<temp::meta> prml = getFptrArgList();
            value vl = value(stmt::FuncDecl(acc,prml),value::t::funcPtr);
            expectErr<lex::ty::rparen>();
            cast->pushStmt<stmt::VarDecl>(stmt::VarDecl(vl));
        };
        template <lex::ty... l >
        stmt::VarDecl getVarDecl(){
            stmt::VarDecl vdecl;
            for(;OneOfLex<lex::ty::Name>();nextTOK();){
                if(kw_in::check(lexitback().u.name)){vdecl.in=true;};
                else if(kw_out::check(lexitback().u.name)){vdecl.out=true;};
                else if(kw_inout::check(lexitback().u.name)){vdecl.in=true;vdecl.out=true;};
                else if(kw_const::check(lexitback().u.name)){vdecl.Const=true;};
                else {break;}
            };
            if(kw_Auto::check(lexitback().u.name)){vdecl.Auto=true;}
            else if(kw_Decltype::check(lexitback().u.name)){
                pri::get<value>(vdecl.tp.back().inst)=handleDeclType();vdecl.tp.back().t=result::rValue;
            }
            else {vdecl.tp = get_value<value::ty::typeValue>();}
            if constexpr (lang == language::cpp ){
                for(nextTOK();OneOfLex<lex::ty::mul>();nextTOK()){vdecl.ptrNum++;}
                for(nextTOK();OneOfLex<lex::ty::band>();nextTOK()){vdecl.refNum++;}
            }
            expectErr<lex::ty::Name>();{vdecl.name=lexitback().u.name;nextTOK();}
            if(lexitback().t==lex::ty::eq){vdecl.DefaultValue=getExpr<l...>();return vdecl;}
            return vdecl;
        };
        stmt::arg_list getArgList(){  stmt::arg_list args;
                for(;!OneOfLex<lex::ty::rparen>();nextTOK()){
                    if constexpr (lang==language::spv){while(kwFound<KW_LYTQ,kw_Const>){nextTOK();}}
                    if constexpr (lang==language::cpp){while(kwFound<kw_Const>){nextTOK();}}
                    args.push_back(getExpr<lex::ty::comma,lex::ty::rparen>());
                };
                return args;
        };
        stmt::init_args getArgInit(){stmt::init_args args;
            for(;!OneOfLex<lex::ty::rparen>();nextTOK()){args.push_back(getExpr<lex::ty::comma,lex::ty::rparen>());};return args;};
        template <lex::ty until>
        stmt::init_args getInitArgs();{
            stmt::init_args res(value::ty::initlist);
            for(nextTOK();!OneOfLex<until>();nextTOK()){res.push_back(getExpr<lex::ty::comma,until>());}
            return res;
        };

        template <lex::ty... l> 
        expr getExprName(){expr res;
            if(OneOfKw<kw_New>()){res.push<op::ty::opNew,expr::node::opty::prefixUnary>();nextTOK();}
            else if(OneOfKw<kw_Delete>()){res.push<op::ty::opNew,expr::node::opty::prefixUnary>();nextTOK();}
            else if(OneOfKw<kw_Noexcept>()){res.push<op::ty::opNoExcept,expr::node::opt::prefixUnary>();
                nextTOK();expectErr<lex::ty::lparen>();ret.emplace( getExpr<lex::ty::rparen>());return ret;
            }
                accMember_list accl = get_accMember_list<l...>();
                if(OneOfLex<lex::ty::lparen>()){stmt::arg_list argl=getArgInit();
                    res.emplace(value(value::FuncCall(accl,argl),value::ty::funcCall));
                }
                else if(OneOfLex<lex::ty::lbrack){ret.emplace(value(getInitArgs<lex::ty::rbrack>()),value::ty::initlist);}
            return res;
        };
        
        attrib_list _atlist ;
        attrib::argList getAtArgsList(){
            attrib::argList res;
            for(nextTOK();lexitback().t!=lex::ty::rparen;nextTOK()){
                if(lexitback().t==lex::ty::comma){continue;}
                res.push_back(lexitback());};nextTOK();return res;
        };
        attrib_list getAttribs(){
            attrib_list atlist;
            nextTOK();
            expectWarn<lex::ty::Name>();
            if(kw_Using::check(lexitback().u.name)){
                nextTOK();expectErr<lex::ty::Name>();
                atlist.useNs=true;atlist.nsUse=lexitback().u.name;
                nextTOK();expectErr<lex::ty::colon>();

                for(nextTOK();lexitback().t!=lex::ty::rdi){
                    if(lexitback().t==lex::ty::Name){atlist.emplace_back(lexitback().u.name);
                        nextTOK();}
                    if(lexitback().t==lex::ty::lparen){atlist.back().args=getAtArgsList();continue;}                      
                }
                expectErr<lex::ty::Name>();
            };
            else {
                for(nextTOK();lexitback().t!=lex::ty::rdi){
                    if(lexitback().t==lex::ty::Name){atlist.emplace_back(lexitback().u.name);
                        nextTOK();if(lexitback().t!=lex::ty::dcolon){atlist.back().Ns=true;nextTOK();
                            expectWarn<lex::t::Name>();
                            atlist.back().nsacc=atlist.back().nsacc;
                            atlist.back().name=lexitback().u.name;}
                    }
                    if(lexitback().t==lex::ty::lparen){atlist.back().args=getAtArgsList();continue;}                      
                }
            }
            return atlist;
        };
        value getRef(){
            resty res;
            result res;
            std::string name=lexitback().u.name;
            nextTOK();if(!OneOfLex<lex::ty::dcolon,>())
        };
        lambda<temp::meta>::captureList getCaptures(){
            lambda<temp::meta>::captureList res;bool byRef=false;bool arg=false;
            for(nextTOK();lexitback().t!=lex::ty::rbrack;nextTOK()){
                if(lexitback().t==lex::ty::Name){
                    arg=true;
                };
                if(lexitback().t==lex::ty::band){byRef=true;}
                if(lexitback().t==lex::ty::eq){res.copy=true;}
                if(lexitback().t==lex::ty::comma){
                    if(arg){
                        try{res.push(byRef,vl)}catch(const RepeatedName& e){err::e(*this,e);};}
                }
                    else if(byRef){res.byCopy=false;}
            };
        }
    
        value getLambda(){lambda<temp::meta> res;
            until<lex::ty::lbrack>();
            lambda<temp::meta>::captureList clist=getCaptures;
            nextTOK();stmt::arg_list l;res.
            if(lex::ty::ltangle){res.Template=true;res.plist=get<temp::meta,param_list>();cast->curtemp.push_back(&res.plist);nextTOK();}
            if(lex::ty::ldi){res.front_ats=getAttribs();nextTOK();}
            if(lex::ty::lparen){l=getArgList();nextTOK();}
            else{err::e(*this,UnexpectedToken<lambda<temp::meta>>());}
            if(lex::ty::Name){if(kw_Noexcept::check(lexitback().u.name)){res.Noexcept=true;};nextTOK();}
            if(lex::ty::ldi){res.back_ats=getAttribs();nextTOK();}
            if(lex::ty::arrow){res.trailing=true;nextTOK();res.rettp=get_accMember_list<lex::ty::lbrace>();nextTOK();}// NOTE reconsider for C++26 feature: ({pre,post} contract specifiers)
            if(lex::ty::lbrace){for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){Stmt();}}
            else{err::e(*this,UnexpectedToken<lambda<temp::meta>>());}
            if(res.Template){cast->curtemp.pop_back();}
            value vl;vl.t=value::lambdav;pri::get<lambda<temp::meta>>(vl.val)=res;
            return res;
        };
        template <lex::ty... l>
        void ContinueExpr(expr& res){
            if(lexitback().t==lex::ty::lbrack){res.tree.val = getLambda();return res;};
            auto prevName = [&res](){return res.e.back() };
            auto unary = <op::ty opT>[&](){if(prevName()){res.add<opT,expr::node::opty::postfixUnary>()}else{res.push<opT,expr::node::opty::prefixUnary>()}}
            stmt::FuncDecl fdecl;type<temp::inst> restt;
            for(;!OneOfLex<l...>(lexptrback()->t);nextTOK()){
                switch(lexitback().t){
                    case lex::ty::dcolon : {res.push(getExprName<l...>());}
                    case lex::ty::Name :{
                        if(lexitback().u.name==std::string("true")){res.pushLiteral(true);break;}
                        if(lexitback().u.name==std::string("false")){res.pushLiteral(false);break;}
                        res.emplace(getExprName<l...>());}
                    case lex::ty::Numflt:{res.pushLiteral(lexitback().u.flt);};
                    case lex::ty::Numuint:{res.pushLiteral(lexitback().u.num);};
                    case lex::ty::bnot : {res.push<op::ty::opbnot>();break;};
                    case lex::ty::band:{res.add<op::ty::oband>();break;}
                    case lex::ty::dot:{res.add<op::ty::opdot,expr::node::opty::binary>();break;}
                    case lex::ty::arrow:{res.add<op::ty::arrow,expr::node::opty::binary>();break;}
                    #ifdef CHAR_LITERALS
                    case lex::ty::sq :{res.pushLiteral(lexitback().u.chr);}
                    case lex::ty::dq :{res.pushLiteral(lexitback().u.name);}
                    #endif
                    case lex::ty::lparen:{nextTOK();
                        if(!res.e.back().TrailOp()){res.addFuncCall(getArgInit());};
                        else{res.pushExpr(getExpr<lex::ty::rparen>());break;}}
                    case lex::ty::rparen:{}
                    case lex::ty::lbrace:{ res.emplace(getInitArgs<lex::ty::rbrace>());break;}
                    case lex::ty::lbrack:{res.push<op::ty::oindex>(); res.push(getExpr<lex::ty::rbrack>());break;}// TODO C++23 support multiple index arguments
                    case lex::ty::Not:{res.push<op::ty::opNot,expr::node::opty::prefixUnary>();break;}
                    case lex::ty::plus:{res.add<op::ty::oplus>();break;}
                    case lex::ty::minus:{res.add<op::ty::ominus>();break;}
                    case lex::ty::bor:{res.add<op::ty::obor>();break;}
                    case lex::ty::bxor:{res.add<op::ty::obxor>();break;}
                    case lex::ty::mul:{res.add<op::ty::omul>();break;}
                    case lex::ty::div:{res.add<op::ty::odiv>();break;}
                    case lex::ty::ltangle:{if(!atTemplateType){res.add<op::ty::olt>()};break;}
                    case lex::ty::lteq:{res.add<op::ty::ogt>();break;}
                    case lex::ty::gtangle:{res.add<op::ty::ogt>();break;}
                    case lex::ty::gteq:{res.add<op::ty::ogteq>();break;}
                    case lex::ty::comma:{err::e<err::t::unexpectedToken>(*this);break;}
                    case lex::ty::semicolon:{return res;}
                    case lex::ty::colon:{res.add<op::ty::opcolon>();break;}
                    case lex::ty::space:{break;}
                    case lex::ty::cond:{res.add<op::ty::cond,expr::node::opty::ternary>();atCond=true;
                            nextTOK();res.emplace(getExpr<lex::ty::colon,l...>());expectErr<lex::ty::colon>();
                            res.push<op::ty::opcolon,expr::node::opty::binary>();
                            nextTOK();if(OneOfLex<lex::ty::lparen>()){res.emplace(getExpr<lex::ty::rparen,l...>());expectErr<lex::ty::rparen>();nextTOK();}                        
                        break;}
                    case lex::ty::eq:{res.add<op::ty::eq>();break;}
                    case lex::ty::peq:{res.add<op::ty::opeq>();break;}
                    case lex::ty::meq:{res.add<op::ty::omeq>();break;}
                    case lex::ty::xoreq:{res.add<op::ty::opxoreq>();break;}
                    case lex::ty::andeq:{res.add<op::ty::opandeq>();break;}
                    case lex::ty::oreq:{res.add<op::ty::oporeq>();break;}
                    case lex::ty::Noteq:{res.add<op::ty::opNoteq>();break;}
                    case lex::ty::muleq:{res.add<op::ty::opmuleq>();break;}
                    case lex::ty::diveq:{res.add<op::ty::opdiveq>();break;}
                    case lex::ty::pack:{unary<op::ty::oppack>();break;}
                    case lex::ty::oand:{res.add<op::ty::opand>();break;}
                    case lex::ty::oor:{res.add<op::ty::opor>();break;}
                    case lex::ty::three:{res.add<op::ty::opthree>();break;}
                    case lex::ty::pp:{unary<op::ty::opp>();break;}
                    case lex::ty::mm:{unary<op::ty::omm>();break;}
                }
            }
            return res;
        };
        
        template <lex::ty... l>
        expr getExpr(){expr res;return ContinueExpr<l...>(res);};
        
        template <> arg_list<temp::meta> get<stmt::arg_list>();

        template <typename STMTty>
        void getStmt();
        template <typename STMTty,bool Strct,bool Func>
        void _getStmt();

        template <bool Strct , bool Func>
        void Stmt();
        
        template < bool Strct>
        void FStmt();        

        template <bool Strct,bool Func>void _getStmt<stmt::block,Strct,Func>(){
            for(;lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>();};
            cast->popbl();
        };  
        void getBody(){
            lexq.push_back(lexitback());
            _getStmt<stmt::block>();
            erase()
        };
        std::vector<qual> quals;
        void pushQual(qual q){quals.push_back(quals);};
        template <typename QualifT>
        void pubQuals(QualifT* res){
            try {res->push(quals);}
            catch (const WrongQualifier& e) {err::e(*this,e);}
            quals.clear();}
        template <bool Strct,bool Func>void _getStmt<stmt::NS,Strct,Func>(){
            for(++lexptrback();lexptrback().t!=lex::ty::lbrace;++lexptrback()){
                if(lexptrback().t==lex::ty::Name){cast->emplace_back<stmt::NS>(it->u.name;);
                    if(!cast->pushNS(pri::get<stmt::NS>(cast->curBlock()->back()))){
                        err::e<err::t::namespaceNotAllowedInCurrentScope>(*this);};continue;}
                if(!OneOfLex<lex::ty::space,lex::ty::nl>(lexptrback().t)){err::e<err::t::unexpectedToken>(*this);}
            };
            open<lex::ty::lbrace>();
        };
        void pushats(attrib_list* atptr){if(!_atlist.empty()){!atptr=_atlist;}}
        template <bool Strct,bool Func>void _getStmt<stmt::While,Strct,Func>(){
        
            stmt::While* res=cast->pushStmt<stmt::While>();pushats(&res->atlist);
            nextTOK();expectErr<lex::ty::lparen>();nextTOK();res->condition=getExpr<lex::rparen>();
            nextTOK();expectErr<lex::ty::lbrace>();
            for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>();}
            nextTOK();
            cast->popbl();
        };
        template <bool Strct,bool Func>void _getStmt<stmt::For,Strct,Func>(){
            stmt::For* res=cast->pushStmt<stmt::For>();pushats(&res->atlist);
            nextTOK();expectErr<lex::ty::lparen>();
            itPtr.push(lexptrback());
            for(nextTOK();!OneOfLex<lex::ty::semicolon,lex::ty::colon>();nextTOK()){}
            nextTOK();res->init=getExpr<lex::ty::semicolon,lex::ty::colon>();
            if(OneOfLex<lex::ty::colon>()){itPtr.pop();
                stmt::ForRange* res=cast->pushStmt<stmt::ForRange>(stmt::ForRange());
                nextTOK();
                res->vardecl=getVarDecl<lex::ty::colon>();nextTOK();
                res->ref=getExpr<lex::ty::rparen>();nextTOK();
            }
            else{stmt::For* res=cast->pushStmt<stmt::For>(stmt::For());
                if(lexitback().t!=lex::ty::semicolon){res->init=getExpr<lex::ty::semicolon>();nextTOK();}
                else{nextTOK();}
                if(lexitback().t!=lex::ty::semicolon){res->condition=getExpr<lex::ty::semicolon>();nextTOK();}
                else {nextTOK();}
                if(lexitback().t!=lex::ty::rparen){res->incr=condition<lex::ty::rparen>();}                                
            }
            nextTOK();expectErr<lex::ty::lbrace>();
            for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>();}
            nextTOK();
            cast->popbl();

        };
        template <bool Strct,bool Func>void _getStmt<stmt::Do,Strct,Func>(){
            stmt::Do* res=cast->pushStmt<stmt::Do>(stmt::Do());pushats(&res->atlist);
            nextTOK();
            if(lexitback().t==lex::ty::lbrace){
                for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt();}
            }
            else FStmt<Strct>();
            nextTOK();
            if(kw_While::check(lexitback().u.name)){
                nextTOK();expectErr<lex::ty::lparen>();
                res->condition=getExpr<lex::ty::rparen>();
            }
            else {err::e(*this,UnexpectedToken());}
            cast->popbl();
        };
        
        template <bool Strct,bool Func>void _getStmt<stmt::Switch,Strct,Func>(){
            stmt::Switch* res=casts->pushStmt<stmt::Switch>(stmt::Switch()) ;pushats(&res->atlist);
            nextTOK();
            expectErr<lex::ty::lparen>();
            nextTOK();res->ex=getExpr<lex::ty::rparen,lex::ty::semicolon>();
            if(OneOfLex<lex::ty::semicolon>()){res->inexpr=ex;res->ex=getExpr(lex::ty::rparen);}
            nextTOK();
            expectErr<lex::ty::lbrace>();
            
            for(nextTOK();OneOfLex<lex::ty::rbrace>();nextTOK()){bool Blck=false;
                if(OneOfKw<kw_Case>()){nextTOK();res->css.emplace_back(getExpr<lex::ty::colon>());nextTOK();
                    if(OneOfLex<lex::ty::lbrace>()){Blck=true;nextTOK();};cast->curBl.push_back(&res->css.back().body);}
                else if(OneOfKw<kw_Default>()){nextTOK();expectErr<lex::ty::colon>();nextTOK();
                    if(OneOfLex<lex::ty::lbrace>()){Blck=true;nextTOK();}cast->curBl.push_back(&res->dflt);}
                else {FStmt<Strct>();continue;}
                for(nextTOK();(Blck?!OneOfLex<lex::ty::rbrace>():true) and !OneOfKw<kw_Case>();nextTOK()){FStmt<Strct>();}
                cast->popbl();Blck=false;
            }
            cast->popbl();
        };

        value ret_ty_val;// Used By various functions to Parse Function stmts;
        template <bool Strct,bool Func>void _getStmt<stmt::OperatorDecl,Strct,Func>(){
            nextTOK();

            stmt::OperatorDecl* res=cast->pushStmt<stmt::OperatorDecl>(stmt::OperatorDecl());pushats(&res->atlist);
            res->opt=getOperator();
// line 584
            if(opt==op::ty::opType){res->opv=get_accMember_list<lex::lparen,lex::ty::semicolon>();}
            nextTOK();res->args=getArgList();
            nextTOK();
            if(lexitback().t==lex::ty::semicolon){return;}
            else if(lexitback().t==lex::ty::lbrace){

            }
            pubQuals();
        };
        template <bool Strct,bool Func>void _getStmt<stmt::Try,Strct,Func>(){
            stmt::Try res; 
            for(nextTOK();OneOfLex<lex::ty::space,lex::ty::nl>();nextTOK()){}
            if(!OneOfLex<lex::ty::lbrace>()){err::e<err::t::unexpectedToken>(*this);}
            cast->pushbl(res.body);
            for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>();};
            cast->popbl();
            cast->pushStmt(res);
        };
        
        template <bool Strct,bool Func>void _getStmt<stmt::Catch,Strct,Func>(){
            cast->pushStmt<stmt::Catch>();
            stmt::Catch& res= pri::get<stmt::Catch>(*getStmt());
            cast->tries.top()->catches.push_back(&res);nextTOK();
            nextTOK();expectErr<lex::ty::lparen>();
            nextTOK();if(OneOfLex<lex::ty::ldi>()){res->atlist=getAttribs();}
            res.var=getVarDecl<lex::ty::rparen>();nextTOK();
            expectErr<lex::ty::lbrace>();
            for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>();}
            cast->popbl();nextTOK();
            if(lexitback().t==lex::ty::semicolon){
                cast->tries.pop_back();
            }
            kwFound<kw_Catch>();

        };
        template <bool Strct,bool Func>void _getStmt<stmt::Throw,Strct,Func>(){
            nextTOK();
            expr vl=getExpr<lex::ty::semicolon>();
            cast->pushStmt<stmt::Throw>(vl);
        };
        
        template <bool Strct,bool Func>void _getStmt<stmt::If,Strct,Func>(){bool ConstExpr=false;
            stmt::If* r=cast->pushStmt<stmt::If>(stmt::If(getExpr<lex::ty::rparen>())); pushats(&res->atlist);
            for(nextTOK();lexitback().t!=lex::ty::lparen;nextTOK()){
                if(OneOfKw<kw_ConstExpr>(lexitback().u.name)){ConstExpr=true;}
                else {err::e<err::t::unexpectedToken>(*this);}
            };
            if(ConstExpr){r.ConstExpr=true;}
            for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>();};
                        cast->popbl();

            
        };
        template <typename T> void Else(T* r){
            stmt* last=cast->getStmt();
            switch(last->t){
                case stmt::stmtty::eIf:{r->elIf=false;pri::get<stmt::If*>(r->Ifs)=&pri::get<stmt::If>(last->var);}
                case stmt::stmtty::eElseIf:{r->elIf=true;pri::get<stmt::ElseIf*>(r->Ifs)=&pri::get<stmt::ElseIf>(last->var);}
            }    
        
        }
        template <bool Strct,bool Func>void _getStmt<stmt::ElseIf,Strct,Func>(){bool ConstExpr=false;
            for(nextTOK();lexitback().t!=lex::ty::lparen;nextTOK()){
                if(checkName()){
                    if(OneOfKw<kw_ConstExpr>(lexitback().u.name)){ConstExpr=true;}
                    else {err::e<err::t::unexpectedToken>(*this);}
                }
            };
            stmt::ElseIf* r=cast->pushStmt<stmt::ElseIf>(stmt::ElseIf(getExpr<lex::ty::rparen>()));
            if(ConstExpr){r->ConstExpr=true;}
            Else<stmt::ElseIf>(r);
            for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>()};
                        cast->popbl();

        };
        template <bool Strct,bool Func>void _getStmt<stmt::Else,Strct,Func>(){
            nextTOK();
            if(kw_If::check(lexiback().u.name)){_getStmt<stmt::ElseIf>();return;}
            stmt::Else* r;
            r=cast->pushStmt<stmt::Else>(stmt::Else());pushats(&res->atlist);
            Else<stmt::Else>(r);
            if(lexitback().t==lex::ty::lbrace){for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>()};;}
            else {if(kw_Return::check(lexitback().u.name)){_getStmt<temp::meta>::Return>();};
            else{_getStmt<temp::meta>::Expr>();}
            }
            cast->popbl();
        };
        template <bool Strct,bool Func>void _getStmt<stmt::Return,Strct,Func>(){cast->pushStmt<stmt::Return>(getExpr<lex::ty::semicolon>());};
        template <bool Strct,bool Func>void _getStmt<stmt::TypeDef,Strct,Func>(){
            stmt::TypeDef* res=cast->pushStmt<stmt::TypeDef>(stmt::TypeDef());
            nextTOK();
            if(OneOfKw<kw_Decltype>()){res->expr=expr(handleDeclType());}
            else {res->expr=get_value<value::ty::typeValue>();nextTOK();expectErr<lex::ty::Name>();}
            res->name=lexitback().u.name;
        };
        
        template <stmt::Layout::stand st, Str s >struct lyt_std{
            stmt::Layout::stand t =st;
            static constexpr auto value = s;
            std::string name(){return std::string(value.data.data());}

        };
        using std430 = lyt_std<stmt::Layout::stand::std430,"std430">;
        using std140 = lyt_std<stmt::Layout::stand::std430,"std140">;

        template <typename STD,typename... STDs>
        bool getStd(stmt::Layout& s){
            if(getStd(s)){return true;}
            return getStd<STDs...>(s);
        };
        template <typename STD>
        bool getStd(stmt::Layout& s){
            if(lexptrback()->u.name==STD::name()){s.st=STD::t;return true;};
            return false;
        };
        
        // using getLytAt = BoolFunc<stmt::Layout,getLytAt,lyt_at,lyt_ats...>;
        template <typename lyt_at,typename... lyt_ats>
        bool getLytAt(stmt::Layout& s){
            if(getLytAt<lyt_at>(s)){return true;};
            return getLytAt<,lyt_ats...>(s);
        };
        template <typename lyt_at>
        bool getLytAt(stmt::Layout& s){
            if(lexptrback()->u.name==lyt_at::name()){
                s.t=lyt_at::lytTy;
                lexres unexp=untilExp<lex::ty::eq,lex::ty::space>();
                unexp+=untilExp<lex::ty::NumUint,lex::ty::space>();
                if(!unexp.empty()){err::e<err::t::unexpectedToken>(*this,unexp);};
                s.loc=lexptrback().u.unum;
                return true;
            };
            return false;
        };
        void getLytVar(stmt::Layout& lyt ){
            for(;OneOfKw<kw_in,kw_out,kw_inout,>(lexitback().t);nextTOK()){
                if(checkName()){
                    if(kw_Uniform::check(lexitback().u.name)){lyt.uniform=true;}
                    if(kw_Flat::check(lexitback().u.name)){lyt.flat=true;}
                }
            };
            if(kw_in::check(lexitback().u.name)){lyt.in=true;}
            if(kw_inout::check(lexitback().u.name)){lyt.in=true;lyt.out=true;}
            if(kw_out::check(lexitback().u.name)){lyt.out=true;}
            lyt.vart=stmt::Layout::tyT::var;
            for(nextTOK();!OneOfLex<lex::ty::semicolon,lex::ty::lbrace>();nextTOK()){
                if(lex::ty::Name==lexitback().t){
                    if(kw_Buffer::check(lexitback().u.name)){lyt.vart=stmt::Layout::tyT::buffer;continue;};
                    if(kw_Uniform::check(lexitback().u.name)){lyt.vart=stmt::Layout::tyT::uniform;continue;};
                    if(lyt.vart==stmt::Layout::tyT::var){

                        cast->ptr.vd=&pri::get<stmt::VarDecl>(lyt.data);getStmt<stmt::VarDecl>() ;
                        lyt.name=pri::get<stmt::VarDecl>(lyt.data).name;
                        return;
                    }
                    else {lyt.name=name;}
                
                }
                else if(!OneOfLex<lex::ty::space,lex::ty::nl>()){err::e<err::t::unexpectedToken>(*this);}
            }
            if(lexitback().t==lex::ty::lbrace){cast->ptr.dt=pri::get<stmt::DefType>(lyt.data);getStmt<stmt::TypeDef>();}
        };
        template <bool Strct,bool Func>void _getStmt<stmt::Layout,Strct,Func>(){
            
            cast->pushStmt<stmt::Layout>();stmt::Layout& lyt=cast->layouts.back();
            nextTOK();
            if(lexitback().t!=lex::ty::lparen){err::e<unexpectedToken,stmt::Layout>(*this);}
            for(;lexptrback()->t==lex::ty::rparen;nextTOK()){
                lexres unexp=untilExp<lex::ty::eq,lex::ty::space>();
                if( checkName()) {
                    if(getStd<std430,std140>(lyt)){continue;}
                    if(getLytAt<kw_Location,kw_Binding>(lyt)){continue;};
                };
                if(!OneOfLex<lex::ty::comma,lex::ty::space,lex::ty::Name>(lexptrback()->t)){
                    err::e<unexpectedToken>(*this);
                };
            };
            getLytVar();
        };
        template <bool Strct,bool Func>void _getStmt<stmt::Expr,Strct,Func>(){cast->pushStmt<stmt::Expr>(GetExpr<lex::ty::semicolon>());};
        // attrib_list _atlist ;
        template <typename StmtTy,bool Strct,bool Func>
        void getStmt(){
            itPtr.push_back(lexptrback());
            _getStmt<StmtTy,Strct,Func>();
            cast->pushAts<StmtTy>(_atlist);_atlist=attrib_list();
            erase();
        };


        integralT getEnumIntegral(){
            integralT res;
            nextTOK();
            expectErr<lex::ty::Name>();
            if(kw_signed::check(lexitback().u.name)){res.s=integralT::sign::Unsinged;nextTOK();}
            else if(kw_unsigned::check(lexitback().u.name)){res.s=integralT::sign::Singed;nextTOK();expectErr<lex::ty::Name>();}
            else if(kw_short::check(lexitback().u.name)){}
            else if(kw_long::check(lexitback().u.name)){
                nextTOK();expectErr<lex::ty::Name>();
                if( kw_long::check(lexitback().u.name)){integralT::len::LongLong;nextTOK();}
                else{res.l=integralT::len::Long;}
                expectErr<lex::ty::Name>();
            }
                if(std::string("int") ==lexitback().u.name){res.t=integralT::ty::flt;}
                eles if(std::string("float") ==lexitback().u.name){res.t=integralT::ty::Int}
                eles if(std::string("double") ==lexitback().u.name){res.t=integralT::ty::Double}
                else if(std::string("char") ==lexitback().u.name){res.t=integralT::ty::Ch;}
                else {
                    result r;resty<temp::meta> rs;lex::ty acc=lex::ty::none;
                    ref_handle(acc,&r,&rs);
                    switch(r){
                        case result::rValue :{res.mt=pri::get<value>(rs);}
                        case result::rType : {res.mt=pri::get<type<temp::meta>*>(rs);}
                    }
                }
            return res;
        };

        void Enum(bool bTemp,stmt::param_list& plist){stmt::Enum res;
            nextTOK();
            if(lexitback().t==lex::ty::Name){if(kw_Struct::check(lexitback().u.name) or(kw_class::check(lexitback().u.name))){enclass=true;nextTOK();}};
            if(lexitback().t==lex::ty::ldi){res.ats=getAttribs();nextTOK();}
            res.name=lexitback().u.name;nextTOK();
            if(lexitback().t==lex::ty::colon){res.SpeqSeq=true;res.intt=getEnumIntegral();};
            nextTOK();
            stmt* ptr=cast->pushStmt(res);
            if(lexitback().t==lex::ty::lbrace){
                for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){
                    if(lexitback().t!=lex::ty::Name){
                        ptr->emplace_back(lexitback().u.name);
                        nextTOK();
                        if(lexitback().t==lex::ty::eq){ptr->back().cexprval=GetExpr<lex::ty::comma,lex::ty::rbrace>();}
                        if(lexitback().t!=lex::ty::comma){err::e(this,UnexpectedToken<stmt::Enum>());}
                        
                    }
                    else {err::e(this,UnexpectedToken<stmt::Enum>());};
                };
            }
        };

        template <bool Strct,bool Func>
        void func(accMember_list& rett,accMember& name,bool btemp,param_list<temp::meta>& plist,param_list<temp::meta>& pll,size_t refn=0,size_t ptrn=0){
            /// From Here
            stmt::FuncDecl* res; 
            if(btemp){cast->prms.push_back(&plist);}
            stmt::arg_list args = getArgList();nextTOK();
            if constexpr (Strct){while(kwFound<kw_Noexcept,kw_Final>()){nextTOK();}}
            else {while(kwFound<kw_Noexcept>()){nextTOK();}}

            
            bool Declared =(cast->find(name) and (name->r==reuslt::rFunc ))  ;
            stmt::FuncDecl* alr;
            if(Declared){alr = pri::get<stmt::FuncDecl*>(name.back().inst);}
            else {
                 res=cast->pushStmt<stmt::FuncDecl>(stmt::FuncDecl());
                 res->name=name.back();res->args=args;res->ret=rett;res.prms=plist;
            }
            stmt::FuncDef* r;                 
            if(OneOfLex<lex::ty::semicolon>() and Declared){err::e(*this,alr);}
            else if (OneOfLex<lex::ty::lbrace>()){
                try {r=alr->push(plist,pll) ;}
                catch (const AlreadyDefdSpec& e){err::e(*this,e);}
                pushats(r->atlist);
                cast->curBl.push_back(&r->body);
                for(nextTOK();!OneOfLex<lex::ty::rbrace>();nextTOK()){FStmt<Strct>();};
            } ;
            if(cast->nss.size()==1 and (cast->strcts.size()==0)){if(name.back().name == sys.entry_pt){cast->entry_pt = r;}}
            pushats(res->atlist);
        };

        void funcCall(accMember_list& n,bool temp=false ,param_list<temp::inst> plist={}){
            cast->find<stmt::FuncDecl> fdecl(n);
        };
        template <bool Strct,bool Func>
        void var(accMember_list& rett,accMember_list& name,bool btemp,param_list<temp::meta>&& plist,size_t refn=0,size_t ptrn=0){// CalledFrom eq or ;
            stmt::VarDecl* res ;
            if(name.size()==1 and !name.globalAcc!=true){
                res=cast->pushStmt<stmt::VarDecl>(stmt::VarDecl(rett,name.front().name,refn,ptrn));
                pubQuals(res);
            }
            else {
                res=cast->pushStmt<stmt::VarDecl,Strct>(stmt::VarDecl());
                stmt::VarDecl& to = *(--name.tail());
                res=pushStmtTo<stmt::VarDecl>(stmt::VarDecl,Strct>(&to,VarDecl(rett,name,refn,ptrn)));
            }
            if(OneOfLex<lex::ty::semicolon>()){return;}
            if(OneOfLex<lex::ty::eq>()){res->Default = true;res->DefaultValue=getExpr<lex::ty::semicolon>();return;}
        };

        template <bool acc>
        struct AccDefault {static constexpr bool acc=Strct?accSpec::Public:accSpec::Private;}
        template <bool acc>
        type<temp::meta>::inher_list getInherList(){
            type<temp::meta>::inher_list res;
            for(nextTOK();!OneOfLex<lex::ty::semicolon,lex::ty::lbrace>()){
                expectErr<lex::ty::Name>();
                if(lexitback().t==lex::ty::Name){
                    if(kw_Public::check(lexitback().u.name)){res.emplace_back(accSpec::Public);nextTOK();}
                    else if(kw_Protected::check(lexitback().u.name)){res.emplace_back(accSpec::Protected);nextTOK();}
                    else if(kw_Private::check(lexitback().u.name)){res.emplace_back(accSpec::Private);nextTOK();}
                    else {res.emplace_back(get_accMember_list<lex::ty::comma,lex::ty::semicolon,lex::ty::lbrace>(),AccDefault<acc>::acc);nextTOK();}
                    if(lexitback().t==lex::ty::Name){ res.back().data=get_accMember_list<lex::ty::comma,lex::ty::semicolon,lex::ty::lbrace>();nextTOK();}
                }
            };
            return res;
        };

        template <bool accPubblic,bool Strct>
        void checkStrct(bool bTemp=false,stmt::param_list& plist={}){
             accMember_list accl;
                    try{accl = get_accMember_list<lex::ty::semicolon>();}
                    catch (const accMemberStop& e){
                        accMember_list acc;size_t ptrNum=0;size_t refNum=0; 
                        if constexpr(lang==language::cpp){
                            for(;OneOfLex<lex::ty::mul>();nextTOK()){ptrNum++;}
                            for(;OneOfLex<lex::ty::band>();nextTOK()){refNum++;}
                        }
                        if (OneOfLex<lex::ty::Name>()){
                            try{acc = get_accMember_list<lex::ty::semicolon>();}
                            catch (const accMemberStop& e){}
                            if(acc.size()>1){expectErr<lex::ty::mul>(); nextTOK();expectErr<lex::ty::Name>();
                                stmt::VarDecl* res = cast->pushStmt<stmt::VarDecl>(stmt::VarDecl(accl,acc,lexitback().u.name),refNum,ptrNum);   
                            }
                            else {stmt::VarDecl* res = cast->pushStmt<stmt::VarDecl>(stmt::VarDecl(accl,acc.back().name,refNum,ptrNum));
                                nextTOK();
                                if(OneOfLex<lex::ty::eq>()){nextTOK();res->DefaultValue=getExpr<lex::ty::semicolon>();return;}
                                else if(OneOfLex<lex::ty::semicolon>()){return;}
                            }
                        }
                        else {
                            if(OneOfLex<lex::ty::semicolon>()){

                            }
                            else if(OneOfLex<lex::ty::dcolon>()){

                            }
                        }

                }
        };
        
        template <bool tdef,bool accPublic,bool Strct,bool Union=false>
        void _Strct(bool bTemp=false,stmt::param_list& plist=NULL){// TODO Union Constexpr
            curPtr.push_back(lexptr.back());
            nextTOK();
            if constexpr (tdef) {
                stmt::TypeDef* res = cast->pushStmt<stmt::TypeDef>(stmt::TypeDef());
                if(OneOfLex<lex::ty::lbrace>()){
                    stmt::DefType dt;res->anon=true;
                    type<temp::inst>* tp=dt->tp;cast->strcts.push_back(&dt);
                    for(nextTOK();!OneOfLex<lex::ty::rbrace>();nextTOK()){PushStmt<true,false>();}
                    res->anontp=*tp ;
                }
                else {
                    if(OneOfLex<lex::ty::Name,lex::ty::dcolon>()){
                        accMember_list accl;
                        try { accl = get_accMember_list<lex::ty::semicolon>()};
                        catch (const accMemberStop& e){res->tp = e.what();}
                        res->tp = accl;
                        nextTOK();expectErr<lex::ty::Name>();
                        res->name=lexitback().u.name;
                    }
                    else {err::e(*this,UnexpectedToken());}
                }
            }
            else {
                    accMember_list accl;
                    try{accl = get_accMember_list<lex::ty::semicolon>();}
                    catch (const accMemberStop& e){
                        accMember_list acc;size_t ptrNum=0;size_t refNum=0; 
                        if constexpr(lang==language::cpp){
                            for(;OneOfLex<lex::ty::mul>();nextTOK()){ptrNum++;}
                            for(;OneOfLex<lex::ty::band>();nextTOK()){refNum++;}
                        }
                        if(OneOfLex<lex::ty::Name>()){
                            try{acc = get_accMember_list<lex::ty::semicolon>();}
                            catch (const accMemberStop& e){}
                            if(acc.size()>1){
                                expectErr<lex::ty::mul>(); nextTOK();expectErr<lex::ty::Name>();
                                stmt::VarDecl* res = cast->pushStmt<stmt::VarDecl>(stmt::VarDecl(accl,acc,lexitback().u.name),refNum,ptrNum);   
                            }
                            else {
                                param_list<temp::inst> pll;if (OneOfLex<lex::ty::ltangle>()){pll=get<temp::inst,param_list>();nextTOK();}
                                if(OneOfLex<lex::ty::lparen>()){func(accl,acc,bTemp,plist,pll,refNum,ptrNum);}
                                else {
                                    stmt::VarDecl* res = cast->pushStmt<stmt::VarDecl>(stmt::VarDecl(accl,acc.back().name,refNum,ptrNum));
                                    nextTOK();
                                    if(OneOfLex<lex::ty::eq>()){nextTOK();res->DefaultValue=getExpr<lex::ty::semicolon>();return;}
                                    else if(OneOfLex<lex::ty::semicolon>()){return;}
                                }
                            }
                        }
                        else {
                            type<temp::meta>::inher_list inh;
                            param_list<temp::inst> pll;
                            bool exists = cast->exists(accl);
                            stmt::DefType* res;
                            if (accl.back().Template){
                                stmt::SpecTreeIncl<stmt::DefType>::specN* spec;
                                try{res=pri::get<stmt::DeclType*>(spec=accl.back().inst)->push(plist,accl.back().plist);res=spec.t;cast->strcts.push_back(res);cast->curtemp.back()=&spec->tprms;}
                                catch(const AlreadyDefdSpec<stmt::DefType>& e){err::e(*this,e);}
                                nextTOK();
                                if(OneOfLex<lex::ty::colon>()){nextTOK();res->curacc=accPublic?accSpec::Public:accSpec::Private;
                                     res->inherits=getInherList<accPublic>();}
                                expectErr<lex::ty::lbrace>();
                                for(nextTOK();OneOfLex<lex::ty::rbrace>();nextTOK()){StmtPush<true,false>();}
                                cast->strcts.pop_back();cast->curtemp.pop_back();return;
                            }
                            //// TODO around here
                            stmt::DeclType* alr;
                            if(OneOfLex<lex::ty::colon>()){nextTOK();inh= getInherList<Strct>();}
                            if(OneOfLex<lex::ty::semicolon>()){
                                if(exists){err::e(*this,Redeclaration());}
                                else {
                                    if(accl.size()==1){
                                         alr = cast->pushStmt(stmt::DeclType(accl.back().name));
                                         alr->specs.front().t.inherits=inh;
                                    }
                                    else { err::e(*this,DeclarationOutsideScope());} 
                                }
                            }
                            else if(OneOfLex<lex::ty::lbrace>()){
                                if(exists){
                                    res = &alr->push(plist,pll); 
                                    cast->strcts.push_back(res);res->inherits =inh;
                                    for(nextTOK();!OneOfLex<lex::ty::rbrace>();nextTOK()){PushStmt<true,false>();}
                                    cast->strct.pop_back();
                                }
                                else {
                                    res = &alr->specs.front().t; 
                                    cast->strcts.push_back(res);res->inherits =inh;
                                    for(nextTOK();!OneOfLex<lex::ty::rbrace>();nextTOK()){PushStmt<true,false>();}
                                    cast->strct.pop_back();

                                }
                            };
                        }

                    }
                }
            erase();return;
        };
        template <bool tdef,bool Strct>
        void Union(bool bTemp=false,param_list<temp::meta>& plist=NULL){_Strct<tdef,Strct,true>(bTemp,plist);};
        template <bool Strct>
        void Using(bool bTemp=false,param_list<temp::meta>& plist=NULL){
            nextTOK();
            if(OneOfKw<kw_Namespace>()){res.Namespace=true;nextTOK();
                bool global=false; 
                if(OneOfLex<lex::ty::dcolon>()){global=true;nextTOK();}
                    pri::deque<std::string> nsName;
                for(;!OneLex<lex::ty::semicolon>();nextTOK()){
                    expectErr<lex::ty::Name>();nsName.push_back(lexitback().u.name);nextTOK();
                    if(OneOfLex<lex::ty::dcolon){continue;}
                };
                try {cast->pushUsingNS(global,nsName);}
                catch (const NameNotFound& e){err::e(*this,e);}
                return ;
            }
            
            stmt::Using* res=cast->pushStmt<stmt::Using>(stmt::Using());
            if(OneOfKw<kw_Typename>()){res->Typename=true;nextTOK();};pushats(res->atlist);
            expectErr<lex::ty::Name>();res.name= lexitback().u.name;nextTOK();
            expectErr<lex::ty::eq>();nextTOK();
            res->expr = get_accMember_list<lex::ty::semicolon>();
        };        
        template <bool Strct,bool Func> 
        void NameStmt(bool bTemp,param_list<temp::meta>& plist){  
            if(!OneOfLex<lex::ty::Name,lex::ty::dcolon>()) {
                if(Func){cast->pushStmt(getExpr());return;};
                else {err::e(*this , UnexpectedToken());}
            }
            accMember_list acclist ;
            size_t refNum=0;size_t ptrNum=0;
                try {acclist = get_accMember_list();}
                catch ( const accMemberStop& e){
                    if constexpr (lang == language::cpp){
                        for(;OneOfLex<lex::ty::mul>();nextTOK()){ptrNum++;}
                        for(;OneOfLex<lex::ty::ref>();nextTOK()){refNum++;}
                    }
                    if(OneOfLex<lex::ty::eq>() or pri::OneOf<lex::ty ,lex::ty::dot,lex::ty::arrow >(acclist.back().acc)){
                        expr ex;ex.emplace(value(acclist));
                        cast->pushStmt<stmt::Expr>(ContinueExpr(ex));return;
                    }
                }
                if(!OneOfLex<lex::ty::Name>()){ 
                    // Check For fptr;
                    if(OneOfLex<lex::ty::lparen>()){isFuncPtr(acclist);return;;} 
                    expr ex;ex.emplace(value(acclist));
                    cast->pushStmt<stmt::Expr>(ContinueExpr(ex));return;
                }
                else {
                    accMember_list nm ;
                    try {nm = get_accMember_list();}
                    catch ( const accMemberStop& e){}
                    if(OneOfLex<lex::ty::mul>()){
                        nextTOK();expectErr<lex::ty::Name>();
                        cast->pushStmt(stmt::VarDecl(acclist,nm,lexitback().u.name,refNum,ptrNum));return;
                    }
                    if(OneOfLex<lex::ty::lparen>()){func<Strct,Func>(acclist,nm,nm.back().Template,plist,nm,refNum,ptrNum);return;}
                    
                    if(nm.size()==1 and OneOfLex<lex::ty::semicolon,lex::ty::eq>()){
                    stmt::VarDecl res=cast->pushStmt<stmt::VarDecl>(stmt::VarDecl()) ; 
                    res->ptrNum=ptrNum;res->refNum=refNum;res->name =nm.back().name;
                    acclist.globalAccess;
                    res->tp=acclist;
                    if(OneOfLex<lex::ty::eq>()){nextTOK();res->DefaultValue = getExpr<lex::ty::semicolon>();}
                    }
                    else err::e(*this,UnexpectedToken();)
                }
        };
        
        void include(std::filesystem::path pth){pos.push_back(posit());this->curFilePath.push_back(pth) ;this->f.push_back(std::ifstream(pth));};
        void includeRel(std::filesystem::path l){
            std::filesystem::path wd=cwd.back();
            std::filesystem::path pth = wd;pth/=l;
            if(std::filesystem::exists(pth)){
                parser pn;cast->include(pn.fromFile(pth,*this));}
            else {err::e<err::t::fileNotFound>(*this,pth);}
        };
        void includeAbs(std::filesystem::path l){
            for(std::filesystem::path p : dirs.arr ){
                std::filesystem::path pth = p;pth/=l;
                if(std::filesystem::exists(ps)){parser pn;cast->include(pn.fromFile(pth,*this));return;}
            };
            err::e<err::t::fileNotFound>(*this,pth);
        };
        template<>void _getMacro<mStmtInclude>(){
             itPtr.push(lexptrback());
            until<lex::ty::nl>();
            ++(lexptrback());
            if(lexptrback()->t==lex::ty::ltangle){
                ++(lexptrback());
                std::filesystem::path p;
                for(;lexptrback()->t!=lex::ty::gtangle;++(lexptrback())){
                    if(OneOfLex<lex::ty::div,lex::ty::escape>(lexptrback()->t)){
                        continue;
                    }
                    if(lexptrback()->t!=lex::ty::name){err::e<err::t::include_closing_angle_brackets>(*this);break;}
                    p/=std::filesystem::path(lexptrback()->u.name);
                }
                includeAbs(p);
            };
            else if(lexptrback()->t==lex::ty::dq){
                ++lexptrback();
                std::filesystem::path p;
                for(;lexptrback()->t!=lex::ty::dq;lexptrback()){
                    if(OneOfLex<lex::ty::div,lex::ty::escape>(lexptrback()->t)){
                        continue;
                    }
                    if(lexptrback()->t!=lex::ty::name){err::e<err::t::include_closing_dq>(*this);break;}
                    p/=std::filesystem::path(lexptrback()->u.name);
                }
                includeRel(p);
            }
            else {err<>();}            
            erase();
        };
        template<>void _getMacro<mStmtIf>(){
            // itPtr.push(lexptrback())
            cast->condition= getExprUntil_EOL();
            // cast->tus.emplace_back(ex);
            erase();
            untilKW<false,kw_Else,kw_elif,kw_elifdef,kw_elifndef,kw_endif>();
        };
        template<>void _getMacro<mStmtDefine>(){
            until<lex::ty::Name>();
            macros.emplace(lexptrback()->u.name,strippedUntil<lex::ty::nl>());
            erase();
        };
        template<>void _getMacro<mStmtElif>(){getMacro<mStmtIf>();};
        template <bool b>
        bool cond(bool t){return b?t:!t;};
        template <bool b>
        void IfDf(){
            itPtr.push_back(lexptrback());until<lex::ty::Name>();
            erase();
            if( cond<b>(macros.exists(ptr->u.name))){
                untilKW<true,kw_Else,kw_elif,kw_elifdef,kw_elifndef,kw_endif>();}
            else{untilKW<false,kw_Else,kw_elif,kw_elifdef,kw_elifndef,kw_endif>();}
        };
        
        template<>void _getMacro<mStmtIfdef>(){IfDf<true>();};
        template<>void _getMacro<mStmtElifdef>(){IfDf<true>();};
        template<>void _getMacro<mStmtIfndef>(){IfDf<false>();};
        template<>void _getMacro<mStmtElifndef>(){IfDf<false>();};

        template<>void _getMacro<mStmtElse>(){
            erase();
            cast->condition= !(cast->condition);
            if(cast->condition){
                untilKW<true,kw_Else,kw_elif,kw_elifdef,kw_elifndef,kw_endif>();
            }else {untilKW<false,kw_Else,kw_elif,kw_elifdef,kw_elifndef,kw_endif>();}
        };
        template<>void _getMacro<mStmtEndIf>(){erase();};
        template <typename T> getMacro(){_getMacro<T>();}


        template <bool Strct>
        void getOperator(bool bTemp,param_list<temp::meta>& plist){
            nextTOK();
            expectErr<lex::ty::Name>();
            accMember_list accl ;
            try{accl = get_accMember_list();}
            catch (const accMemberStop& e){err::e(*this,e);}
            type<temp::meta>* res=cast->strcts.back().t;
            if(accl.size()>1){
                auto it=accl.tail();--it;
                switch(it->t){
                    case result::rType: {res=pri::get<stmt::DefType*>(it->inst)};
                    case result::rNs: {res=pri::get<stmt::NS*>(it->inst)};
                };
            };
            cast->strcts.back().operators.emplace_back(cast->strcts.back().curacc,);
        }
        void GetFriend(){nextTOK();
            
            nextTOK();bool bPack=false;bool blist=true;
            lexq.push_back(lexptrback());
            if(kw_Class::check(lexitback().u.name or kw_Struct::check(lexitback().u.name))){
                nextTOK();
                result r;resty<temp::meta> res;lex::ty acc=lex::ty::none;
                ref_handle(acc,&r,&res);
                reslt.t=mtype::ty::rType;pri::get<type<temp::meta>*>(reslt.var)=res.
            }
            else {accMember_list accl ;
                
                try {accl = get_accMember_list();}
                catch {}


                if(kw_Operator::check(lexitback().u.name)){
                op::ty r=getOperator();reslt.opt=r;
                if(r==op::ty::opType){
                    try {reslt.op = get_accMember_list<lex::ty::lparen,lex::ty::semicolon>();}
                    catch (const accMemberStop& e){err::e(*this,e);}

                }
                return reslt;
            };
            
            }
            
            for(;!OneOfLex<lex::ty::semicolon>();nextTOK()){
                if(lexitback().t==lex::ty::pack){bPack=true;}
                if(lexitback().t==lex::ty::comma){blist=true;}
                if(res.t==mtype::ty::Func){curTp->frndsFunctions.push_back(pri::get<stmt::FuncDecl*>(res.var));}
                if(res.t==mtype::ty::Oper){curTp->frndsOperators.push_back(pri::get<stmt::OperatorDecl*>(res.var));}
                if(res.t==mtype::ty::rType){curTp->frndsTypes.push_back(pri::get<stmt::DeclType*>(res.var));}
            };
            erase();
        };

        void Destructor(){
            nextTOK();expectErr<lex::ty::lparen>();
            nextTOK();expectErr<lex::ty::rparen>();
            if(kwFound<kw_Noexcept>()){nextTOK();};
            nextTOK();expectErr<lex::ty::lbrace>();cast->curBl.push_back(&cast->strcts.back().t.destructor);
            for(nextTOK();!OneOfLex<lex::ty::rbrace>();nextTOK()){StmtPush<true,true>();}
            nextTOK();
        };
        
        inline void getInit(stmt::Constructor* res){
            expectErr<lex::ty::Name>();
            type<temp::meta>::accMem<stmt::VarDecl>* r;
            try{r= cast->strcts.back().find<stmt::VarDecl>(lexitback().u.name);}
            catch (const NameNotFound<stmt::VarDecl>& e){err::e(*this ,e);}
            nextTOK();
            if(OneOfLex<lex::ty::lbrace>()){res.init_list.emplace_back(r,true,getInitArgs<lex::ty::rbrace>());}
            if(OneOfLex<lex::ty::lparen>()){res.init_list.emplace_back(r,false,getInitArgs<lex::ty::rparen>());}
        };
        void Constructor(bool bTemp,param_list<temp::meta>& plist){nextTOK();
            stmt::Constructor* res=cast->pushStmt<stmt::Constructor>(stmt::Constructor());
            if(bTemp){res->plist=plist;cast->curtemp.push_back(&res->plist)};
            if(kwFound<kw_Noexcept>()){nextTOK();}
            if(OneOfLex<lex::ty::ldi>()){res->atlist = getAttribs();nextTOK();}
            else {pushats(res->atlist)};
            
            res->pushQuals(quals);
            expectErr<lex::ty::lparen>();res->args = getArgList();

            nextTOK();
            if(OneOfLex<lex::ty::eq>()) {nextTOK();
                if(OneOfKw<kw_Default>()){res->Default=true;}
                else {err::e(*this,ExpectedKw<kw_Default>());}
                nextTOK();
                expectErr<lex::ty::semicolon>();return;
            }
            if(OneOfLex<lex::ty::colon>()){
                for(nextTOK();!OneOfLex<lex::ty::lbrace>();nextTOK()){
                    getInit(res);nextTOK();
                    if(OneOfLex<lex::ty::comma>()){continue;}
                    if(OneOfLex<lex::ty::lbrace>()){break;}
                };
            } 
            expectErr<lex::ty::lbrace>();
            cast->curBl.push_back(&res->body);
            for(nextTOK();!OneOfLex<lex::ty::rbrace>();nextTOK()){PushStmt<true,true>();};
            cast->curBl.pop_back();nextTOK();

            if(bTemp){cast->curtemp.pop_back();}; 
        };
        #define PUBLIC_ACCESS true
        #define PRIVATE_ACCESS false
        template <bool Strct,bool Func>
        bool StmtPush(){
            if(lexitback().t==lex::ty::lbrace){
                if constexpr (Func){
                    cast->pushStmt<stmt::block>(stmt::block);
                    for(nextTOK();OneOfLex<lex::ty::rbrace>();nextTOK()){StmtPush<Strct,Func>();}
                return true;
                }
                else throw UnexpectedToken(lexitback().t);
            };
            
            if(lexitback().t==lex::ty::ldi){_atlist = getAttribs(); nextTOK(); }
            if(lexitback().t==lex::ty::semicolon){nextTOK();return;}
            if(OneOfLex<lex::ty::dcolon>()){}
            else if(OneOfLex<lex::ty::bnot>()){
                nextTOK();
                if constexpr (Strct and !Func){
                    if(lexitback().u.name==cast->strcts.back().t.name){Destructor();return;} }
                    else {
                        expr ex;ex.push<op::ty::opbnot,node::opty::prefixUnary>();
                        ContinueExpr(ex);return;
                    }
            }
            if(OneOfLex<lex::ty::Name>()){
                if(kwFound<KW_LISTM>()){return true;}
                if constexpr (lang==language::cpp){while(kwFound<KW_QUAL,kw_Extern>()){nextTOK();}}
                else if constexpr (lang ==  langauge::stmsl){while(kwFound<KW_QUAL,KW_LYTQ,kw_Layout>()){nextTOK();}}
                stmt::param_list plist ;bool bTemp=false;
                if(OneOfKw<kw_Template>()){bTemp=true;
                    if constexpr (Func){err::e(*this,UnexpectedToken("Template Not allowed in function body"));}
                    nextTOK();expectErr<lex::ty::ltangle>();
                    plist = get<temp::meta,param_list>();until<lex::ty::Name,lex::ty::gtangle,lex::ty::space,lex::ty::nl>();
                    cast->curtemp.push_back(&plist);
                };
                if constexpr (Strct){
                    if constexpr(!Func){    if(lexitback().u.name==cast->strcts.back().t.name){Constructor(bTemp,plist);return;} }
                    if(OneOfKw<kw_Operator>()){GetOperator<Strct>(bTemp,plist);return;}
                    else if(OneOfKw<kw_Friend>()){GetFriend();return;}
                    else if(OneOfKw<kw_Public>()){cast->curAcc=accSpec::Public;nextTOK();expectErr<lex::ty::colon>();};
                    else if(OneOfKw<kw_Private>()){cast->curAcc=accSpec::Private;nextTOK();expectErr<lex::ty::colon>();};
                    else if(OneOfKw<kw_Protected>()){cast->curAcc=accSpec::Protected;nextTOK();expectErr<lex::ty::colon>();};
                }
                if constexpr (!Func) {
                    if(OneOfKw<kw_Typedef>()){
                        if(OneOfKw<kw_Struct>()){_Strct<true,PUBLIC_ACC,Strct>(bTemp,plist);}
                        else if(OneOfKw<kw_class>()){_Strct<true,PRIVATE_ACC,Strct>(bTemp,plist);}
                        else if(OneOfKw<kw_Union>()){Union<true,Strct>(bTemp,plist);}
                        else {
                            stmt::TypeDef* res = cast->pushStmt<stmt::TypeDef>(stmt::TypeDef());
                            try{ get_accMember_list<lex::ty::semicolon>()}
                            catch (const accMemberStop& e) {res->tp=e.what();}
                            res->name=lexitback().u.name;
                        }
                    }
                    else if(OneOfKw<kw_Enum>()){Enum(bTemp,plist);return;}
                    else if(OneOfKw<kw_Struct>()){_Strct<false,true,Strct>(bTemp,plist);return;}// Attbs after
                    else if(OneOfKw<kw_class>()){_Strct<false,false,Strct>(bTemp,plist);return;}
                    else if(OneOfKw<kw_Union>()){Union<false,Strct>(bTemp,plist);return;}
                    else if(OneOfKw<kw_Using>()){Using<Strct>(bTemp,plist);return;}
                }
            }
            NameStmt<Strct,Func>(bTemp,plist);
        };

        
        template <bool Strct , bool Func>
        void Stmt(){
            if(!kwFound<KW_LISTM>()){
            StmtPush<Strct,Func>();
            }
        }
        template <bool Strct>
        void FStmt(){Stmt<Strct,true>();};

        ast<temp::inst> fromFile(std::filesystem::path pth){ curFilePath.push_back(pth);
            cast=new ast();
        cwd.push(pth.parent_path());
        f.open(pth);
        pos.push_back(posit());bool fend=false;
        try {while(Line()){Stmt();}}
        catch (const FileEnd& e){fend=e.b;}
        if(pth.extension()==std::filesystem::path("hstmsl")){wrcph(pth,cur); }
        cwd.pop();
        parserStack.pop();
        return get_inst(cast);
        };
        ast<temp::inst> fromFile(std::string pth){filePos =0; std::filesystem::path s(pth);return fromFile(s); }
        parser() = default;
    
    }
}
#endif