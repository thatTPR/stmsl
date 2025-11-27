#ifndef STMSL_LEX
#define STMSL_LEX
#include <filesystem>
namespace stmsl {
    struct posit {
            size_t ln=0;
            size_t col=0;size_t filePos=0;size_t ecol=0;
            std::filesystem::path* file;
            decltype(*this) operator++(){col++;}
            decltype(*this) operator++(int){filePos+=col;col=0;}
            bool operator<(size_t& i){return col<i;}
            bool operator<=(size_t& i){return col<=i;}
            operator size_t(){return col;};
            posit() = default;
            posit(std::filesystem::path& _pth)  {*this = {0,0,0,0,&_pth} }
            posit(posit& p) {*this=p;ecol=col;}
        };
    struct lex {
            enum ty {Name,NumFlt,NumUint,lithex,litbin,
                escape='\\',
                Not='!',
                bnot='~',
                lparen='(',
                rparen=')',
                lbrace='{',
                rbrace='}',
                lbrack='[',
                rbrack=']',ldi,rdi,//[[,]]
                dq='\"',
                sq='\'',
                dot='.',
                plus='+',
                minus='-',
                mod='%',modeq,
                band='&',
                bor='|',
                bxor='^',xoreq,
                mul='*',
                div='/',comm,blockcomm
                ltangle='<',lteq,strmlt,strmlteq,arrow,
                gtangle='>',gteq,strmgt,strmgteq,
                three,
                comma=',',
                semicolon=';',
                colon=':',
                dcolon,ptrmember,
                arrowptr,dotptr,dcolptr
                space=' ',
                cond='?',
                nl='\n',
                eq='=',peq,meq,andeq,oreq,Noteq,muleq,diveq,eqeq,
                pack,
                pp,mm,oand,oor
                
                str="#",tokpaste,
                none
            };

            std::string getStr(ty t){
                switch(t){
                    case ldi :{return std::string("[[")};
                    case rdi :{return std::string("]]")};
                    case modeq :{return std::string("%=")};
                    case xoreq :{return std::string("^=")};
                    case comm :{return std::string("//")};
                    case blockcomm :{return std::string("/*")};
                    case lteq :{return std::string("<=");}
                    case strmlt :{return std::string("<<");}
                    case strmlteq :{return std::string("<<=");}
                    case arrow :{return std::string("->");}
                    case gteq :{return std::string(">=");}
                    case strmgt :{return std::string(">>");}
                    case strmgteq : {return std::string(">>=");}
                    case dcolon :{return std::string("::");}
                    case dcolptr :{return std::string("::*");}
                    case arrowptr :{return std::string("->*");}
                    case dotptr :{return std::string(".*");}
                    case three :{return std::string("<=>");}
                    case ptrmember :{return std::string("::*");}
                    case peq : {return std::string("+=");}
                    case meq : {return std::string("-=");}
                    case andeq : {return std::string("&=");}
                    case oreq : {return std::string("|=");}
                    case Noteq : {return std::string("!=");}
                    case muleq : {return std::string("*=");}
                    case diveq : {return std::string("/=");}
                    case eqeq : {return std::string("==");}
                    case pack : {return std::string("...");}
                    case pp : {return std::string("++");}
                    case mm : {return std::string("--");}
                    case oand : {return std::string("&&");}
                    case oor : {return std::string("||");}
                    case str : {return std::string("#");}
                    case tokpaste : {return std::string("##");}
                    case none : {return std::string("");}
                    default : {return std::string(t);}
                }
            };
            #define LEX_OPER comma,plus,minus,mod,modeq,band,bor,bxor,mul,lteq,strmlt,strmlteq,arrow,gteq,strmgt,strmgteq,peq,meq,andeq,oreq,Noteq,muleq,diveq,eqeq,pp,mm,oand,oor
            #define LEX_OP2 lparen,lbrack
            
            ty t;
            union {
                float flt;int unum;
                std::string name;char chr;
                // type<meta>* ty;
            }u;
            posit pos;
            size_t size(){return (pos.ecol-pos.col);}
            void setUnum(char c){u.unum=(c-'0');}
            void addUnum(char c){u.unum*=10;u.unum+=(c-'0');pos.ecol++;}
            void addflt(char c){u.unum*=10;u.unum+=(c-'0');pos.ecol++;}
            void setFlt(size_t p){float f=u.unum;for(;p!=0;p--){f/=10;};
                u.flt=f;
            };
            void addName(char c){u.name+=c;pos.ecol++;}
            bool isType(ty ts){return t=ts;}
            template <ty... ts>
            bool Variant(ts... first){return first==t || ...;};

            template <ty... ts>
            bool typeOf(){return ts==t || ...;}

            lex(ty tt) : t(tt){} 
            lex(char c)  {t=ty::Name;u.name=std::string(c);}
            
            lex(uint n) : u.unum(n){t=NumUint;};
            lex(int n) : u.inum(n){t=NumInt;};
            lex(float n) : u.flt(n){t=NumFlt;};
            lex(std::string n) : u.name(n) {t=Name;}
            lex(std::string n,lex _l) : u.name(n) ,pos(l.pos) {t=Name;}


            lex(posit _pos,ty tt) : pos(_pos)  t(tt){} 
            lex(posit _pos,char c) : pos(_pos) {t=ty::Name;u.name=std::string(c);}
            lex(posit _pos,uint n) : pos(_pos)  u.unum(n){t=NumUint;};
            lex(posit _pos,int n) : pos(_pos)  u.inum(n){t=NumInt;};
            lex(posit _pos,float n) : pos(_pos)  u.flt(n){t=NumFlt;};
            
        };
        


                using tylexq=pri::deque<lex>;
            template <lex::ty cur,lex::ty l,lex::ty... ls>
            struct mgraph {
                static constexpr size_t size = sizeof...(ls)+1; 
                static constexpr lex::ty c= cur; 
                static constexpr lex::ty arr[] = {l,ls...};
            };  
            template <lex::ty l,typename MGt,size_t s=0>
            struct lex_pt {static constexpr bool value = MGt::arr[s]==l;};
            template <typename MGt,typename... MGts>
            struct max_graphs {static constexpr bool maxs = MGts::size>max_graphs<MGts...>::maxs?MGt::size:max_graphs<MGts...>::maxs;}
            template <typename MGt>
            struct max_graphs {static constexpr bool maxs = MGts::size;}


            template <typename MGt,typename... MGts>
            struct mgs{ 
                static constexpr size_t maxs = max_graphs<MGt,MGts...>::maxs;                
                using tup = pri::tuple<MGt,MGts...> ;
                template <typename... Ts>
                mgs<Ts...> operator[](lex::ty l){lex::ty h}

                constexpr mgs(){

                };
            };
            // TODO see if multi_graphs can be templated;
};


#endif