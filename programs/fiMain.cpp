#ifndef LWMAIN_CPP
#define LWMAIN_CPP

#include<iostream>
#include<string>
#include<vector>
#include<fstream>

#include"returnCode.h"
#include"fiRunner.cpp"

std::string lwPath;

const std::string pathC=
#ifdef _WIN32
"\\"
#else
"/"
#endif
;

#define notexit(cond,value) do{if(!(cond))exit(value);}while(false)

class lwMain{
    std::vector<std::string> args;
	bool autoconnect=false;
    public:
        lwMain(std::vector<std::string> _args={}):args(std::move(_args)){

        }
        int run(){
			using namespace std;
			string temp;
			for(int i=0;i<args.size();i++){

			}
            return 0;
        }
};

#endif
