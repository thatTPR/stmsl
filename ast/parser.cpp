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
        void erase(){lexq.clear();};

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


        pri::deque<lex>::iter& lexptrback(){return lexq.tail();}
        lex& lexitback(){return lexq.back();}

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
        
        template <lex::ty T,lex::ty... Ts>
        bool _OneOfLex(lex l){return pri::OneOf<lex::ty,T,Ts...>(l);}
        
        template <lex::ty T,lex::ty... Ts>
        bool OneOfLex(){return _OneOfLex<T,Ts...>(lexitback().t);}

        template <typename Kw,typename... Kws>
        bool OneOfKw(){
            if(KW::check(lexitback().u.name)){return true;}
            else if constexpr(sizeof...(Kws)){return OneOfKw<Kws...>(str);}
            else return false;
        };
      
        using lexres=pri::deque<pri::deque<lex>::iter>;
       
        template <lex::ty to,lex::ty... tos>
        void untilLex(){for(;!OneOfLex<to,tos...>();nextTOK()){}}
        template <lex::ty to,lex::ty... tok>
        void until(){
            for(;OneOfLex<lex::ty::to>() ;nextTOK()){
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
                        lexEmplace(posb(),lex::ty::mul);lexitback().u.unum=1;
                        for(;line[posb()+1]==lex::ty::mul;++posb()){lexitback().u.unum++;}
                        return;}
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
   
   
        template <lex::ty fromTok,lex::ty toTok,template <temp q> typename T>
        T<temp::meta> _getFromUntil(){return getFromUntil<fromTok,toTok,T<temp::meta>();};



        template <lex::ty t,template <temp q> typename T>
        T<temp::meta> _getUntil(){return getUntil<t,T<temp::meta>();        };
        
        template <lex::ty tokFrom,lex::ty tokUntil , typename T>
        T getFromUntil(){
            
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
            if constexpr (KW::KW_SET){return KW::found(*this);}
            if constexpr (KW::check(lexitback().u.name)){KW::proc(*this);return true;}
            if constexpr (sizeof...(KWs)>0){return kwFound<KWs>();}
            return false;
        } ;
      
        template <bool Eval,typename KW,typename... KWs>
        void untilKW(){for(;!OneOfKw<KW,KWs...>();nextTOK()){};};
        
        // Handling Structs
        template <temp q,template<temp> typename T,lex::ty... l> T<q>& get();
        
         bool correct(accMember_seq& acc){
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
        bool member(accMember_seq& acc){
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
        std::vector<qual> quals;
        void pushQual(qual q){quals.push_back(quals);};
        template <typename QualifT>
        void pubQuals(QualifT* res){
            try {res->push(quals);}
            catch (const WrongQualifier& e) {err::e(*this,e);}
            quals.clear();}
        
         op::ty getOperator(){
            op::ty op;
            nextTOK();
            if(lexitback().t!=lex::ty::Name){nextTOK();return lexitback().t;}
            else { if(kw_New::check(lexitback().u.name)) {nextTOK();if(lexitback().t==lex::ty::lbrack){nextTOK();nextTOK();return op::ty::opNewArr};return op::ty::opNew;}
            else if(kw_Delete::check(lexitback().u.name)){nextTOK();if(lexitback().t==lex::ty::lbrack){nextTOK();nextTOK();return op::ty::opDeleteArr};return op::ty::opDelete;}
            else {return op::ty::opType;} 
            }
        }
        template <lex::ty l>
        stmt::init_args getArgInit(){stmt::init_args args;
            for(;!OneOfLex<l>();){nextTOK();
                args.push_back(getExpr<lex::ty::comma,lex::ty::rparen>());};return args;};
        
        template <bool pren> 
        void procAccMem(accMember_seq& res){
              if(!OneOfLex<lex::ty::Name>()){if(OneOfLex<LEX_ACC>(){err::e(*this,UnexpectedToken(lexitback().t))});return;};
                 if(OneOfKw<kw_Operator>()){
                    res.emplace_back(accMember::result::rOperator);
                    res.back().oprt=getOperator();}
                else {res.emplace_back(lexitback().u.name);}
                // try {
                //     if constexpr (Start){
                //         if(res.globalAcc){cast->findFrom<stmt::NS>(lexitback().u.name,&res.back().r,&res.back().inst,&cast->global);}
                //          else {cast->find(res.back().name,&res.back().r,&res.back().inst);};
                //         }
                //         else {
                //             auto last =res.tail();--last;
                //               switch(last->r){
                //                 case result::rType : {try{findFrom<type<q>>(res.back().name,&res.back().r,&res.back().inst,pri::get<type<q>*>(last->inst));}
                //                     catch(const MemberNotFound& e){}};
                //                 case result::rNs : {try{findFrom<stmt::NS>(res.back().name,&res.back().r,&res.back().inst,pri::get<stmt::StmtNS*>(last->inst));}
                //                                 catch(const MemberNotFound& e){};}
                //                 case result::rEnum : {
                //                     try {findFrom<stmt::NS>(res.back().name,&res.back().r,&res.back().inst,pri::get<stmt::Enum*>(last->inst));}
                //                     catch(const MemberNotFound& exc){};} }
                //                             cast->find(lexitback().u.name,&resl.r,&resl.inst);}
                //                 }catch (const NameNotFound& e){}

                if(OneOfLex<lex::ty::ltangle>()){res.back().plist=get<temp::inst,param_list>();nextTOK();res.back().Template=true;}
                    
                if(OneOfLex<lex::ty::lparen>()){
                    if constexpr(pren){res.back().args=getArgInit<lex::ty::lparen>();nextTOK();}else {return;};}
                    if(OneOfLex<LEX_ACC>()){res.back().acc=lexitback().t;};                    
        };
        static constexpr bool pren_false = false; 
        static constexpr bool pren_true = true; 
        template <bool pren=true> accMember_seq get_accMember_seq(){
            accMember_seq res;res.pos=lexitback().pos;res.acc=lex::ty::none;
            if(lexitback().t==lex::ty::dcolon){res.globalAcc=true;nextTOK();}
              procAccMem<pren>(res);if constexpr (!pren) {if(OneOfLex<lex::ty::lparen>(){return;})
            do{ nextTOK();procAccMem<pren>(res);}while(OneOfLex<LEX_ACC>());
        };        

        template <temp q> value handleDeclType(){
            nextTOK();expectErr<lex::ty::lparen>();nextTOK();value res;res.t=value::ty::DeclT;bool rval=false;
            if(OneOfLex<lex::ty::lparen>()){nextTOK();rval=true;}
            pri::get<expr>(res.val)=getExpr<lex::ty::rparen>();
            expectErr<lex::ty::rparen>();nextTOK();
            if(rval){expectErr<lex::ty::rparen>();nextTOK();res.refNum++;}
            nextTOK();
            if(OneOfLex<lex::ty::mul>()){nextTOK();res.ptrNum+=lexitback().u.unum;}
            if(OneOfLex<lex::ty::mul>()){nextTOK();res.refNum++;}
            return res;
        };
        // These two functions are invalid
        template <value::ty vlt>
        value get_value();
        value& get_value<value::ty::typeValue>(){
            value res;
            res.t=value::ty::typeValue;
            if(kw_Decltype::check(lexitback().u.name)){ res=handleDeclType<temp::inst>();}
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
        template<>value get_value<value::ty::ptrmember>(){expectErr<lex::ty::band>();
            value res;res.t=value::ty::ptrmember;res.tt=value::truTy::eaccList;
            pri::get<accMember_seq>(res.val)=get_accMember_seq();
        };

        template <lex::ty... l> 
        expr getExprName();
         template <lex::ty... l> 
        expr getExpr();

        template <> param<temp::inst>& get<temp::inst,param>(){

            param<temp::inst> res;
            while(kwFound<KW_LYTQ,KW_QUAL>()){nextTOK();}pubQuals(res);
            res.vl=getExpr<lex::ty::comma,lex::ty::gtangle>();
            
            return res;};
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
                else if(OneOfKw<KW_LYTQ,KW_QUAL>()){
                    while(kwFound<KW_LYTQ,KW_QUAL>()){nextTOK();}pubQuals(res);
                }
                else {
                     if(lexitback().t==lex::ty::Name){res.t=param<temp::meta>::ty::Type;
                        res.tp = get_accMember_seq<false>();
                        res.memberList = get_accMember_seq<false>();
                        }
                        if(OneOfLex<lex::ty::mul>()){res.t=param<temp::meta>::ty::PtrToMember};
                        else {res.name=res.memberList.back().name; res.memberList.clear();}
                        packCheck();return res;
                    }
                    else {err::e(*this,UnexpectedToken());}
                }
        };
        
        template <temp q> param_list<q> get<q,param_list>(){param_list plist;
            
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
                accMember_seq ls = get_accMember_seq();
                pl.emplace_back(ls);
                for(nextTOK();OneOfLex<lex::ty::mul>();nextTOK()){pl.back().ptrNum++;}
                for(;OneOfLex<lex::ty::band>();nextTOK()){pl.back().refNum++;}
                for(qual it : quals){pl.back().push(quals);}
            }
            return pl;
        };

        void isFuncPtr(accMember_seq& acc){
            std::string name;nextTOK();
            if(!OneOfLex<lex::ty::Name>()){err::e(*this,ResolveError(););}
            name=lexitback().u.name;
            nextTOK();
            if(!OneOfLex<lex::ty::rparen>()){err::e(*this,ResolveError();)}
            nextTOK();if(!OneOfLex<lex::ty::lparen>()){err::e(*this,ResolveError();)}
            arg_list<temp::meta> prml = getFptrArgList();
            value vl = value(stmt::FuncDecl(name,acc,prml),value::t::funcPtr);
            expectErr<lex::ty::rparen>();
            cast->pushStmt<stmt::VarDecl>(stmt::VarDecl(vl));
        };
        bool getDType(dtype& tp){
            if constexpr (lang == language::cpp ){bool b=false;
                if(OneOfLex<lex::ty::mul>()){tp.ptrNum=lexitback().u.unum;b=true;}
                if(OneOfLex<lex::ty::band>()){tp.refNum++;b=true;}
                else if(OneOfLex<lex::ty::oand>()){tp.refNum+=2;b=true;}
                if(OneOfLex<lex::ty::lbrack>()){tp.arrSize=getExpr<lex::ty::rbrack>();b=true;}
                return b;
            };
        }
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
            if(OneOfLex<lex::ty::pack>()){vedecl.pack=true;}
            else {getDtype(vdecl.tp.dt);}
            expectErr<lex::ty::Name>();{vdecl.name=lexitback().u.name;nextTOK();}
            if(lexitback().t==lex::ty::lbrack){nextTOK();vdecl.tp.arrSize=getExpr<lex::ty::rbrack>();nextTOK();}
            if(lexitback().t==lex::ty::eq){vdecl.DefaultValue=getExpr<l...>();return vdecl;}
            else {OneOfLex<l...>();return vdecl;}
            return vdecl;
        };
        stmt::arg_list getArgList(){  stmt::arg_list args;
                for(;!OneOfLex<lex::ty::rparen>();nextTOK()){args.push_back(getVarDecl<lex::ty::comma,lex::ty::rparen>());};
                auto it = args.begin();
                for(;it!=args.end() ;++it){if(it.pack() and (it!=args.tail())){err::e(*this,ArgListPackNotAtEnd(args))}}
                return args;
        };
        template <lex::ty until>
        stmt::init_args getInitArgs();{
            stmt::init_args res(value::ty::initlist);
            for(;!OneOfLex<until>();nextTOK()){res.push_back(getExpr<lex::ty::comma,until>());}
            return res;
        };


        template <lex::ty... l> 
        void getExprName(expr& res){
            if(OneOfKw<kw_New>()){res.push<op::ty::opNew,expr::node::opty::prefixUnary>();nextTOK();}
            else if(OneOfKw<kw_Delete>()){res.push<op::ty::opNew,expr::node::opty::prefixUnary>();nextTOK();}
            else if(OneOfKw<kw_Noexcept>()){res.push<op::ty::opNoExcept,expr::node::opt::prefixUnary>();
            if(kwFound<KW_CASTS>()){}
            
        if(OneOfKw<kw_sizeof>()){res.push<op::ty::opSizeof>();return;}
        nextTOK();expectErr<lex::ty::lparen>();ret.emplace( getExpr<lex::ty::rparen>());return ret;
                accMember_seq accl = get_accMember_seq<true>();
                if(!accl.back().args.empty()){res.emplace(value(accl,value<temp::meta>::ty::funcCall));}
                else if(OneOfLex<lex::ty::lbrace>()){nextTOK();res.emplace(value(getInitArgs<lex::ty::rbrace>()),value::ty::initlist);}
            return res;
        };
        
        attrib_list _atlist ;
        attrib::argList getAtArgsList(){
            attrib::argList res;
            for(nextTOK();lexitback().t!=lex::ty::rparen;nextTOK()){
                if(lexitback().t==lex::ty::comma){continue;}
                res.push_back(lexitback());};nextTOK();return res;
        };
        attrib getAttribute(){attrib at;
             if(lexitback().t==lex::ty::Name){at=attrib(lexitback().u.name);nextTOK();
            if(OneOfLex<lex::ty::lparen>()){nextTOK();at.args=getInitArgs<lex::ty::rparen>();nextTOK();}
                        if(lexitback().t!=lex::ty::dcolon){at.Ns=true;nextTOK();
                            expectWarn<lex::t::Name>();
                            at.nsacc=at.name;
                            at.name=lexitback().u.name;}
                    }
            if(lexitback().t==lex::ty::lparen){atlist.back().args=getAtArgsList();}             

        };
        attrib_list getAttributes(){
            attrib_list atlist;
            nextTOK();
            expectWarn<lex::ty::Name>();
            if(OneOfKw<kw_Using>()){
                nextTOK();expectErr<lex::ty::Name>();
                atlist.useNs=true;atlist.nsUse=lexitback().u.name;
                nextTOK();expectErr<lex::ty::colon>();
            };
            for(nextTOK();!OneOfLex<lex::ty::rdi>();nextTOK()){atlist.push_back(getAttribute());
                if(OneOfLex<lex::ty::pack>()){atlist.pack=true;}
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
            if(lex::ty::ldi){res.front_ats=getAttributes();nextTOK();}
            if(lex::ty::lparen){l=getArgList();nextTOK();}
            else{err::e(*this,UnexpectedToken<lambda<temp::meta>>());}
            if(lex::ty::Name){if(kw_Noexcept::check(lexitback().u.name)){res.Noexcept=true;};nextTOK();}
            if(lex::ty::ldi){res.back_ats=getAttributes();nextTOK();}
            if(lex::ty::arrow){res.trailing=true;nextTOK();res.rettp=get_accMember_seq<lex::ty::lbrace>();nextTOK();}// NOTE reconsider for C++26 feature: ({pre,post} contract specifiers)
            if(lex::ty::lbrace){for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){Stmt();}}
            else{err::e(*this,UnexpectedToken<lambda<temp::meta>>());}
            if(res.Template){cast->curtemp.pop_back();}
            value vl;vl.t=value::lambdav;pri::get<lambda<temp::meta>>(vl.val)=res;
            return res;
        };
        void SwExpr(expr& res){
            switch(lexitback().t){
                    case lex::ty::dcolon : {res.push(getExprName<l...>());}
                    case lex::ty::Name :{
                        if(OneOfKw<kw_and>()){res.push<op::ty::opand>();break;}
                        else if(OneOfKw<kw_or>()){res.push<op::ty::opor>();break;}
                        else if(OneOfKw<kw_xor>()){res.push<op::ty::opxor>();break;}
                        else if(lexitback().u.name==std::string("true")){res.pushLiteral(true);break;}
                        else if(lexitback().u.name==std::string("false")){res.pushLiteral(false);break;}
                        if(kwFound<KW_LYTQ,KW_QUAL>()){while(kwFound<KW_LYTQ,KW_QUAL>()){nextTOK();}pubQuals(res);break;}
                        res.emplace(getExprName<l...>(res));}
                    case lex::ty::Numflt:{res.pushLiteral(lexitback().u.flt);};
                    case lex::ty::Numuint:{res.pushLiteral(lexitback().u.num);};
                    case lex::ty::bnot : {res.push<op::ty::opbnot>();break;};
                    case lex::ty::band:{res.add<op::ty::oband>();break;}
                    case lex::ty::dot:{res.add<op::ty::opdot>();break;}
                    case lex::ty::arrow:{res.add<op::ty::arrow>();break;}
                    
                    #ifdef CHAR_LITERALS
                    case lex::ty::sq :{res.pushLiteral(lexitback().u.chr);}
                    case lex::ty::dq :{res.pushLiteral(lexitback().u.name);}
                    #endif
                    case lex::ty::lparen:{nextTOK();
                        if(!res.e.back().TrailOp()){res.addArgs(getArgInit<lex::ty::rparen>());};
                        else{res.pushExpr<op::ty::ocall>(getExpr<lex::ty::rparen>());break;}}
                    case lex::ty::rparen:{err::e(&this,UnexpectedToken());break;}
                    case lex::ty::lbrace:{ nextTOK();res.emplace(getInitArgs<lex::ty::rbrace>());break;}
                    case lex::ty::lbrack:{res.push<op::ty::oindex>(); res.addArgs<false>()(getArgInit<lex::ty::rbrack>());break;}
                    case lex::ty::Not:{res.push<op::ty::opNot>();break;}
                    case lex::ty::plus:{res.push<op::ty::oplus>();break;}
                    case lex::ty::minus:{res.push<op::ty::ominus>();break;}
                    case lex::ty::bor:{res.push<op::ty::obor>();break;}
                    case lex::ty::bxor:{res.push<op::ty::obxor>();break;}
                    case lex::ty::mul:{res.push<op::ty::omul>();if(lexitback().u.unum>1){res.refNum=lexitback().u.unum;} break;}
                    case lex::ty::div:{res.push<op::ty::odiv>();break;}
                    case lex::ty::ltangle:{if(!atTemplateType){res.add<op::ty::olt>()};break;}
                    case lex::ty::lteq:{res.push<op::ty::ogt>();break;}
                    case lex::ty::gtangle:{res.push<op::ty::ogt>();break;}
                    case lex::ty::gteq:{res.add<op::ty::ogteq>();break;}
                    case lex::ty::comma:{err::e<err::t::unexpectedToken>(*this);break;}
                    case lex::ty::semicolon:{return res;}
                    case lex::ty::colon:{res.add<op::ty::opcolon>();break;}
                    case lex::ty::space:{break;}
                    case lex::ty::cond:{res.push<op::ty::opcond>();atCond=true;
                            nextTOK();res.emplace(getExpr<lex::ty::colon,l...>());expectErr<lex::ty::colon>();
                            res.push<op::ty::opcolon,expr::node::opty::binary>();
                            nextTOK();if(OneOfLex<lex::ty::lparen>()){res.emplace(getExpr<lex::ty::rparen,l...>());expectErr<lex::ty::rparen>();nextTOK();}                        
                        break;}
                    case lex::ty::eq:{res.push<op::ty::eq>();break;}
                    case lex::ty::peq:{res.push<op::ty::opeq>();break;}
                    case lex::ty::meq:{res.push<op::ty::omeq>();break;}
                    case lex::ty::xoreq:{res.push<op::ty::opxoreq>();break;}
                    case lex::ty::andeq:{res.push<op::ty::opandeq>();break;}
                    case lex::ty::oreq:{res.push<op::ty::oporeq>();break;}
                    case lex::ty::Noteq:{res.push<op::ty::opNoteq>();break;}
                    case lex::ty::muleq:{res.push<op::ty::opmuleq>();break;}
                    case lex::ty::diveq:{res.push<op::ty::opdiveq>();break;}
                    case lex::ty::pack:{res.push<op::ty::oppack>();break;}
                    case lex::ty::oand:{res.push<op::ty::opand>();break;}
                    case lex::ty::oor:{res.push<op::ty::opor>();break;}
                    case lex::ty::three:{res.push<op::ty::opthree>();break;}
                    case lex::ty::pp:{res.push<op::ty::opp>();break;}
                    case lex::ty::mm:{res.push<op::ty::omm>();break;}
                }
        };
        template <lex::ty... l>
        void ContinueExpr(expr& res){
            if(lexitback().t==lex::ty::lbrack){res.tree.val = getLambda();return res;};
            for(;!OneOfLex<l...>(lexptrback()->t);nextTOK()){SwExpr(res);}
            return res;
        };
        
        template <lex::ty... l>
        expr getExpr(){expr res(lexitback().pos);return ContinueExpr<l...>(res);};
        
        template <> arg_list<temp::meta> get<stmt::arg_list>();

        template <typename STMTty,cntxt c>
        void getStmt();
        template <typename STMTty,cntxt c>
        void _getStmt();

        template <cntxt c>
        void Stmt();
        
        template < cntxt c>
        void FStmt();        

        template <cntxt c>void _getStmt<stmt::block,c>(){
            for(;lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>();};
            cast->popbl();
        };  
        void getBody(){
            lexq.push_back(lexitback());
            _getStmt<stmt::block>();
            erase()
        };
        template <cntxt c>void _getStmt<stmt::NS,c>(){
            for(++lexptrback();lexptrback().t!=lex::ty::lbrace;++lexptrback()){
                if(lexptrback().t==lex::ty::Name){cast->emplace_back<stmt::NS>(it->u.name;);
                    if(!cast->pushNS(pri::get<stmt::NS>(cast->curBlock()->back()))){
                        err::e<err::t::namespaceNotAllowedInCurrentScope>(*this);};continue;}
                if(!OneOfLex<lex::ty::space,lex::ty::nl>(lexptrback().t)){err::e<err::t::unexpectedToken>(*this);}
            };
            open<lex::ty::lbrace>();
        };
        void pushats(attrib_list* atptr){if(!_atlist.empty()){!atptr=_atlist;}}
        template <cntxt c>void _getStmt<stmt::While,c>(){
        
            stmt::While* res=cast->pushStmt<stmt::While>();pushats(&res->atlist);
            nextTOK();expectErr<lex::ty::lparen>();nextTOK();res->condition=getExpr<lex::rparen>();
            nextTOK();expectErr<lex::ty::lbrace>();
            for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>();}
            nextTOK();
            cast->popbl();
        };
        template <cntxt c>void _getStmt<stmt::For,c>(){
            stmt::For* res=cast->pushStmt<stmt::For>();pushats(&res->atlist);
            nextTOK();expectErr<lex::ty::lparen>();
            for(nextTOK();!OneOfLex<lex::ty::semicolon,lex::ty::colon>();nextTOK()){}
            nextTOK();res->init=getExpr<lex::ty::semicolon,lex::ty::colon>();
            if(OneOfLex<lex::ty::colon>()){
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
        template <cntxt c>void _getStmt<stmt::Do,c>(){
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
        
        template <cntxt c>void _getStmt<stmt::Switch,c>(){
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
        template <cntxt c>void _getStmt<stmt::Try,c>(){
            stmt::Try res; 
            for(nextTOK();OneOfLex<lex::ty::space,lex::ty::nl>();nextTOK()){}
            if(!OneOfLex<lex::ty::lbrace>()){err::e<err::t::unexpectedToken>(*this);}
            cast->pushbl(res.body);
            for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>();};
            cast->popbl();
            cast->pushStmt(res);
        };
        
        template <cntxt c>void _getStmt<stmt::Catch,c>(){
            cast->pushStmt<stmt::Catch>();
            stmt::Catch& res= pri::get<stmt::Catch>(*getStmt());
            cast->tries.top()->catches.push_back(&res);nextTOK();
            nextTOK();expectErr<lex::ty::lparen>();
            nextTOK();if(OneOfLex<lex::ty::ldi>()){res->atlist=getAttributes();}
            res.var=getVarDecl<lex::ty::rparen>();nextTOK();
            expectErr<lex::ty::lbrace>();
            for(nextTOK();lexitback().t!=lex::ty::rbrace;nextTOK()){FStmt<Strct>();}
            cast->popbl();nextTOK();
            if(lexitback().t==lex::ty::semicolon){
                cast->tries.pop_back();
            }
            kwFound<kw_Catch>();

        };
        template <cntxt c>void _getStmt<stmt::Throw,c>(){
            nextTOK();
            expr vl=getExpr<lex::ty::semicolon>();
            cast->pushStmt<stmt::Throw>(vl);
        };
        
        template <cntxt c>void _getStmt<stmt::If,c>(){
            
            stmt::If* res=cast->pushStmt<stmt::If>(stmt::If()); pushats(&res->atlist);
            stmt::If* r =res;nextTOK();r->If=true;
            for(nextTOK();true;){
                if(OneOfKw<kw_ConstExpr>(lexitback().u.name)){r->push(qual::QConstExpr);nextTOK();}
                if(OneOfKw<kw_ConstEval>(lexitback().u.name)){r->push(qual::QConstEval);nextTOK();}
                expectErr<lex::ty::lparen>();nextTOK();
                r->condition = GetExpr<lex::ty::rparen>();
                nextTOK();cast->curBl.push_back(&r->body);
                if(OneOfLex<lex::ty::lbrace>()){
                    nextTOK();
                    for(nextTOK();!OneOfLex<lex::ty::rbrace>();nextTOK()){StmtPush<c>();}
                }
                else {StmtPush<c>();}
                nextTOK();cast->curBl.pop_back();
                if(OneOfKw<kw_Else>()){
                    if(!r->If){err::e(*this,KwError<kw_Else>())}
                    r=cast->pushStmt(stmt::If());}
                nextTOK();if(OneOfKw<kw_If>()){r->If=true;nextTOK();}
            };
            
        };
        template <cntxt c> void _getStmt<stmt::Goto,Stct,Func>(){
            nextTOK();cast->pushStmt<stmt::Goto>(findLabel());
        };

        template <cntxt c>void _getStmt<stmt::Return,c>(){cast->pushStmt<stmt::Return>(getExpr<lex::ty::semicolon>());};
        template <cntxt c>void _getStmt<stmt::TypeDef,c>(){
            stmt::TypeDef* res=cast->pushStmt<stmt::TypeDef>(stmt::TypeDef());
            nextTOK();
            if(OneOfKw<kw_Decltype>()){res->e=expr(handleDeclType());res->dtype=true;nextTOK();res->name=lexitback().u.name;return;}
            else if(OneOfKw<kw_Struct,kw_Class>()){
                nextTOK();if(OneOfLex<lex::ty::lbrace>()){res->anon=true;
                    stmt::DefType* r = cast->pushStmt<stmt::DefType,c>(stmt::DefType());
                    r->curacc=OneOfKw<kw_Struct>():accSpec::Public : accSpec::Private; 
                    for(nextTOK();!OneOFLex<lex::ty::rbrace>();nextTOK()){StmtPush<cntxt::strct>();}
                    nextTOK();res->anontp = &r->t; expectErr<lex::ty::Name>();res->name=lexitback().u.name;
                }
            }
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
        template <cntxt c>void _getStmt<stmt::Layout,c>(){
            
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
        template <cntxt c>void _getStmt<stmt::Expr,c>(){cast->pushStmt<stmt::Expr>(getExpr<lex::ty::semicolon>());};
        // attrib_list _atlist ;
        template <typename StmtTy,cntxt c>
        void getStmt(){
            _getStmt<StmtTy,c>();
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
            if(lexitback().t==lex::ty::ldi){res.ats=getAttributes();nextTOK();}
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

        void preC(stmt::FuncDef::preC& p){
            nextTOK();if(OneOfLex<lex::ty::ldi>()){p.atilist=getAttributes();nextTOK();};
            expectErr<lex::ty::lparen>();p.e=getExpr<lex::ty::rparen>();
        };
        
        void postC(stmt::FuncDef::postC& p){
            nextTOK();if(OneOfLex<lex::ty::ldi>()){p.atilist=getAttributes();nextTOK();};
            expectErr<lex::ty::lparen>();nextTOK();
            auto it  = lexptrback();nextTOK();
            if(OneOfLex<lex::ty::colon>()){p.res_name=it->u.name;nextTOK();p.e=getExpr<lex::ty::rparen>();}
            else {--lexptrback();p.e=getExpr<lex::ty::rparen>();}
            nextTOK();
        };
        template <cntxt c,bool OutsideDef=false>
        void func(accMember_seq& rett,accMember& name,param_list<temp::meta>& pll){
            arg_list args=getArgList();bool Declared=false;nextTOK();
            try{cast->findFunc(name);}catch(const NameNotFound& e){Declared=true;}
            stmt::FuncDecl* alr;
            if constexpr (OutsideDef){
                if(Declared){alr = pri::get<stmt::FuncDecl*>(name.back().inst);}
                else {err::e(*this,OutSideDefinition();)}
                if(OneOfLex<lex::ty::semicolon>() ){err::e(*this,alr);return;}
            }
            else {
                if(Declared){alr = pri::get<stmt::FuncDecl*>(name.back().inst);}
                else {
                    res=cast->pushStmt<stmt::FuncDecl>(stmt::FuncDecl());
                    res->name=name.back();res->args=args;res->ret=rett;res.prms=plist;
                }
            if(OneOfLex<lex::ty::semicolon>() ){nextTOK();return;}

            }
            stmt::FuncDef* r;                 
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

        template <cntxt c>
        void var(accMember_seq& rett,accMember_seq& name,bool btemp,param_list<temp::meta>&& plist,size_t refn=0,size_t ptrn=0){// CalledFrom eq or ;
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
                    else {res.emplace_back(get_accMember_seq<lex::ty::comma,lex::ty::semicolon,lex::ty::lbrace>(),AccDefault<acc>::acc);nextTOK();}
                    if(lexitback().t==lex::ty::Name){ res.back().data=get_accMember_seq<lex::ty::comma,lex::ty::semicolon,lex::ty::lbrace>();nextTOK();}
                }
            };
            return res;
        };


        template <cntxt c,bool outside=false>
        void Operator(bool btemp,param_list<temp::meta> plist , accMember_seq& accl,accMember_seq& toCls=NULL){
            
            nextTOK();op::ty o;
            auto lamarrCh = [&o](op::ty n , op::ty arr){nextTOK();if(OneOfLex<lex::ty::ltbrack>()){o=arr}else{o=n}; if()nextTOK();expectErr<lex::ty::gtbrack>();}
            auto lamCast = [&o](op::ty h){}
            std::string n ;
            if(OneOfLex<lex::ty::dq>()){ 
                nextTOK();expectErr<lex::ty::dq>();nextTOK();o=lex::ty::dq;n=lexitback().u.name;
                
            }
            else if(OneOfLex<LEX_OPER>()){o = lexitback().t;}
            else if(OneOfLex<lex::ty::lparen>()){o=lex::ty::lparen;nextTOK();expectErr<lex::ty::rparen;}
            else if(OneOfLex<lex::ty::lbrack>()){o=lex::ty::lbrack;nextTOK();expectErr<lex::ty::rbrack;}
            else if(OneOfKw<kw_New>()){lamarrCh(op::ty::opNew,op::ty::opNewArr);}
            else if(OneOfKw<kw_Delete>()){lamarrCh(op::ty::opDelete,op::ty::opDeleteArr);}
            nextTOK();
            param_list<temp::inst> pll;
            if(OneOfLex<lex::ty::ltangle>()){param_list<temp::inst> pll = get<temp::inst,param_list>();}
            expectErr<lex::ty::lparen>();
            stmt::Operator* alr;bool found=true;
            if constexpr (outside){auto it = toCls.tail();--it;
                
                try {if(o==lex::ty::dq){
                        switch(it->r){
                        case result::rType: {alr=  pri::get<stmt::DeclType*>(r->inst).findOperatorLit(o);break;}
                        case result::rNS: {alr=pri::get<stmt::NS*>(r->inst).findOperatorLit(o);break;}
                        default : {err::e(*this,MemberAccess(toCls));break;}
                        }   
                    }
                    else {
                        switch(it->r){
                            case result::rType: {alr=  pri::get<stmt::DeclType*>(r->inst).findOperator(o);break;}
                            case result::rNS: {alr=pri::get<stmt::NS*>(r->inst).findOperator(o);break;}
                            default : {err::e(*this,MemberAccess(toCls));break;}
                        }
                    }
                }
                catch (const OperatorNotFound& e){err::e(*this,NameNotFound());}                
            }
            else {
                try {
                    if(o==lex::ty::dq){findOperatorLit(n);}
                    else {alr= cast->findOperator(o);}
                }catch (const OperatorNotFound& e){found=false;alr =cast->stmtPush<stmt::Operator>(stmt::Operator(o));}
            }
            stmt::arg_list args=getArgList();
            alr->args=getArgList();nextTOK();
            stmt::FuncDef::preC pc;stmt::FuncDef::postC poc;bool preb=false;bool postb=false;
            while(kwFound<KW_QUAL>()){nextTOK();}
            accMember_seq rets ;
            if(OneOfLex<lex::ty::arrow>()){nextTOK();accMember_seq rets = get_accMember_seq();}
            while(OneOfKw<kw_pre,kw_post>()){
                if(OneOfKw<kw_pre>()){preC(pc);preb=true;}
                else if(OneOfKw<kw_post>()){postC(poc);postb=true;}
                nextTOK();
            };
            accMember_seq arret;
            if(OneOfLex<lex::ty::arrow>()){nextTOK();arret=  get_accMember_seq();}
            // pubQuals(res);
            if(OneOfLex<lex::ty::lbrace>()){nextTOK();
                stmt::FuncDef * res ;alr->push(plist,pll);res->setContracts(pc,preb,poc,postb);
                cast->curBl.push_back(&res->body);pushats(res);res->args=args;res->arrowRet=arret;
                for(;OneOfLex<lex::ty::rbrace>();nextTOK()){StmtPush<c,true>()}
                erase();
            }
            else if(OneOfLex<lex::ty::semicolon>()){
                if(found){err::e(*this,AlreadyDeclared<stmt::Operator>(alr));}    
                else {alr->setContracts(pc,preb,poc,postb);res->arrowRet=arret;}
                pubQuals(alr);
            }
            erase()
        };

        template <bool accPubblic,cntxt c>
        void checkStrct(bool bTemp=false,stmt::param_list& plist={}){
             accMember_seq accl = get_accMember_seq<lex::ty::semicolon>();
                        accMember_seq acc;size_t ptrNum=0;size_t refNum=0; 
                       getDtype(accl.dt);
                        if (OneOfLex<lex::ty::Name>()){
                            acc = get_accMember_seq<lex::ty::semicolon>();
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
        };

        template <cntxt c,bool Union=false>
        void _Strct(bool bTemp=false,accSpec access,stmt::param_list& plist=NULL){
            nextTOK();            
            if(OneOfLex<lex::ty::lbrace>()){
                nextTOK();
                stmt::DefType* cur  = cast->pushStmt<stmt::DefType>(stmt::DefType());cur->curacc=access;
                if constexpr (c==cntxt::strct){cast->strcts.back()->anons.push_back(&cur->types.back());}
                else if constexpr (c==cntxt::ns) {cast->nss.back()->anons.push_back(&nss->types.back());}
                for(;!OneOfLex<lex::ty::rbrace>();nextTOK()){StmtPush<true,false>();}
                cur->resolve(*cast);return;
            };
            accMember_seq accl = get_accMember_seq<false>();
            if(OneOfKw<kw_Operator>()){Operator<c>(bTem,plist,accl);return;}
            accMember_seq acc;
            if(getDtype(accl.dt));
            if(OneOfLex<lex::ty::Name>()){
                acc = get_accMember_seq<false>();
                if(acc.size()>1){
                    if(OneOfLex<lex::ty::lparen>()){func<c,true>(accl,acc,plist);return;}
                    expectErr<lex::ty::mul>(); nextTOK();expectErr<lex::ty::Name>();
                    stmt::VarDecl* res = cast->pushStmt<stmt::VarDecl>(stmt::VarDecl(accl,acc,lexitback().u.name),refNum,ptrNum);   
                }
                else {
                    if(OneOfLex<lex::ty::lparen>()){func<c,false>(accl,acc,plist);return;}
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
                    spec->t.resolve(*cast);return;
                    cast->strcts.pop_back();cast->curtemp.pop_back();return;
                };
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
                    if(exists){res = &alr->push(plist,pll); }
                    else {res = &alr.top();}
                    if constexpr(Union){res->t.isUnion=true;}
                    res->curacc=access;cast->strcts.push_back(res);res->inherits =inh;
                        for(nextTOK();!OneOfLex<lex::ty::rbrace>();nextTOK()){StmtPush<cntxt::strct>();}
                        res->resolve(*cast);
                        cast->strct.pop_back();
                };
            }                   
            erase();return;
        };
        template <bool tdef,cntxt c>
        void Union(bool bTemp=false,param_list<temp::meta>& plist=NULL){_Strct<c,true>(bTemp,accSpec::Public,plist);};
        template <cntxt c>
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
            res->expr = get_accMember_seq<lex::ty::semicolon>();
        };        
        void StructuredBinding(){
            nextTOK();auto res = cast->pushStmt(stmt::StructuredBinding());
            for(nextTOK();!OneOfLex<lex::ty::rbrack>();nextTOK()){
                if(OneOfLex<lex::lex::ty::pack>()){nextTOK();res->bn.emplace_back(lexitback().u.name,true);}
                else {expectErr<lex::ty::Name>();res->bn.emplace_back(lexitback().u.name);
            }
            expectErr<lex::ty::eq>();nextTOK();
            res->e=getExpr<lex::ty::semicolon>();
        };
        void 
        template <cntxt c> 
        void NameStmt(bool bTemp,param_list<temp::meta>& plist){ 

            accMember_seq acclist ;
            if(OneOfKw<kw_Auto>()){
                nextTOK();
                dtype dt;
                getDtype(dt);
                if(OneOfLex<lex::ty::lbrack>()){
                    if(bTemp){err::e(this,TemplateNotAllowed<stmt::StructuredBinding>());}
                    if constexpr(c!=cntxt::func){err::e(*this,OutsideOf<cntxt::func,stmt::StructuredBinding>())}
                    StructuredBinding();return;}
                    else {acclist.Auto=true;}
            }
            else {acclist = get_accMember_seq<false>();}
            getDtype(acclist);
                if(OneOfKw<kw_Operator>()){Operator<c>(bTemp,plist,acclist);return;}
                if(OneOfLex<lex::ty::colon>()){
                    if(acclist.Auto){err::e(*this,LabelName(lexitback()));untilLex<lex::ty::semicolon,lex::ty::rbrace>();}
                    cast->pushStmt<stmt::Label>(stmt::Label(acclist.back().name));return;}
                if(OneOfLex<lex::ty::lparen>() ) 
                    nextTOK();if(OneOfLex<lex::ty::mul>()){isFuncPtr(acclist);return;}
                    else{acclist.args=getInitArgs<lex::ty::rparen>();expr e(acclist);
                        pushats(cast->pushStmt(stmt::Expr(ContinueExpr<lex::ty::semicolon>(e))->atlist));return;}
                };
                    if(OneOfLex<lex::ty::eq>() or pri::OneOf<lex::ty ,lex::ty::dot,lex::ty::arrow >(acclist.back().acc)){
                        expr ex;ex.emplace(value(acclist));
                        cast->pushStmt<stmt::Expr>(ContinueExpr(ex));return;
                    }
                if(!OneOfLex<lex::ty::Name>()){ 
                    expr ex;ex.emplace(value(acclist));
                    cast->pushStmt<stmt::Expr>(ContinueExpr(ex));return;
                }
                else {
                    accMember_seq nm = get_accMember_seq() ;
                    if(nm.back().name==kw_Operator.str()){Operator<c,true>(bTemp,plist,acclist,nm);return;}


                    for(;!OneOfLex<lex::ty::semicolon>();){
                    stmt::VarDecl* res;
                        if(nm.back().acc == lex::ty::dcolon){
                            expectErr<lex::ty::mul>();nextTOK();
                            res=cast->pushStmt(stmt::VarDecl(acclist,nm,lexitback().u.name,refNum,ptrNum));return;
                            
                        }
                    if(OneOfLex<lex::ty::lparen>()){func<c>(acclist,nm,plist);return;}
                    if(OneOfLex<lex::ty::lbrack>()){
                        nextTOK();
                        if(!OneOfLex<lex::ty::rbrack>()){res->arr=true;res->arrSize=getExpr<lex::ty::rbrack>();}
                    }
                    if(nm.size()==1 and OneOfLex<lex::ty::comma,lex::ty::semicolon,lex::ty::eq>()){
                        res=cast->pushStmt<stmt::VarDecl>(stmt::VarDecl()) ; 
                        res->name =nm.back().name;res->tp=acclist;
                        if(OneOfLex<lex::ty::eq>()){nextTOK();res->DefaultValue = getExpr<lex::ty::semicolon,lex::ty::comma>();}
                        
                    }
                    else {err::e(*this,UnexpectedToken();)}
                        if(OneOfLex<lex::ty::comma>()){nextTOK();};
                            try {nm = get_accMember_seq();}
                        catch ( const accMemberStop& e){}
                    }
                }
        };
        
        void include(std::filesystem::path pth){this->f.push_back(std::ifstream(pth));this->curFilePath.push_back(pth);pos.emplace_back(posit(curFilePath.back())) ;};
        void includeRel(std::filesystem::path l){
            std::filesystem::path wd=cwd.back();
            std::filesystem::path pth = wd;pth/=l;
            if(std::filesystem::exists(pth)){include(pth);}
            else {err::e(*this,fileNotFound(pth));}
        };
        void includeAbs(std::filesystem::path l){
            for(std::filesystem::path p : dirs.arr ){
                std::filesystem::path pth = p;pth/=l;
                if(std::filesystem::exists(pth)){include(pth);return;}
            };
            err::e(*this,fileNotFound(pth));
        };
        template<>void _getMacro<mStmtInclude>(){
            until<lex::ty::nl>();
            ++(lexptrback());
            if(lexptrback()->t==lex::ty::ltangle){
                ++(lexptrback());
                std::filesystem::path p;
                for(;lexptrback()->t!=lex::ty::gtangle;++(lexptrback())){
                    if(OneOfLex<lex::ty::div,lex::ty::escape>(lexptrback()->t)){continue;}
                    if(lexptrback()->t!=lex::ty::name){err::e(*this,UnexpectedToken());break;}
                    p/=std::filesystem::path(lexptrback()->u.name);
                }
                includeAbs(p);
            };
            else if(lexptrback()->t==lex::ty::dq){
                ++lexptrback();
                std::filesystem::path p;
                for(;lexptrback()->t!=lex::ty::dq;lexptrback()){
                    if(OneOfLex<lex::ty::div,lex::ty::escape>(lexptrback()->t)){continue;}
                    if(lexptrback()->t!=lex::ty::name){err::e(*this,UnexpectedToken());break;}
                    p/=std::filesystem::path(lexptrback()->u.name);
                }
                includeRel(p);
            }
            else {err::e(*this,UnexpectedToken());}            
            erase();
        };
        template<>void _getMacro<mStmtIf>(){
            cast->condition= getExprUntil_EOL();
            // cast->tus.emplace_back(ex);
            erase();
            untilKW<false,kw_Else,kw_elif,kw_elifdef,kw_elifndef,kw_endif>();
        };
        template<>void _getMacro<mStmtDefine>(){until<lex::ty::Name>();macros.emplace(lexptrback()->u.name,strippedUntil<lex::ty::nl>());erase();};
        template<>void _getMacro<mStmtElif>(){getMacro<mStmtIf>();};
        template <bool b>
        bool cond(bool t){return b?t:!t;};
        template <bool b>
        void IfDf(){
            until<lex::ty::Name>();
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
        template<>void _getMacro<mStmtPragma>(){nextTOK();expectErr<lex::ty::Name>();
            for(pragma it : pragmas){
                if(it->name == lexitback().u.name){it.proc(*this);}
            }
        }

        template <typename T> getMacro(){_getMacro<T>();}


        void getOperatorType(bool bTemp,param_list<temp::meta>& plist){
            nextTOK();
            expectErr<lex::ty::Name>();
            
                accMember_seq accl ;
                accl = get_accMember_seq<pren_false>();
                stmt::Operator* res =  cast->pushStmt<stmt::Operator>(stmt::Operator(accl));
                expectErr<lex::ty::lparen>();
                res->args=getArgList();
                nextTOK();
                while(kwFound<KW_QUAL>()){nextTOK()};
                pubQuals(res);if(bTemp){res->plist=plist;}
                if(OneOfLex<lex::ty::lbrace>()){nextTOK();
                    cast->curBl.push_back(res->Def.body);
                    for(;!OneOfLex<lex::ty::rbrace>();nextTOK()){StmtPush<true,true>();}
                    nextTOK();
                };
                return ;
        };
        void GetFriend(){nextTOK();
            
            nextTOK();bool bPack=false;bool blist=true;
            lexq.push_back(lexptrback());
            if(kw_Class::check(lexitback().u.name or kw_Struct::check(lexitback().u.name))){
                nextTOK();
                result r;resty<temp::meta> res;lex::ty acc=lex::ty::none;
                ref_handle(acc,&r,&res);
                reslt.t=mtype::ty::rType;pri::get<type<temp::meta>*>(reslt.var)=res.
            }
            else {accMember_seq accl ;
                accl = get_accMember_seq();
                if(kw_Operator::check(lexitback().u.name)){
                op::ty r=getOperator();reslt.opt=r;
                if(r==op::ty::opType){
                    reslt.op = get_accMember_seq<lex::ty::lparen,lex::ty::semicolon>();    
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
            if(OneOfLex<lex::ty::lbrace>()){nextTOK();res.init_list.emplace_back(r,true,getInitArgs<lex::ty::rbrace>());}
            if(OneOfLex<lex::ty::lparen>()){nextTOK();res.init_list.emplace_back(r,false,getInitArgs<lex::ty::rparen>());}
        };
        void Constructor(bool bTemp,param_list<temp::meta>& plist){
                       curPtr.push_back(lexptrback()); 

            nextTOK();

            stmt::Constructor* res=cast->pushStmt<stmt::Constructor>(stmt::Constructor());
            if(bTemp){res->plist=plist;cast->curtemp.push_back(&res->plist)};
            if(kwFound<kw_Noexcept>()){nextTOK();}
            if(OneOfLex<lex::ty::ldi>()){res->atlist = getAttributes();nextTOK();}
            else {pushats(res->atlist)};
            
            res->pushQuals(quals);
            expectErr<lex::ty::lparen>();res->args = getArgList();

            nextTOK();
            if(OneOfLex<lex::ty::eq>()) {nextTOK();
                if(OneOfKw<kw_Default>()){res->Default=true;}
                else {err::e(*this,ExpectedKw<kw_Default>());}
                nextTOK();
                expectErr<lex::ty::semicolon>();erase();return;
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
            for(nextTOK();!OneOfLex<lex::ty::rbrace>();nextTOK()){StmtPush<cntxt::func>();};
            cast->curBl.pop_back();nextTOK();

            if(bTemp){cast->curtemp.pop_back();}; 
            erase()
        };

        void AsmBlock(){
           curPtr.push_back(lexptrback()); 
            nextTOK();expectErr<lex::ty::lparen>();
            stmt::Asm* res = cast->pushStmt<stmt::Asm>(stmt::Asm());
            if(OneOfLex<lex::ty::dq>()){res->strlit=lexitback().u.name;
                for(;OneOfLex<lex::ty::colon>();nextTOK()){
                    std::string cc;std::string re;
                    nextTOK();if(OneOfLex<lex::ty::dq>()){cc=lexitback().u.name;nextTOK();
                        if(OneOfLex<lex::ty::lparen>()){nextTOK();re=lexitback().u.name;nextTOK();}
                        res->blt_seq.emplace_back(cc,re);
                    }
                }
            }
            nextTOK();expectErr<lex::ty::semicolon>();
            erase();
        };
        template <Strct>
        void Concept(param_list<temp::meta>& pl){
            nextTOK();auto res = cast->pushStmt<stmt::Concept>(stmt::Concept(pl));
            if(OneOfLex<lex::ty::ldi>()){nextTOK();res->atlist=getAttributes();nextTOK()}
            expectErr<lex::ty::eq>();nextTOK();
            if(OneOfKw<kw_requires>()){nextTOK();
                res->reqr=true;
                expectErr<lex::ty::lparen>();res->argl=getArgList();
                if(OneOfLex<lex::ty::lbrace>()){nextTOK();
                    for(nextTOK();!OneOfLex<lex::ty::rbrace>();nextTOK()){
                        stmt::Concept::condition h;
                        if(OneOfLex<lex::ty::lbrace>()){
                            nextTOK();h.e=getExpr<lex::ty::rbrace>();nextTOK();
                            if(OneOfLex<lex::ty::arrow>()){nextTOK();
                                h.rslt=getExpr<lex::ty::semicolon>()
                            }
                            res->cds.push_back(h);
                        }
                    }
                }
                return;
            }   
            res->e=getExpr<lex::ty::semicolon>();
        };
        template <cntxt c,bool Inline=false>
        void Namespace(){
            if(c!=cntxt::ns){throw NameSpaceNotAllowed();}
            nextTOK();
            if(OneOfLex<lex::ty::Name>()){
                stmt::NS* res=cast->pushNS<Inline>(lexitback().u.name);
                nextTOK();expectErr<lex::ty::lbrace>();
                for(nextTOK();!OneOfLex<lex::ty::rbrace>()){StmtPush<cntxt::ns>();}
                nextTOK();res->resolve(cast);
            }
        };


        constraint get_constraint(){nextTOK();

            constraint c = expr(lexitback().pos);
        
            for(;OneOfLex<lex::ty::semicolon>()){
                if(OneOfLex<lex::ty::Name>() and e.back().e.o == op::ty::none){break;}
                SwExpr(c);
            };
        };
        #define PUBLIC_ACCESS true
        #define PRIVATE_ACCESS false
        template <cntxt c>
        [[likely]] bool StmtPush(){
            
            if(lexitback().t==lex::ty::ldi){_atlist = getAttributes(); nextTOK(); }
            
            if(lexitback().t==lex::ty::lbrace){
                if constexpr (Func){
                    cast->pushStmt<stmt::block>(stmt::block);
                    for(nextTOK();OneOfLex<lex::ty::rbrace>();nextTOK()){StmtPush<c>();}
                return true;
                }
                else throw UnexpectedToken(lexitback().t);
            };

            if(lexitback().t==lex::ty::semicolon){auto res=cast->pushStmt(stmt::NullStmt());pushats(res->atlist);return;}
            if(OneOfLex<lex::ty::dcolon>()){}
            else if(OneOfLex<lex::ty::bnot>()){
                nextTOK();
                if constexpr (c==cntxt::strct){
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
                if(OneOfKw<kw_Asm>()){AsmBlock();}
                if(OneOfKw<kw_Template>()){bTemp=true;
                    if constexpr (Func){err::e(*this,UnexpectedToken("Template Not allowed in function body"));}
                    nextTOK();expectErr<lex::ty::ltangle>();
                    plist = get<temp::meta,param_list>();until<lex::ty::Name,lex::ty::gtangle,lex::ty::space,lex::ty::nl>();
                    nextTOK(); if(OneOfKw<kw_requires>()){nextTOK();plist.constraint = get_constraint();nextTOK();}
                    cast->curtemp.push_back(&plist);
                };
                if(OneOfKw<kw_concept>()){Concept<c>(plist);}
                if constexpr (c==cStrct){
                    if(lexitback().u.name==cast->strcts.back().t.name){Constructor(bTemp,plist);return;} 
                    else if(OneOfKw<kw_Operator>()){ GetOperatorType(bTemp,plist);return;}
                    else if(OneOfKw<kw_Friend>()){GetFriend();return;}
                    else if(OneOfKw<kw_Public>()){cast->curAcc=accSpec::Public;nextTOK();expectErr<lex::ty::colon>();};
                    else if(OneOfKw<kw_Private>()){cast->curAcc=accSpec::Private;nextTOK();expectErr<lex::ty::colon>();};
                    else if(OneOfKw<kw_Protected>()){cast->curAcc=accSpec::Protected;nextTOK();expectErr<lex::ty::colon>();};
                }
                if(OneOfKw<kw_Namespace>()){Namespace<c>();return;}
                else if(OneOfKw<kw_Typedef>()){getStmt<stmt::TypeDef,c>();return;}
                else if(OneOfKw<kw_Enum>()){Enum(bTemp,plist);return;}
                else if(OneOfKw<kw_Struct>()){_Strct<c>(bTemp,accSpec::Public,plist);return;}// Attbs after
                else if(OneOfKw<kw_class>()){_Strct<c>(bTemp,accSpec::Private,plist);return;}
                else if(OneOfKw<kw_Union>()){Union<false,Strct>(bTemp,plist);return;}
                else if(OneOfKw<kw_Using>()){Using<Strct>(bTemp,plist);return;}
                
            }
            NameStmt<c>(bTemp,plist);
        };



        
        template <cntxt c>
        void Stmt(){StmtPush<c>();}
        template <cntxt c>
        void FStmt(){Stmt<cntxt::func>();};

        ast fromFile(std::filesystem::path pth){ curFilePath.push_back(pth);
            cast=new ast();
        cwd.push(pth.parent_path());
        f.open(pth);
        pos.push_back(posit());bool fend=false;
        try {while(Line()){Stmt();}}
        catch (const FileEnd& e){fend=e.b;}
        if(pth.extension()==std::filesystem::path("hstmsl")){wrcph(pth,cur); }
        cwd.pop();
        
        cast->procFinal();
            
        return *cast;
        };
        ast fromFile(std::string pth){filePos =0; std::filesystem::path s(pth);return fromFile(s); }
        parser() = default;
    
    }
}
#endif