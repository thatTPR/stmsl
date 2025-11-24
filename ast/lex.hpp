#ifndef STMSL_LEX
#define STMSL_LEX
#include <queue>
namespace stmsl {
    struct posit {
            size_t ln=0;
            size_t col=0;size_t filePos=0;size_t ecol=0;
            decltype(*this) operator++(){col++;}
            decltype(*this) operator++(int){filePos+=col;col=0;}
            bool operator<(size_t& i){return col<i;}
            bool operator<=(size_t& i){return col<=i;}
            operator size_t(){return col;};
            posit() = default;
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