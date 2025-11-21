// #define MACROSP
#include <acqres/xml.cpp>

typedef struct table {
    std::string name ; 
    struct field {
        std::string type ; 
        std::string name ;
        std::string index; 
    };

    std::vector<field> strct ;
} table;

std::ofstream ofs;
void writeFile(std::ostream& of, table it){
        size_t pos = it.name.find(" ");
        if(pos!= std::string::npos){
            it.name[pos]='_';
        };
        for(int i = 0 ; i < it.name.size();i++){
            if(it.name[i] =='\''){
                it.name.erase(it.name.begin()+i);
            }
        };
        for(table::field& s :it.strct ){
            if(!s.name.empty()){
                size_t pos = s.name.find("[");
                if(pos != std::string::npos){
                    size_t pos2 = s.name.find("]",pos+1);
                    if(pos2 != std::string::npos){
                        s.index=s.name.substr(pos+1,pos2-(pos+1));
                        s.name=s.name.substr(0,pos);
                    }
                    else {
                    s.name = s.name.substr(0,pos);
                    s.index = "variable";
                }
                }
            };
        };
        bool use ;
   
        of<<"enum {\n"<<it.name;
       
        for(const auto& f : it.strct){
            if(!f.index.empty()){
                of<<f.name<<"="<<f.type<<"m";
                use = true;
                of<<"//"<<"["<<f.index<<"]";
            }
            else {
                of<<f.name<<"="<<f.type<<";";
            }
            of<<std::endl;

        };
        of<<"}"<<it.name<<";";
        of<<std::endl;
        use =false ; 
};

void execTable(table tb){
    writeFile(std::cout, tb );
    writeFile(ofs, tb );
}

void onep(int s , std::string str,  table* tb){
    switch(s){
        case  0 : {tb->name=str;return;};
        case  1 : {tb->strct.push_back(table::field());return;};
        case  2 : {tb->strct.back().type = str;return;};
        case  3 : {tb->strct.back().name = str;return;};
    };
};
void pattern(std::ifstream& ifs){
    std::string one= "<table>\
<colgroup/>\
<tbody>\
<tr>\
<th>$val=0</th>\
</tr>\
<tr $iter=1>\
<td $val=2></td>\
<td $val=3></td>\
</tr>\
</tbody>\
</table>" ;
    std::string two = "<table>\
<colgroup/>\
<thead>\
<tr>\
<th>$val=0</th>\
</tr>\
</thead>\
<tbody>\
<tr $iter=1>\
<td $val=2></td>\
<td $val=3></td>\
</tr>\
</tbody>\
</table>" ;
std::string three = "<table>\
<colgroup/>\
<tr>\
<th>Type</th>\
</tr>\
<tr $iter=1>\
<td $val=2> </td>\
<td $val=3> </td>\
</tr>\
</table>";


    xmlParser::pattern<table> patone(one, &onep);
    xmlParser::pattern<table> pattwo(two, &onep);
    xmlParser::pattern<table> patthree(three, &onep);
    std::vector<xmlParser::pattern<table>> patt = {patone,pattwo,patthree};
    xmlParser p ; p.feed<table>(patt,&execTable,ifs);  
};



int main(int argc, char* argv[]){
    std::string in = std::string(argv[1]);
    std::string out = std::string(in+".hpp");
    std::cout<<"Crawling file: "<<in<<" to file: "<<out<<std::endl;
    
    // p.parse(in);
    // p.print(&p.tree) ;
    ofs.open(out);
    std::ifstream ins(in);
    if(ins.is_open()){
        std::cout<<"Opened file: "<<in<<std::endl;
    }
    pattern(ins);
    
    // for( table& it : vec ){
    //     writeFile(ofs,it);        
    // };
   
};
