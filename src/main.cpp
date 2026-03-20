#include<string>
#include"fiMain.cpp"

std::string path_fetch(const std::string &path){
	std::string ret;
	int i=path.size()-1;
	for(;i>=0;i--){
		if(path[i]=='\\'||path[i]=='/'){
			break;
		}
	}
	for(int post=0;post<=i;post++){
		ret+=std::string(1,path[post]);
	}
	return ret;
}

int main(int ac,char **av){
    std::vector<std::string> args;
    for(int i=1;i<ac;i++){
        args.emplace_back(av[i]);
    }
    fiPath=path_fetch(av[0]);
    fiMain fi(args);
    return fi.run();
}