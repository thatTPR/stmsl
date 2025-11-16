#include"spv5.hpp"
#include <string>
#include<type_traits>
#include <petri/list.hpp>
typedef uint32_t WORD 
// Layout


// Physical Layout
template <bool btype,bool bresult> 
struct instructionLayoutbase {
    WORD wordCountAndopcode;
    std::enable_if<btype,WORD>>::type type ;
    std::enable_if<bresult,WORD>>::type result ;
    std::list<WORD> operands;//
    uint16_t wordCount(){return wordCountAndopcode>>16;};
    uint16_t opcode(){return wordCountAndopcode&(1<<16 -1);};
    void get(WORD* set,size_t* size){
        *size= 1;
        if constexpr (btype){
            *size=*size+1;
        };
        if constexpr(bresult){
            *size=*size+1
        }
        *size= *size + wordCount();
        set= new WORD[*size];
        set[0] = wordCountAndopcode;
        if constexpr (btype){
            set[1]=type;
        };
        if constexpr(bresult){
            set[2]=type;
        }
        std::memcpy(set+6,operands.data(),2*(wordCount()-2));
    }

}


struct instructionLayout : instructionLayoutbase<true,true>;


template <typename T>struct id{

} ;

template <typename T> struct resultID ;

struct moduleLayout {
    WORD magic;
    WORD version;
    WORD genMagic;
    WORD Bound;
    WORD reserved;
    std::list<instructionLayout> layout;
};
struct controlFlow {

};