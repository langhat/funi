#ifndef FIMAIN_CPP
#define FIMAIN_CPP

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

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

std::string default_funi_config 
=R"deffc({
    path: {
        lib: "lib/",
        pwd: ""
    },
    init: _ -> @("builtin_func.fi")
})deffc";
std::string fi_gitignore
=R"figi(# IDE Settings
*~
.vscode/
.vs/

# Executables
*.exe
*.out
*.app
bin/)figi";
std::string project_fi
=R"project_fi({
    name: "new_project",   // name
    version: 1,  // unreadable version
    version_readable: "0.1.0",  // readable version
    main: "src/main.fi",    // main file
    test: { // test files
        // test information: test file path
    },
    depend: {   //depend list
        // package name: version number
    }
})project_fi";

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
            if(args[i] == "run") {
                ifstream ifp(args[++i]);
                string temp, content;
                while(getline(ifp, temp)){
                    content += temp + "\n\n";
                }

                fiRunner fir;
                try {
                    fir.run (content);
                } catch (const exception &e){
                    cout << "\033[31m";
                    for(auto &ec: fir.error_trace) {
                        cout<<ec<<endl;
                    }
                    fir.error_trace.clear();
                    cout << "\033[0m";
                }
                cout<<endl;
            }else if(args[i] == "expr") {
                vector<string> object;
                split(args[++i],object);

                fiRunner fir;
                try {
                    visit(printVisitor{}, fir.expr(object));
                } catch (const exception &e){
                    for(auto &ec: fir.error_trace) {
                        cout<<ec<<endl;
                    }
                    fir.error_trace.clear();
                }
                cout<<endl;
            }else if(args[i] == "visit") {
                ifstream ifp(args[++i]);
                string temp, content;
                while(getline(ifp, temp)){
                    content += temp + "\n\n";
                }
                fiRunner fir;
                try {
                    fir.run (content);
                } catch (const exception &e){
                    cout << "\033[31m";
                    for(auto &ec: fir.error_trace) {
                        cout<<ec<<endl;
                    }
                    fir.error_trace.clear();
                    cout << "\033[0m";
                }

                vector<string> object;
                split(args[++i], object);

                visit(printVisitor{}, fir.expr(object));
                cout<<endl;
            }else if(args[i] == "repl") {
                repl();
            }else if(args[i] == "init") {
                ofstream ofp("funi_config.fi");
                ofp << default_funi_config;
                ofp.close();
            }else if(args[i] == "new") {
                const auto &path = args[++i];

                mkdir((path + "docs").c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                mkdir((path + "src").c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

                ofstream ofp(path + "docs/index.md");
                ofp.close();

                ofp.open(path + "src/main.fi");
                ofp << "main = _ -> unit";ofp.close();

                ofp.open(path + ".gitignore");
                ofp << fi_gitignore;ofp.close();

                ofp.open(path + "README.md");
                ofp.close();

                ofp.open(path + "project.fi");
                ofp << project_fi;ofp.close();
            }
		}
        return 0;
    }
private:
    void repl() {
        using namespace std;
        fiRunner fir;

        string temp;

        while(1){
            vector<string> object;

            cout<<">>> "<<flush;
            getline(cin,temp);
            split(temp, object);

            try {
                auto val = fir.expr(object);
                if(!holds_alternative<Unit> (val)) visit(printVisitor{}, val);
            } catch (const exception &e){
                cout << "\033[31m";
                for(auto &ec: fir.error_trace) {
                    cout<<ec<<endl;
                }
                fir.error_trace.clear();
                cout << "\033[0m";
            }
            cout<<endl;
        }
    }
};

#endif
