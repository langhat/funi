#ifndef FIMAIN_CPP
#define FIMAIN_CPP

#include<iostream>
#include<string>
#include<vector>
#include<fstream>

#include"returnCode.h"
#include"fiRunner.cpp"

std::string fiPath;

const std::string pathC=
#ifdef _WIN32
"\\"
#else
"/"
#endif
;

#define notexit(cond,value) do{if(!(cond))exit(value);}while(false)

class fiMain{
    std::vector<std::string> args;
	bool autoconnect=false;
    public:
        fiMain(std::vector<std::string> _args={}):args(std::move(_args)){

        }
        int run(){
			using namespace std;
			string temp;
			for(int i=0;i<args.size();i++){
                if(args[i]=="run"){
                    ifstream ifp(args[++i]);
                    string temp,content;
                    while(getline(ifp,temp)){
                        content+=temp+"\n\n";
                    }
                    fiRunner fir;
                    fir.run(content);
                    cout<<endl;
                }else if(args[i]=="expr"){
                    vector<string> object;
                    split(args[++i],object);

                    fiRunner fir;
                    visit(printVisitor{}, fir.expr(object));
                    cout<<endl;
                }else if(args[i]=="visit"){
                    ifstream ifp(args[++i]);
                    string temp,content;
                    while(getline(ifp,temp)){
                        content+=temp+"\n\n";
                    }
                    fiRunner fir;
                    fir.run(content);

                    vector<string> object;
                    split(args[++i],object);

                    visit(printVisitor{}, fir.expr(object));
                    cout<<endl;
                }
			}
            return 0;
        }
};

#endif
