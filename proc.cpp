#include<fstream>
#include <iostream>
#include <string>
#include<algorithm>
bool isdigit(char c){
    return (c>='0' and (c<='9'));
}
int num_line(std::string& line,bool* hex){
    int num=-1;
    for(int i=0;i<line.size();i++){
        if(line[i]==' ' or (line[i]=='\n')){continue;}

        if(isdigit(line[i])){num=0;
            if(line[i+1]=='x'){
                i+=2;
                *hex=true;
                for(;isdigit(line[i]);i++){
                num*=0x10;num+=(line[i] - '0' )>9?line[i] - 'A' + 0xA:(line[i] - '0' );
                }
                return num;
            } ;
            num=line[i] - '0';
            for(i++;isdigit(line[i]);i++){
                num*=10;num+=line[i] - '0';
            }
            return num;
            continue;
        };
        return -1;
    }
    return num;
}
void isNum(std::string* s){

};
int main(){
    std::string path=std::string("./spv.md");
    std::ifstream fi(path);
    std::string out=std::string("./spvp3.hpp");
    std::ofstream of(out);
    std::string line;
    bool started=false;
    while(std::getline(fi,line)){
        bool startInstr=false;
        if(line.find(std::string("3.2. Enumerants")) !=std::string::npos  and !started){started=true;std::cout<<"Started";continue;}
        if(line.find(std::string("3.2. Enumerants")) !=std::string::npos and started){\
            of<<"\\*ENUMERANTS*\\";
            while(std::getline(fi,line)){
                if(line.find(std::string("3.2."))!= std::string::npos){
                    start: 
                    
                    of<<"}\nenum"<<line<<"{\n";
                    while(std::getline(fi,line)){
                        int num=-1;bool hex=false;
                        num=num_line(line,&hex);
                        std::string name;
                        if(num!=-1 ){
                            std::getline(fi,line);
                            std::getline(fi,line);
                            name=line;
                            of<<name<<"=";
                            if(hex){of<<std::hex<<num;}
                            else{of<<std::dec<<num;}
                            of<<",\n";
                            
                        }
                        if(line.find(std::string("3.2."))!= std::string::npos){
                            goto start;
                        }

                    if(line.find(std::string("3.3. Instructions"))!= std::string::npos){break;startInstr=true;};
                    }
                    ;}
                    if(line.find(std::string("3.3. Instructions"))!= std::string::npos){break;startInstr=true;};
                    
            }
        }
        if(startInstr){
            of<<"\\\\Instructions\n";
            label: 
            if(line.find(std::string("3.3."))!=std::string::npos){
                of<<"}\nenum"<<" "<<line.substr(5)<<"{\n";
            while(std::getline(fi,line)){
                if(line.find(std::string("Op")) != std::string::npos){
                    of<<line<<"=";
                    int wordCount=-1;bool wchex;bool varword=false;
                    int opCode=-1;bool ochex;
                    while(std::getline(fi,line)){
                        if(wordCount==-1){
                            wordCount=num_line(line,&wchex);
                            if(line.find(std::string("+ variable"))!=std::string::npos){
                                varword=true;
                            };
                        }
                        else {opCode=num_line(line,&ochex);}
                        if(opCode!=-1){break;}
                    };

                    // of<<name<<"=";
                    if(ochex){of<<std::hex<<opCode;}
                    else{of<<std::dec<<opCode;}
                    of<<",\\\\";
                    if(wchex){of<<std::hex<<wordCount;}
                    else {of<<std::dec<<wordCount;}
                    if(varword){of<<"+variable";}
                    of<<std::endl;
                }
                if(line.find(std::string("3.3."))!=std::string::npos){goto label;}
                if(line.find(std::string("4.1"))!=std::string::npos){return 1;}
            }
        };
    }

    };
}