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
    pri::stack<expr<inst>> condition;
   
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
        void Stmt();
        
       
        
        pri::stack<accSpec> curAcc;
        template <accSpec acc>void access(){curAcc.back()=acc;};
        template <accSpec acc>void accessPush(){curAcc.push(acc);};
        template <lex::ty tok>
        stmt<temp::meta>::block getBlockUntil(){
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
        void proc(accMember_list<q>& res){// TOD
            if constexpr (Global){res.globalAcc=true;nexTOK();}
            if(kw_Operator::check(lexitback().u.name)){
                res.emplace_back(accMember::result::rOperator);
                res.back().oprt=getOperator();}
                else {res.emplace_back(lexitback().u.name)
                if constexpr (Global){res.back().inst=cast->findNameFromNs(cast->global,lexitback().u.name,&res.back().r);}
                if constexpr (!Res){res.back().inst=cast->findName(lexitback().u.name,&res.back().r,&res.back().inst);}
                else {res.back().inst=cast->findName(lexitback().u.name,&res.inst,&res.back().r);}
            };
            nextTOK();
            if(OneOfLex<lex::ty::ltangle>()){res.back().tmplt=true;res.back().plist=get<temp::inst,param_list>();nextTOK();}
            if(OneOfLex<LEX_ACC>()){res.back().acc=lexitback().t};
            else {throw accMemberStop(res);}
        }
        template<temp q>
        void procName(accMember_list<q>& res){
            res.emplace_back(lexitback.u.name);nextTOK();
            if(OneOfLex<lex::ty::ltangle>()){res.back().plist=get<temp::inst,param_list>();nextTOK();}
            if(OneOfLex<LEX_ACC>()){res.back().acc=lexitback().t};
            else throw accMemberStop(res);}
        };
        template <lex::ty... l> accMember_list get_accMember_List(){
            try {
            accMember_list<q> res;res.acc=lex::ty::none;
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
        template <temp q> value<q> handleDeclType(){// TODO . tricky Must evaluate expressions 
            expr<q> res;nextTOK();
            accMember_list<q> accmem = get_accMember_list<lex::ty::rparen,lex::ty::lparen,lex::ty::>();
            if(lexitback().t==lex::ty::lparen){
                stmt<temp::meta>::arg_list argl = getArgList();nextTOK();
                if(lexitback().t==lex::ty::rparen){

                }
            };
            res.variable
        };
        // These two functions are invalid
        template <temp q> value<q>& get<q,value,value<q>::ty::typeValue>(){
            res.t=value<temp::inst>::ty::typeValue;
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
                    if constexpr (q==temp::meta){return pri::get<stmt<q>::Using>(rs).expr;}
                    else {return pri::get<stmt<q>::Using>(rs).expr;}
                }
                default :{throw TypeNotFound();};
            }
        };
        template <temp q> value<q>& get<q,value,value<q>::ty::ptrmember>(){
            value<q> res;res.t=value<temp::inst>::ty::ptrmember;
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
                    if constexpr (q==temp::meta){return pri::get<stmt<q>::VarDecl>(rs).tp;}
                    else {return value(pri::get<stmt<q>::VarDecl>(rs).tp);}
                }
                default throw NameNotFound();
            }
        };

         template <lex::ty... l> 
        expr<temp::meta> getExprName();
         template <lex::ty... l> 
        expr<temp::meta> getExpr();


       // TODO funcs again

        template <temp q> param<q>& get<q,param>(){
            param<q> res;
            for(;!OneOfLex<lex::ty::gtangle;nextTOK()){
                for(nextTOK();!OneOfLex<lex::ty::gtangle,lex::ty::comma>();nextTOK()){
                    expectErr<lex::ty::Name>();
                    if(OneOfKw<kw_Template>()){
                        nextTOK();res.tmplt=true;
                        if constexpr (q==temp::meta){expectErr<lex::ty::ltangle>();
                            res.pl=get<q,param_list,lex::ty::gtangle>();nextTOK();
                            expectErr<lex::ty::Name>();
                            if(OneOfKw<kw_Typename,kw_Class,kw_Struct>()){
                                res.t=param<q>::ty::Typename;nextTOK();expectErr<lex::ty::Name>();
                                res.name=lexitback().u.name;
                            }
                            else {res.t=param<q>::ty::PtrToMember;}
                        }
                        else {res.memberList=get_accMember_list<lex::ty::comma,lex::ty::gtangle>();}
                        continue;
                    }
                    if(OneOfKw<kw_Typename>()){res.t=param<q>::ty::Typename;
                        nextTOK();
                        if constexpr(q==temp::meta){//TODO left herr
                            res.t;
                        }
                        else {
                            if(!OneOfKw<kw_Decltype>()){
                                res.memberList=get_accMember_list<>();}
                            else {res.val=handleDeclType();}

                            }
                    };

                }
            };
        }
        template <> param<temp::inst>& get<temp::inst,param>(){
            if(!Until<lex::ty::Name,lex::ty::ltangle>()){};
            expr<temp::meta> values=getExpr<lex::ty::comma,lex::ty::rangle>();
            value<temp::inst>& v=values.val();
            param<temp::inst> res;
            pri::get<expr<temp::meta>>(res.varg)=v;;
            switch(v.t){
                case value<temp::inst>::ty::ptrmember:{res.t=param<temp::meta>::ty::PtrToMember;}
                case value<temp::inst>::ty::typeValue:{res.t=param<temp::meta>::ty::Typename;}
                case value<temp::inst>::ty::literal:{res.t=param<temp::meta>::ty::Type;}
                case value<temp::inst>::ty::varRef:{res.t=param<temp::meta>::ty::Type;}
            };
            return res;            
        };
        
        template <temp q> param<temp::meta>& get<temp::meta,param>(){
             
            param<temp::meta> res;
            for(;!OneOfLex<lex::ty::comma,lex::ty::gtangle>();nextTOK()){
                if(kw_Template::check(lexitback().u.name)){
                    until<lex::ty::ltangle,lex::ty::space,lex::ty::nl>();
                    param_list<temp::meta> pl=get<temp::meta,param_list>();
                    res.t=param<temp::meta>::ty::Template;
                    res.pl=pl;
                    untilDif<lex::ty::space,lex::ty::rtanfgle,lex::ty::nl>();
                    if(lexitback().t==lex::ty::Name){if(!OneOfKw<kw_Typename,kw_Class,kw_Struct>(lexitback().u.name) ){err::e(*this);}
                    else{nextTOK();try{until<lex::ty::Name,lex::ty::space,lex::ty::nl}catch(const UnexpectedToken& e){return res;}
                    res.name=lexitback().u.name;return res;    }
                };
                }
                else if(OneOfKw<kw_Typename>(lexitback().u.name)){
                    nextTOK();res.t=param<temp::meta>::ty::Typename;
                    if(lexitback().t==lex::ty::Name){res.name=lexitback().u.name;return res;}
                    else {throw UnexpectedToken(); }
                }
                else if( OneOfKw<kw_Class,kw_Struct>(lexitback().u.name)){res.t=param<temp::meta>::ty::Typename;
                    nextTOK();
                    if(lexitback().t==lex::ty::Name){
                        std::string n=lexitback().u.name;
                        nextTOK();
                        if(OneOfLex<lex::ty::comma,lex::ty::gtangle>()){res.t=param<temp::meta>::ty::Typename;res.name=lexitback().u.name;return res;}
                        else if(lexitback().t==lex::ty::Name){
                            result r=result::rErr;resty<q> rs;
                            rs=cast->findName(n,&r);
                            res.name=lexitback().u.name;
                            if(r=result::rType ){res.arg=pri::get<type<q>*>(rs);;return res;}
                            else if(r==result::rValue){res.val = pri::get<value<q>>(rs);res.t=param<q>::ty::Type;return res;}
                            else{err::warn(*this,TypeNotFound());}
                        }
                        else if(lexitback().t==lex::ty::dcolon){
                            result r=result::rErr;resty<q> rs;lex::ty acc=lex::ty::dcolone;
                            rs=cast->findName(n,&r);
                            for(nextTOK();ref_handle(acc,&r,&rs);){}
                            res.name=lexitback().u.name;
                            if(r=result::rType ){res.arg=pri::get<type<q>*>(rs);;return res;}
                            else if(r==result::rValue){res.val = pri::get<value<q>>(rs);res.t=param<q>::ty::Type;return res;}
                            else{err::warn(*this,TypeNotFound());}

                        };
                        else {throw UnexpectedToken(); }
                    }
                    else {throw UnexpectedToken();}
                }
                else {// TODO make sure about   ptrmember
                     if(lexitback().t==lex::ty::Name){res.t=param<temp::meta>::ty::Type;
                        std::string n=lexitback().u.name;
                        nextTOK();
                        if(OneOfLex<lex::ty::comma,lex::ty::gtangle>()){res.t=param<temp::meta>::ty::Typename;res.name=lexitback().u.name;return res;}
                        else if(lexitback().t==lex::ty::Name){
                            result r=result::rErr;resty<q> rs;
                            rs=cast->findName(n,&r);nextTOK();
                            if(lexitback().t==lex::ty::Name){res.name=lexitback().u.name;}
                            if(r=result::rType ){res.arg=pri::get<type<q>*>(rs);;return res;}
                            else if(r==result::rValue){res.val = pri::get<value<q>>(rs);res.t=param<q>::ty::Type;return res;}
                            else{err::warn(*this,TypeNotFound());}
                        }
                        else if(lexitback().t==lex::ty::dcolon){
                            result r=result::rErr;resty<q> rs;lex::ty acc=lex::ty::dcolon;
                            rs=cast->findName(n,&r);
                            for(nextTOK();ref_handle(acc,&r,&rs);){};nextTOK();
                            if(lexitback().t==lex::ty::Name){res.name=lexitback().u.name;}
                            if(r=result::rType ){res.arg=pri::get<type<q>*>(rs);;return res;}
                            else if(r==result::rValue){res.val = pri::get<value<q>>(rs);res.t=param<q>::ty::Type;return res;}
                            else{err::warn(*this,TypeNotFound());}

                        };
                        else {throw UnexpectedToken(); }
                    }
                };
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
        ///TODO This function ahould Return a type
        template <temp q> type<q>* get<q,type>(){type<q> r;stmt<temp::inst>::param_list plist;  
            result r=result::rErr;
            resty<temp::meta> res ;
            itPtr.push_back(lexptrback());
            try {res=cast->findName(lexitback().u.name,&r);}
            catch (const NameNotFound& nsexc){err::e(*this,nsexc);}
            if(r!=result::rErr){
                for(nextTOK();OneOfLex<lex::ty::name,lex::ty::dcolon>() and (r==result::rNs);nextTOK() ){
                    if(OneOfLex<lex::ty::dcolon>()){;continue;}
                    if(lexitback().t==lex::ty::Name){
                        try {    res= cast->findNameFromNs(&pri::get<stmt<temp::meta>::NS>(res),lexitback().u.name,&r);}
                        catch (const NameNotFound& nsexc){err::e(*this,nsexc);continue;}
                    }
                    else {err::e<err::t::unexpectedToken(*this);}
                }
                for(nextTOK();OneOfLex<lex::ty::name,lex::ty::dcolon>();nextTOK()){
                    if(OneOfLex<lex::ty::dcolon>()){continue;}
                    if(lexitback().t==lex::ty::Name){
                        try {res=cast->findNameFromTp(pri::get<type<temp::meta>>(res),lexitback().u.name,&r);}
                        catch (const NameNotFound& exc) {err::e::t::typeNotFound>(*this);}
                        nextTOK();
                        if(lexitback().t==lex::ty::ltangle){
                            stmt<temp::inst>::param_list plist=get<stmt<temp::inst>::param_list>();
                            res=cast->getTp(r,plist);
                            break;
                        };
                    }
                }
                for(nextTOK();OneOfLex<lex::ty::name,lex::ty::dcolon>();nextTOK()){
                    if(OneOfLex<lex::ty::dcolon>()){continue;}
                    if(lexitback().t==lex::ty::Name){
                        try {r=cast->findNameFromTp<temp::meta>(r,lexitback().u.name);}
                        catch(const NameNotFound& exc){err::e::t::typeNotFound>(*this);}
                        nextTOK();
                        if(lexitback().t==lex::ty::ltangle){
                            stmt<temp::inst>::param_list plist=get<stmt<temp::inst>::param_list>();
                            res=cast->getTp(res,plist);
                            break;
                        };
                    }
                }
            }
            else {err::e::t::typeNotFound>(*this);}
            erase();
            return Plist
        };
       
       
        template <lex::ty... l >
        stmt<temp::meta>::VarDecl getVarDecl(){
            stmt<temp::meta>::VarDecl vdecl;
            for(;OneOfLex<lex::ty::Name>();nextTOK();){
                if(kw_in::check(lexitback().u.name)){vdecl.in=true;};
                else if(kw_out::check(lexitback().u.name)){vdecl.out=true;};
                else if(kw_const::check(lexitback().u.name)){vdecl.Const=true;};
                else {break;}
            };
            if(kw_Auto::check(lexitback().u.name)){vdecl.Auto=true;}
            else {vdecl.tp = get<temp::inst,value>();}
            for(nextTOK();OneOfLex<l...>();nextTOK()){
                if(lexitback().t==lex::ty::Name){vdecl.name=lexitback().u.name;break;}
            };
            for(nextTOK();!OneOfLex<l...>();nextTOK()){
                if(lexitback().t==lex::ty::eq){vdecl.DefaultValue=getExpr<l...>();return vdecl;}
            };
            return vdecl;
        };
        stmt<temp::meta>::arg_list getArgList(){  stmt<temp::meta>::arg_list args;

                for(;!OneOfLex<lex::ty::rparen>();nextTOK()){
                    if constexpr (lang==language::spv){while(kwFound<KW_LYTQ,kw_Const>){nextTOK();}}
                    if constexpr (lang==language::cpp){while(kwFound<kw_Const>){nextTOK();}}
                    args.push_back(getExpr<lex::ty::comma,lex::ty::rparen>());
                };
                return args;
        };

        value<temp::meta> getInitList();{
            value<temp::meta> res(value::ty::initlist);
                value<temp::meta>::InitList il;
            for(nextTOK();!OneOfLex<lex::ty::rbrace>();nextTOK()){
                il.push_back(getExpr<lex::ty::comma,lex::ty::rbrace>());
            }
            pri::get<value<temp::meta>::InitList>(res.val)=il;
        };

        template <lex::ty... l> 
        expr<temp::meta> getExprName(){expr<temp::meta> res;
            if(OneOfKw<kw_New>()){res.push<op::ty::opNew,expr::node::opty::prefixUnary>();nextTOK();}
            else if(OneOfKw<kw_Delete>()){res.push<op::ty::opNew,expr::node::opty::prefixUnary>();nextTOK();}
            else if(OneOfKw<kw_Noexcept>()){res.push<op::ty::opNoExcept,expr::node::opt::prefixUnary>();
                nextTOK();expectErr<lex::ty::lparen>();ret.emplace( getExpr<lex::ty::rparen>());return ret;
            }
                accMember_list accl = get_accMember_List<l...>();
                if(OneOfLex<lex::ty::lparen>()){stmt<temp::meta>::arg_list argl=getArgList();
                    res.emplace(value<temp::meta>(value<temp::meta>::FuncCall(accl,argl),value<temp::meta>::ty::funcCall));
                }
                else if(OneOfLex<lex::ty::lbrack){ret.emplace(getInitList());}
            return res;
        };
        template <lex::ty l>
        void expectErr(){
            if(lexitback().t==l){return }
            else{err::e::(*this,UnexpectedToken());for(nextTOK();lexitback().t!=lex::ty::Name;nextTOK()){};}
        }
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
        value<temp::meta> getRef(){
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
    
        value<temp::meta> getLambda(){lambda<temp::meta> res;
            until<lex::ty::lbrack>();
            lambda<temp::meta>::captureList clist=getCaptures;
            nextTOK();stmt<temp::meta>::arg_list l;res.
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
            value<temp::meta> vl;vl.t=value<temp::meta>::lambdav;pri::get<lambda<temp::meta>>(vl.val)=res;
            return res;
        };
        template <lex::ty... l>
        expr<temp::meta> getExpr(){
            expr<temp::meta> res;
            if(lexitback().t==lex::ty::lbrack){res.tree.val = getLambda();return res;};
            auto prevName = [&res](){return res.e.back() };
            auto unary = <op::ty opT>[&](){if(prevName()){res.add<opT,expr::node::opty::postfixUnary>()}else{res.push<opT,expr::node::opty::prefixUnary>()}}
            stmt<temp::meta>::FuncDecl fdecl;type<temp::inst> restt;
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
                    case lex::ty::dot:{res.add<op::ty::opdot,expr<temp::meta>::node::opty::binary>();break;}
                    case lex::ty::arrow:{res.add<op::ty::arrow,expr<temp::meta>::node::opty::binary>();break;}
                    #ifdef CHAR_LITERALS
                    case lex::ty::sq :{res.pushLiteral(lexitback().u.chr);}
                    case lex::ty::dq :{res.pushLiteral(lexitback().u.name);}
                    #endif
                    case lex::ty::lparen:{nextTOK();
                        if(!res.e.back().TrailOp()){
                            res.addFuncCall(getArgList());
                        };
                        else{res.pushExpr(getExpr<lex::ty::rparen>());break;}}
                    case lex::ty::rparen:{}
                    case lex::ty::lbrace:{ res.emplace(getInitList());break;}
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
        template <> arg_list<temp::meta> get<stmt<temp::meta>::arg_list>();

        template <typename STMTty>
        void getStmt();
        template <typename STMTty,bool Strct,bool Func>
        void _getStmt();

        template <bool Strct , bool Func>
        void Stmt();
        
        template < bool Strct>
        void FStmt();        

        template <bool Strct,bool Func>void _getStmt<stmt<temp::meta>::block,Strct,Func>(){
            for(;lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>();};
            cast->popbl();
        };  
        void getBody(){
            lexq.push_back(lexitback());
            _getStmt<stmt<temp::meta>::block>();
            erase()
        };
        std::vector<qual> quals;
        void pushQual(qual q){quals.push_back(quals);};
        template <typename QualifT>
        void pubQuals(QualifT* res){
            try {res->push(quals);}
            catch (const WrongQualifier& e) {err::e(*this,e);}
            quals.clear();}
        template <bool Strct,bool Func>void _getStmt<stmt<meta>::NS,Strct,Func>(){
            for(++lexptrback();lexptrback().t!=lex::ty::lbrace;++lexptrback()){
                if(lexptrback().t==lex::ty::Name){cast->emplace_back<stmt<meta>::NS>(it->u.name;);
                    if(!cast->pushNS(pri::get<stmt<meta>::NS>(cast->curBlock()->back()))){
                        err::e<err::t::namespaceNotAllowedInCurrentScope>(*this);};continue;}
                if(!OneOfLex<lex::ty::space,lex::ty::nl>(lexptrback().t)){err::e<err::t::unexpectedToken>(*this);}
            };
            open<lex::ty::lbrace>();
        };
        void pushats(attrib_list* atptr){if(!_atlist.empty()){!atptr=_atlist;}}
        template <bool Strct,bool Func>void _getStmt<stmt<temp::meta>::While,Strct,Func>(){
        
            stmt<temp::meta>::While* res=cast->pushStmt<stmt<temp::meta>::While>();pushats(&res->atlist);
            nextTOK();expectErr<lex::ty::lparen>();nextTOK();res->condition=getExpr<lex::rparen>();
            nextTOK();expectErr<lex::ty::lbrace>();
            for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>();}
            nextTOK();
            cast->popbl();
        };
        template <bool Strct,bool Func>void _getStmt<stmt<temp::meta>::For,Strct,Func>(){
            stmt<temp::meta>::For* res=cast->pushStmt<stmt<temp::meta>::For>();pushats(&res->atlist);
            nextTOK();expectErr<lex::ty::lparen>();
            itPtr.push(lexptrback());
            for(nextTOK();!OneOfLex<lex::ty::semicolon,lex::ty::colon>();nextTOK()){}
            nextTOK();res->init=getExpr<lex::ty::semicolon,lex::ty::colon>();
            if(OneOfLex<lex::ty::colon>()){itPtr.pop();
                stmt<temp::meta>::ForRange* res=cast->pushStmt<stmt<temp::meta>::ForRange>(stmt<temp::meta>::ForRange());
                nextTOK();
                res->vardecl=getVarDecl<lex::ty::colon>();nextTOK();
                res->ref=getExpr<lex::ty::rparen>();nextTOK();
            }
            else{stmt<temp::meta>::For* res=cast->pushStmt<stmt<temp::meta>::For>(stmt<temp::meta>::For());
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
        template <bool Strct,bool Func>void _getStmt<stmt<temp::meta>::Do,Strct,Func>(){
            stmt<temp::meta>::Do* res=cast->pushStmt<stmt<temp::meta>::Do>(stmt<temp::meta>::Do());pushats(&res->atlist);
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
        
        template <bool Strct,bool Func>void _getStmt<stmt<temp::meta>::Switch,Strct,Func>(){
            stmt<temp::meta>::Switch* res=casts->pushStmt<stmt<temp::meta>::Switch>(stmt<temp::meta>::Switch()) ;pushats(&res->atlist);
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

        value<temp::meta> ret_ty_val;// Used By various functions to Parse Function stmts;
        template <bool Strct,bool Func>void _getStmt<stmt<temp::meta>::OperatorDecl,Strct,Func>(){
            nextTOK();

            stmt<temp::meta>::OperatorDecl* res=cast->pushStmt<stmt<temp::meta>::OperatorDecl>(stmt<temp::meta>::OperatorDecl());pushats(&res->atlist);
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
        template <bool Strct,bool Func>void _getStmt<stmt<temp::meta>::Try,Strct,Func>(){
            stmt<temp::meta>::Try res; 
            for(nextTOK();OneOfLex<lex::ty::space,lex::ty::nl>();nextTOK()){}
            if(!OneOfLex<lex::ty::lbrace>()){err::e<err::t::unexpectedToken>(*this);}
            cast->pushbl(res.body);
            for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>();};
            cast->popbl();
            cast->pushStmt(res);
        };
        
        template <bool Strct,bool Func>void _getStmt<stmt<temp::meta>::Catch,Strct,Func>(){
            cast->pushStmt<stmt<temp::meta>::Catch>();
            stmt<temp::meta>::Catch& res= pri::get<stmt<temp::meta>::Catch>(*getStmt());
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
        template <bool Strct,bool Func>void _getStmt<stmt<temp::meta>::Throw,Strct,Func>(){
            nextTOK();
            expr<temp::temp::meta> vl=getExpr<lex::ty::semicolon>();
            cast->pushStmt<stmt<temp::meta>::Throw>(vl);
        };
        
        template <bool Strct,bool Func>void _getStmt<stmt<temp::meta>::If,Strct,Func>(){bool ConstExpr=false;
            stmt<temp::meta>::If* r=cast->pushStmt<stmt<temp::meta>::If>(stmt<temp::meta>::If(getExpr<lex::ty::rparen>())); pushats(&res->atlist);
            for(nextTOK();lexitback().t!=lex::ty::lparen;nextTOK()){
                if(OneOfKw<kw_ConstExpr>(lexitback().u.name)){ConstExpr=true;}
                else {err::e<err::t::unexpectedToken>(*this);}
            };
            if(ConstExpr){r.ConstExpr=true;}
            for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>();};
                        cast->popbl();

            
        };
        template <typename T> void Else(T* r){
            stmt<temp::meta>* last=cast->getStmt();
            switch(last->t){
                case stmt<q>::stmtty::eIf:{r->elIf=false;pri::get<stmt<temp::meta>::If*>(r->Ifs)=&pri::get<stmt<temp::meta>::If>(last->var);}
                case stmt<q>::stmtty::eElseIf:{r->elIf=true;pri::get<stmt<temp::meta>::ElseIf*>(r->Ifs)=&pri::get<stmt<temp::meta>::ElseIf>(last->var);}
            }    
        
        }
        template <bool Strct,bool Func>void _getStmt<stmt<temp::meta>::ElseIf,Strct,Func>(){bool ConstExpr=false;
            for(nextTOK();lexitback().t!=lex::ty::lparen;nextTOK()){
                if(checkName()){
                    if(OneOfKw<kw_ConstExpr>(lexitback().u.name)){ConstExpr=true;}
                    else {err::e<err::t::unexpectedToken>(*this);}
                }
            };
            stmt<temp::meta>::ElseIf* r=cast->pushStmt<stmt<temp::meta>::ElseIf>(stmt<temp::meta>::ElseIf(getExpr<lex::ty::rparen>()));
            if(ConstExpr){r->ConstExpr=true;}
            Else<stmt<temp::meta>::ElseIf>(r);
            for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>()};
                        cast->popbl();

        };
        template <bool Strct,bool Func>void _getStmt<stmt<temp::meta>::Else,Strct,Func>(){
            nextTOK();
            if(kw_If::check(lexiback().u.name)){_getStmt<stmt<temp::meta>::ElseIf>();return;}
            stmt<temp::meta>::Else* r;
            r=cast->pushStmt<stmt<temp::meta>::Else>(stmt<temp::meta>::Else());pushats(&res->atlist);
            Else<stmt<temp::meta>::Else>(r);
            if(lexitback().t==lex::ty::lbrace){for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>()};;}
            else {if(kw_Return::check(lexitback().u.name)){_getStmt<temp::meta>::Return>();};
            else{_getStmt<temp::meta>::Expr>();}
            }
            cast->popbl();
        };
        stmt<temp::meta>::FuncDef* curfunc;
        template <bool Strct,bool Func>void _getStmt<stmt<temp::meta>::Return,Strct,Func>(){cast->pushStmt<stmt<temp::meta>::Return>(getExpr<lex::ty::semicolon,lex::ty::rbrace>());};


        
  
        
        // template <bool Strct,bool Func>void _getStmt<stmt<temp::meta>::TypeDef,Strct,Func>(){
        //     stmt<temp::meta>::TypeDef* res=cast->pushStmt<stmt<temp::meta>::TypeDef>(stmt<temp::meta>::TypeDef());
        //     nextTOK();
        //     res->expr=get<temp::inst,value>();nextTOK();expectErr<lex::ty::Name>();
        //     res->name=lexitback().u.name;
        // };
        
        template <stmt<temp::meta>::Layout::stand st, Str s >struct lyt_std{
            stmt<temp::meta>::Layout::stand t =st;
            static constexpr auto value = s;
            std::string name(){return std::string(value.data.data());}

        };
        using std430 = lyt_std<stmt<temp::meta>::Layout::stand::std430,"std430">;
        using std140 = lyt_std<stmt<temp::meta>::Layout::stand::std430,"std140">;

        template <typename STD,typename... STDs>
        bool getStd(stmt<temp::meta>::Layout& s){
            if(getStd(s)){return true;}
            return getStd<STDs...>(s);
        };
        template <typename STD>
        bool getStd(stmt<temp::meta>::Layout& s){
            if(lexptrback()->u.name==STD::name()){s.st=STD::t;return true;};
            return false;
        };
        
        // using getLytAt = BoolFunc<stmt<temp::meta>::Layout,getLytAt,lyt_at,lyt_ats...>;
        template <typename lyt_at,typename... lyt_ats>
        bool getLytAt(stmt<temp::meta>::Layout& s){
            if(getLytAt<lyt_at>(s)){return true;};
            return getLytAt<,lyt_ats...>(s);
        };
        template <typename lyt_at>
        bool getLytAt(stmt<temp::meta>::Layout& s){
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
        void getLytVar(stmt<temp::meta>::Layout& lyt ){
            for(;OneOfKw<kw_in,kw_out,kw_inout,>(lexitback().t);nextTOK()){
                if(checkName()){
                    if(kw_Uniform::check(lexitback().u.name)){lyt.uniform=true;}
                    if(kw_Flat::check(lexitback().u.name)){lyt.flat=true;}
                }
            };
            if(kw_in::check(lexitback().u.name)){lyt.in=true;}
            if(kw_inout::check(lexitback().u.name)){lyt.in=true;lyt.out=true;}
            if(kw_out::check(lexitback().u.name)){lyt.out=true;}
            lyt.vart=stmt<temp::meta>::Layout::tyT::var;
            for(nextTOK();!OneOfLex<lex::ty::semicolon,lex::ty::lbrace>();nextTOK()){
                if(lex::ty::Name==lexitback().t){
                    if(kw_Buffer::check(lexitback().u.name)){lyt.vart=stmt<temp::meta>::Layout::tyT::buffer;continue;};
                    if(kw_Uniform::check(lexitback().u.name)){lyt.vart=stmt<temp::meta>::Layout::tyT::uniform;continue;};
                    if(lyt.vart==stmt<temp::meta>::Layout::tyT::var){

                        cast->ptr.vd=&pri::get<stmt<temp::meta>::VarDecl>(lyt.data);getStmt<stmt<temp::meta>::VarDecl>() ;
                        lyt.name=pri::get<stmt<temp::meta>::VarDecl>(lyt.data).name;
                        return;
                    }
                    else {lyt.name=name;}
                
                }
                else if(!OneOfLex<lex::ty::space,lex::ty::nl>()){err::e<err::t::unexpectedToken>(*this);}
            }
            if(lexitback().t==lex::ty::lbrace){cast->ptr.dt=pri::get<stmt<temp::meta>::DefType>(lyt.data);getStmt<stmt<temp::meta>::TypeDef>();}
        };
        template <bool Strct,bool Func>void _getStmt<stmt<temp::meta>::Layout,Strct,Func>(){
            
            cast->pushStmt<stmt<temp::meta>::Layout>();stmt<temp::meta>::Layout& lyt=cast->layouts.back();
            nextTOK();
            if(lexitback().t!=lex::ty::lparen){err::e<unexpectedToken,stmt<temp::meta>::Layout>(*this);}
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
        template <bool Strct,bool Func>void _getStmt<stmt<temp::meta>::Expr,Strct,Func>(){cast->pushStmt<stmt<temp::meta>::Expr>(GetExpr<lex::ty::semicolon>());};
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
            if(kw_signed::check(lexitback().u.name)){res.s=integralT::sing::Unsinged;nextTOK();}
            else if(kw_unsigned::check(lexitback().u.name)){res.s=integralT::sing::Singed;nextTOK();expectErr<lex::ty::Name>();}
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
                        case result::rValue :{res.mt=pri::get<value<temp::meta>>(rs);}
                        case result::rType : {res.mt=pri::get<type<temp::meta>*>(rs);}
                    }
                }
            return res;
        };

        void Enum(bool bTemp,stmt<temp::meta>::param_list& plist){stmt<temp::meta>::Enum res;
            nextTOK();
            if(lexitback().t==lex::ty::Name){if(kw_Struct::check(lexitback().u.name) or(kw_class::check(lexitback().u.name))){enclass=true;nextTOK();}};
            if(lexitback().t==lex::ty::rdi){res.ats=getAttribs();nextTOK();}
            res.name=lexitback().u.name;nextTOK();
            if(lexitback().t==lex::ty::colon){res.SpeqSeq=true;res.intt=getEnumIntegral();};
            nextTOK();
            stmt<temp::meta>* ptr=cast->pushStmt(res);
            if(lexitback().t==lex::ty::lbrace){
                for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){
                    if(lexitback().t!=lex::ty::Name){
                        ptr->emplace_back(lexitback().u.name);
                        nextTOK();
                        if(lexitback().t==lex::ty::eq){ptr->back().cexprval=GetExpr<lex::ty::comma>();}
                        if(lexitback().t!=lex::ty::comma){err::e(this,UnexpectedToken<stmt<temp::meta>::Enum>());}
                        
                    }
                    else {err::e(this,UnexpectedToken<stmt<temp::meta>::Enum>());};
                };
            }
        };

        template <bool Strct,bool Func>
        void func(accMember_list& rett,accMember& name,bool btemp,param_list<temp::meta>& plist,size_t refn=0,size_t ptrn=0){// CalledFrom Lparen
            if(btemp){cast->prms.push_back(&plist);
            stmt<temp::meta>::argList args = getArgList();nextTOK();
            if constexpr (Strct){while(kwFound<kw_Noexcept,kw_Final>()){nextTOK();}}
            else {while(kwFound<kw_Noexcept>()){nextTOK();}}
            bool Declared =(cast->find(name) and (name->r==reuslt::rFunc ))  ;
            bool isTemplate = Declared?pri::get<stmt<temp::meta>::FuncDecl*>(name->back().inst)->Template:false;
            if(OneOfLex<lex::ty::semicolon>()){
                
                if(Declared and !isTemplate) {err::e(*this,AlreadyDeclared<stmt<q>::FuncDecl>(
                    pri::get<stmt<temp::meta>::FuncDecl*>(name->back().inst),rett,name,btemp,plist));}
                else {
                    if(name.size()==1 and (name.globalAcc ==false)){ //TODO Check not already Declared
                        stmt<temp::meta>::FuncDecl* res = pushStmt<stmt<temp::meta>::FuncDecl>(stmt<temp::meta>::FuncDecl(rett,name,plist,refn,ptrn));
                        res->name=name.front().name;pubQuals(res);if(btemp){res->prms=plist;cast->curtemp.back()=&res->prms;}
                        res->ret = rett;res.args=args;pushats(res->atlist);}
                    else {
                        stmt<temp::meta>::FuncDef* res;
                        try{res = cast->pushSpec<stmt<temp::meta>::FuncDef>(pri::get<stmt<temp::meta>::FuncDecl*>(name.back().inst,
                            stmt<temp::meta>::FuncDef(rett,plist,args)),plist;}
                        catch(const SpecializationError& e){err::e(*this,e)}
                    }
                }
            }
            else if (OneOfLex<lex::ty::lbrace>()){
                if (Declared){
                    if(name.size()==1 and (name.globalAcc==false)){
                        stmt<temp::meta>::FuncDecl* res = pushStmt<stmt<temp::meta>::FuncDecl>(stmt<temp::meta>::FuncDecl());
                        res->name=name.front().name;pubQuals(res);if(btemp){res->prms=plist;}
                        res->ret = rett;res.args=args;pushats(res->atlist);
                    }
                    else {

                    }
                }
                else 
                else throw DeclarationOutsideOfScope<stmt<temp::meta>::FuncDecl>();
            } ;
        };
        template <bool Strct,bool Func>
        void var(accMember_list& rett,accMember_list& name,bool btemp,param_list<temp::meta>&& plist,size_t refn=0,size_t ptrn=0){// CalledFrom eq or ;
            stmt<temp::meta>::VarDecl* res ;
            if(name.size()==1 and !name.globalAcc!=true){
                res=cast->pushStmt<stmt<temp::meta>::VarDecl>(stmt<temp::meta>::VarDecl(rett,name.front().name,refn,ptrn));
                pubQuals(res);
            }
            else {
                res=cast->pushStmt<stmt<temp::meta>::VarDecl,Strct>(stmt<temp::meta>::VarDecl());
                stmt<temp::meta>::VarDecl& to = *(--name.tail());
                res=pushStmtTo<stmt<temp::meta>::VarDecl>(stmt<temp::meta>::VarDecl,Strct>(&to,VarDecl(rett,name,refn,ptrn)));
            }
            if(OneOfLex<lex::ty::semicolon>()){return;}
            if(OneOfLex<lex::ty::eq>()){res->Default = true;res->DefaultValue=getExpr<lex::ty::semicolon>();return;}
        };

        template <bool Strct>
        struct AccDefault {static constexpr bool acc=Strct?accSpec::Public:accSpec::Private;}
        template <bool Strct>
        type<temp::meta>::inher_list getInherList(){
            type<temp::meta>::inher_list res;
            for(nextTOK();!OneOfLex<lex::ty::semicolon,lex::ty::lbrace>()){
                expectErr<lex::ty::Name>();
                if(lexitback().t==lex::ty::Name){
                    if(kw_Public::check(lexitback().u.name)){res.emplace_back(accSpec::Public);nextTOK();}
                    else if(kw_Protected::check(lexitback().u.name)){res.emplace_back(accSpec::Protected);nextTOK();}
                    else if(kw_Private::check(lexitback().u.name)){res.emplace_back(accSpec::Private);nextTOK();}
                    else {res.emplace_back(get_accMember_list<lex::ty::comma,lex::ty::semicolon,lex::ty::lbrace>(),AccDefault<Strct>::acc);nextTOK();}
                    if(lexitback().t==lex::ty::Name){ res.back().t=get_accMember_list<lex::ty::comma,lex::ty::semicolon,lex::ty::lbrace>();nextTok();}
                }
            };
            return res;
        };
        template <bool tdef,bool accPublic,bool Strct>
        void _Strct(bool bTemp=false,stmt<temp::meta>::param_list& plist={}){
            curPtr.push_back(lexptr.back());
            nextTOK();
            if constexpr (tdef) {
                stmt<temp::meta>::TypeDef* res = cast->pushStmt<stmt<temp::meta>::TypeDef>(stmt<temp::meta>::TypeDef());
                if(OneOfLex<lex::ty::lbrace>()){
                    nextTOK();
                    if(!OneOfLex<lex::ty::lbrace>()){
                        --lexptrback();res->Tt=new type<temp::meta>();
                        cast->strcts.push_back(res->Tt);
                        for(nextTOK();!OneOfLex<lex::ty::semicolon>();nextTOK()){Stmt<true,false>();};
                        cast->strct.pop_back();
                    }
                    else if(OneOfLex<lex::ty::Name>()){
                        try { get_accMember_list<lex::ty::semicolon>()};
                        catch (const accMemberStop& e){res->tp =e.what();}
                        res->name=lexitback().u.name;
                    }
                    else {err::e(*this,UnexpectedToken());}
                };
            }
            else {
                expectErr<lex::ty::Name>();
                nextTOK();
                accMember_list<temp::meta> aclist ;
                
                try {aclist =  get_accMember_list<lex::ty::colon,lex::ty::semicolon>();}
                catch (const accMemberStop& e){aclist=e.what();
                    
                    if(OneOfLex<lex::ty::Name>()){
                        accMember_list<temp::meta> nm ;bool caught=false;
                        try {nm =  get_accMember_list<lex::ty::eq,lex::ty::semicolon>();}
                        catch (const accMemberStop& e){// Check ptrMember
                            caught=true;
                            nm= e.what();
                            if(nm.back().acc==lex::ty::ptrmember){
                                res->memberPtr=true;res->vl=value<temp::meta>(nm,value<temp::meta>::ty::ptrmember);
                                res->name=lexitback().u.name;
                            }
                            else if(nm.size()==1){
                                stmt<temp::meta>::VarDecl* res=cast->pushStmt<stmt<temp::meta>::VarDecl>(stmt<temp::meta>::VarDecl());
                                res->name=nm.front().name;res->tp=aclist;
                                nextTOK();
                                if(OneOfLex<lex::ty::eq>){nextTOK();res->DefaultValue=getExpr<lex::ty::semicolon>();}
                                else if(OneOfLex<lex::ty::eq>){res->vl=get<temp::meta,value,lex::ty::semicolon>();}
                            }
                            erase();return;
                        }
                        if(!caught){
                            if(OneOfLex<lex::ty::eq>()){
                                stmt<temp::meta>::VarDecl* res =cast->pushStmt<stmt<temp::meta>::DeclType>(stmt<temp::meta>::VarDecl()); 
                                bool find;
                                res->findTp(acclist,&find);
                            }
                            else 
                        }
                        
                    }
                    
                    erase();return;
                }
                if(OneOfLex<lex::ty::lparen){
                    value<temp::meta> cCall;cCall.t=value<temp::meta>::ty::funcCall;
                    cast->findTp()
                }
                else if(OneOfLex<lex::ty::semicolon>()){
                    stmt<temp::meta>::DeclType* res =cast->pushStmt<stmt<temp::meta>::DeclType>(stmt<temp::meta>::DeclType()); 
                    if(aclist.size()==1){
                        res->name=aclist().front().name;}
                    else {
                        if(res->)

                    }
                }
                else if(OneOfLex<lex::ty::colon>()){
                       
                }
                else if(OneOfLex<lex::ty::lbrace>()){

                }
                return ;
            }
            }
            erase();
        };
        template <bool tdef,bool Strct>
        void Union(bool bTemp,stmt<temp::meta>::param_list& plist){
            try {until<lex::ty::Name>();}
            catch (const UnexpectedToken& e){
                if(e.what().t==lex::ty::lbrace){
                    getStmt<stmt<temp::meta>::Union>();
                    try{until<lex::ty::Name,lex::space,lex::ty::nl>();}
                    catch (const UnexpectedToken& e){return;}
                    stmt<temp::meta>::VarDecl res;cast->pushStmt(res);

                }
                else{err::e(*this,e);}
            }
        };
        template <bool Strct>
        void Using(bool bTemp,stmt<temp::meta>::param_list& plist){
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
            
            stmt<temp::meta>::Using* res=cast->pushStmt<stmt<temp::meta>::Using>(stmt<temp::meta>::Using());
            if(OneOfKw<kw_Typename>()){res->Typename=true;nextTOK();};pushats(res->atlist);
            expectErr<lex::ty::Name>();res.name= lexitback().u.name;nextTOK();
            expectErr<lex::ty::eq>();nextTOK();
            res->expr = get_accMember_list<lex::ty::semicolon>();
        };
        
       
        
        
        template <bool Strct,bool Func>
        void NameStmt(bool bTemp,param_list<temp::meta> plist){
            auto r= result::rErr;
            accMember_list acclist ;bool glbl=false;
            if(OneOfLex<lex::ty::dcolon>()){glbl=true;nextTOK();}
            size_t refNum=0;size_t ptrNum=0;
                try {acclist = get_accMember_List();}
                catch ( const accMemberStop& e){
                    if constexpr (lang == language::cpp){
                        while(OneOfLex<lex::ty::mul>()){ptrNum++;}
                        while(OneOfLex<lex::ty::ref>()){refNum++;}
                    }
                }
                acclist.globalAcc=glbl;
                if(OneOfLex<lex::ty::Name>()){
                    res->name=lexitback().u.name;
                    res->ptrNum=ptrNum;res->refNum=refNum;
                    acclist.gloaglAccess;res->
                    res->tp=acclist;
                    nextTOK();
                    accMember_list nm ;
                    try {nm = get_accMember_List();}
                    catch ( const accMemberStop& e){}
                    if(OneOfLex<lex::ty::lparen>()){func<Strct,Func>(acclist,nm,bTemp,plist,refNum,ptrNum);}
                    else if(OneOfLex<lex::ty::semicolon,lex::ty::eq>() and (nm.size==1)) {varDecl<Strct,Func>(acclist,nm,btemp,plist,refNum,ptrNum);};
                    else err::e(*this,UnexpectedToken();)
                }
        };
        

        void getTemplate(){//NOTE probably no use;
            itPtr.push_back(lexptrback());
            for(nextTOK();lexitback().t;nextTOK()){
                // if(OneOfLex<lex::ty::space,lex::ty::nl>()){continue;}
                if(OneOfLex<lex::ty::lbrace>()){
                     plist=get<stmt<temp::meta>::param_list>();Template=true;
                    break;
                }
            };
            for(nextTOK();lexitback().t!=lex::ty::semicolon;nextTOK()){
                if(checkName()){
                    if(OneOfKw<kw_Class>(lexitback().u.name)){accessPush<accSpec::Private>();
                        access= getStmt<stmt<temp::meta>::DeclType>();break;
                    }                    
                    if(OneOfKw<kw_Struct>(lexitback().u.name)){accessPush<accSpec::Public>();
                        getStmt<stmt<temp::meta>::DeclType>();break;
                    }
                    getStmt<stmt<temp::meta>::FuncDecl>()

                }
            };
            Template=false;plist=stmt<temp::meta>::param_list();
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

        struct mtype {
            pri::variant<stmt<temp::meta>::FuncDecl*,stmt<temp::meta>::OperatorDecl*,stmt<temp::meta>::DeclType*,type<temp::meta>*> var;
            enum class ty{Func,Oper,rType} ;ty t;
        };


       
        mtype getMtype(){
            mtype reslt;
            if(kw_Class::check(lexitback().u.name or kw_Struct::check(lexitback().u.name))){
                nextTOK();
                result r;resty<temp::meta> res;lex::ty acc=lex::ty::none;
                ref_handle(acc,&r,&res);
                reslt.t=mtype::ty::rType;pri::get<type<temp::meta>*>(reslt.var)=res.
            }
            else if(kw_Operator::check(lexitback().u.name)){
                op::ty r=getOperator();reslt.opt=r;
                if(r==op::ty::opType){
                    reslt.op = get_accMember_list<lex::ty::lparen,lex::ty::semicolon>();
                }
                return reslt;
            };
            else {

            }
            
        };
        void GetFriend(){nextTOK();
            
            nextTOK();bool bPack=false;bool blist=true;
            lexq.push_back(lexptrback());
            for(;!OneOfLex<lex::ty::semicolon>();nextTOK()){
                mtype res=getMtype();
                if(lexitback().t==lex::ty::pack){bPack=true;}
                if(lexitback().t==lex::ty::comma){blist=true;}
                if(res.t==mtype::ty::Func){curTp->frndsFunctions.push_back(pri::get<stmt<temp::meta>::FuncDecl*>(res.var));}
                if(res.t==mtype::ty::Oper){curTp->frndsOperators.push_back(pri::get<stmt<temp::meta>::OperatorDecl*>(res.var));}
                if(res.t==mtype::ty::rType){curTp->frndsTypes.push_back(pri::get<stmt<temp::meta>::DeclType*>(res.var));}

            };
            erase();
            
        };
        #define PUBLIC_ACCESS true
        #define PRIVATE_ACCESS false
        template <bool Strct,bool Func>
        bool StmtPush(){
           
            if(lexitback().t==lex::ty::lbrace){
                if constexpr (Func){
                    cast->pushStmt<stmt<temp::meta>::block>(stmt<temp::meta>::block);
                    for(nextTOK();OneOfLex<lex::ty::rbrace>();nextTOK()){StmtPush<Strct,Func>();}
                return true;
                }
                else throw UnexpectedToken(lexitback().t);
            };
            
            if(lexitback().t==lex::ty::ldi){_atlist = getAttribs(); nextTOK(); }
            if(lexitback().t==lex::ty::semicolon){nextTOK();return;}
            if(OneOfLex<lex::ty::dcolon>()){}
            if(OneOfLex<lex::ty::Name>()){
                if(kwFound<KW_LISTM>()){return true;}
                if constexpr (lang==language::cpp){while(kwFound<KW_QUAL,kw_Extern>()){nextTOK();}}
                else if constexpr (lang ==  langauge::stmsl){while(kwFound<KW_QUAL,KW_LYTQ,kw_Layout>()){nextTOK();}}
                stmt<temp::meta>::param_list plist ;bool bTemp=false;
                if(OneOfKw<kw_Template>()){bTemp=true;
                    if constexpr (Func){err::e(*this,UnexpectedToken("Template Not allowed in function body"));}
                    nextTOK();expectErr<lex::ty::ltangle>();
                    plist = get<temp::meta,param_list>();until<lex::ty::Name,lex::ty::gtangle,lex::ty::space,lex::ty::nl>();
                }

                if constexpr (Strct){
                    if(OneOfKw<kw_Operator>()){GetOperator<Strct>();return;}
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
                            stmt<temp::meta>::TypeDef* res = cast->pushStmt<stmt<temp::meta>::TypeDef>(stmt<temp::meta>::TypeDef());
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
            StmtPush<Strct,false>();
        };
        template <bool Strct>
        void FStmt(){if(!kwFound<KW_LISTM>()){
            StmtPush<Strct,true>();
        };
        

        ast* fromFile(std::filesystem::path pth){ curFilePath.push_back(pth);
            cast=new ast();
        cwd.push(pth.parent_path());
        f.open(pth);
        pos.push_back(posit());bool fend=false;
        try {while(Line()){Stmt();}}
        catch (const FileEnd& e){fend=e.b;}
        if(pth.extension()==std::filesystem::path("hstmsl")){wrcph(pth,cur); }
        cwd.pop();
        parserStack.pop();
        return cast;
        };
        ast& fromFile(std::string pth){filePos =0; std::filesystem::path s(pth);return *fromFile(s); }
        parser() = default;
        

}
}
#endif