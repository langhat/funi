#ifndef fiRunner_CPP
#define fiRunner_CPP

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<ranges>
#include<functional>
#include<fstream>

#define DEBUG_TRACE do {cout<<"$";\
	for(auto &ec: code)\
		cout<<ec<<" ";\
	\
	cout<<"$\n";\
} while(0)

/*
USAGE: match
match `origin` and change it 2 `object` from `code`
*/
std::string match(std::string code,
	const std::string& origin, const std::string& object) {
    if (origin.empty()) {
        return code;
    }
    size_t pos = 0;
    while ((pos = code.find(origin, pos)) != std::string::npos) {
        code.replace(pos, origin.length(), object);
        pos += object.length();
    }
    return code;
}

#include"spliter.cpp"
#include"value.cpp"
#include"visitor.cpp"

/*

WRARNING: GetArgs has a ul if

*/

#define GetArgs vector<string> tv,tmp;\
						split(a_s,tv);\
						vector<vector<string>> args;\
						for(auto &ec:tv){\
							if(ec==","){\
								if(1){\
									args.push_back(tmp);\
									tmp.clear();\
								}\
							}else{\
								tmp.push_back(ec);\
							}\
						}\
						if(!tmp.empty()){\
							args.push_back(tmp);\
							tmp.clear();\
						}

class NotExistError:public std::exception{
public:
	const char *what()const noexcept{
		return "NotExistError : ?The sun raise from the west!";
	}
};

class ExpectLeft:public SyntaxError{
	const char *what()const noexcept{
		return "SyntaxError : Lvalue required as left operand of assignment";
	}
};

class BadFuncCall:public SyntaxError{
	const char *what()const noexcept{
		return "SyntaxError : Expression cannot be used as a function";
	}
};

class PackageNotFound:public SyntaxError{
	const char *what()const noexcept{
		return "SyntaxError : Except a package but not found";
	}
};

class UncalableExpr:public SyntaxError{
	const char *what()const noexcept{
		return "SyntaxError : Find a uncalable expr";
	}
};

template<typename T>
std::vector<T> back_vec(const std::vector<T> &vec){
	std::vector<T> ret;
	for(const auto &ec:vec | std::views::reverse){
		ret.push_back(ec);
	}
	return ret;
}

class fiRunner{
	std::set<std::string> types = {
		"Int",
		"Bool",
		"Real",
		"Str",
		"Func",
		"Unit"
	};
	std::map<std::string, value> valtb;
	std::string vtoa(const std::vector<std::string> &vec){
		std::string ret;
		for(auto &ec: vec){
			ret+=ec+" ";
		}
		return ret;
	}
public:
	std::vector<std::string> error_trace;
	int line;
	
	std::vector<std::string> paths;

	fiRunner(): paths{"lib/"} {
		line = 0;
	}

	void load(const std::vector<std::string> &npaths) {
		for(const auto &each: npaths) {
			paths.push_back(each);
		}
	}

	void run(const std::string &code){
		line=0;
		using namespace std;
		vector<string> object;
		split(code,object);
		
		vector<string> expr;

		string ret;
		
		for(auto &ec:object){
			//cout<<ec<<" | ";
			if(ec!="\n"){
				expr.push_back(ec);
			}else{
				line++;
				auto result=this->expr(expr);
				expr.clear();
			}
		}
		
		if(!expr.empty()){
			line++;
			auto result=this->expr(expr);
			expr.clear();
			//throw LessSemicolon{};
		}
	}
	value expr(const std::vector<std::string> &code){
		using namespace std;

		if(code.empty()) return Unit {};
		
		vector<string> part[3];
		int post=0;
		
		try {{
			string a_s;
			bool nohap=1;
			for(const auto &ec:code){// wrong 结合性
				if(nohap&&(ec=="=")){
					a_s=ec;
					post++;
					nohap=0;
				}else{
					part[post].push_back(ec);
				}
			}
			if(a_s=="="){
				string v1=part[0][0];
				value v2=expr(part[1]);
				if(valtb.find(v1)!=valtb.end())throw(ExpectLeft{});
				return valtb[v1]=v2;
			}else if(a_s.empty()){}
			else{
				throw NotExistError{};
			}
		}
		part[0].clear();part[1].clear();part[2].clear();
		post=0;
		
		{
			string a_s;
			bool nohap=1;
			for(const auto &ec:code){
				if(nohap&&(ec=="->")){
					a_s=ec;
					post++;
					nohap=0;
				}else{
					part[post].push_back(ec);
				}
			}
			if(a_s=="->"){
				string v1=part[0][0];
				string v2;
				for(auto &ec: part[1]){
					v2+=ec+" ";
				}
				Func f;
				f.arg=v1;
				f.body=v2;
				return f;
			}else if(a_s.empty()){}
			else{
				throw NotExistError{};
			}
		}
		part[0].clear();part[1].clear();part[2].clear();
		post=0;

		{
			bool nohap=1;
			for(const auto &ec:code){
				if(ec=="if" || ec == "else"){
					post++;
					nohap=0;
				}else{
					part[post].push_back(ec);
				}
			}
			if(!nohap){
				bool boolean;
				try {boolean = get<bool>(expr(part[1]));}
				catch(const exception &) {throw TypeError{};}

				if(boolean) {
					return expr(part[0]);
				}
				return expr(part[2]);

			}
		}
		part[0].clear();part[1].clear();part[2].clear();
		post=0;

		{
			string a_s;
			bool nohap=1;
			for(const auto &ec:code){
				if(nohap&&(ec == ">=" ||
					ec == "<=" ||
					ec == "==" ||
					ec == "<" ||
					ec == ">" ||
					ec == "!=")){
					a_s=ec;
					post++;
					nohap=0;
				}else{
					part[post].push_back(ec);
				}
			}
			if(a_s.empty()){}
			else if(a_s==">="){
				value v1=expr(part[0]);
				value v2=expr(part[1]);
				return visit(greaterEqualVisitor{},v1,v2);
			}
			else if(a_s=="<="){
				value v1=expr(part[0]);
				value v2=expr(part[1]);
				return visit(lessEqualVisitor{},v1,v2);
			}
			else if(a_s=="=="){
				value v1=expr(part[0]);
				value v2=expr(part[1]);
				return visit(equalVisitor{},v1,v2);
			}
			else if(a_s=="<"){
				value v1=expr(part[0]);
				value v2=expr(part[1]);
				return visit(lessVisitor{},v1,v2);
			}
			else if(a_s==">"){
				value v1=expr(part[0]);
				value v2=expr(part[1]);
				return visit(greaterVisitor{},v1,v2);
			}
			else if(a_s=="!="){
				value v1=expr(part[0]);
				value v2=expr(part[1]);
				return visit(notEqualVisitor{},v1,v2);
			}
			else{
				throw NotExistError{};
			}
		}
		part[0].clear();part[1].clear();part[2].clear();
		post=0;

		{
			string a_s;
			bool nohap=1;
			for(const auto &ec:code | views::reverse){
				if(nohap&&(ec=="+"||ec=="-")){
					a_s=ec;
					post++;
					nohap=0;
				}else{
					part[post].push_back(ec);
				}
			}
			part[0]=back_vec(part[0]);
			part[1]=back_vec(part[1]);
			if(a_s=="+"){
				value v1=expr(part[0]);
				value v2=expr(part[1]);
				return visit(addVisitor{},v2,v1);
			}else if(a_s.empty()){}
			else if(a_s=="-"){
				value v1=expr(part[0]);
				value v2=expr(part[1]);
				return visit(subVisitor{},v2,v1);
			}
			else{
				throw NotExistError{};
			}
		}
		part[0].clear();part[1].clear();part[2].clear();
		post=0;
		
		{
			string a_s;
			bool nohap=1;
			for(const auto &ec:code | views::reverse){
				if(nohap&&(ec=="*"||ec=="/"||ec=="%")){
					a_s=ec;
					post++;
					nohap=0;
				}else{
					part[post].push_back(ec);
				}
			}
			part[0]=back_vec(part[0]);
			part[1]=back_vec(part[1]);
			if(a_s=="*"){
				value v1=expr(part[0]);
				value v2=expr(part[1]);
				return visit(mulVisitor{},v2,v1);
			}else if(a_s.empty()){}
			else if(a_s=="/"){
				value v1=expr(part[0]);
				value v2=expr(part[1]);
				return visit(divVisitor{},v2,v1);
			}else if(a_s=="%"){
				value v1=expr(part[0]);
				value v2=expr(part[1]);
				return visit(modVisitor{},v2,v1);
			}
			else{
				throw NotExistError{};
			}
		}
		part[0].clear();part[1].clear();part[2].clear();
		post=0;

		{
			string a_s;
			bool nohap=1;
			for(const auto &ec:code){
				if(nohap&&(ec == "..")){
					a_s=ec;
					post++;
					nohap=0;
				}else{
					part[post].push_back(ec);
				}
			}
			if(a_s.empty()){}
			else if(a_s==".."){
				value v1=expr(part[0]);
				string v2;

				try {
					v2=get<string>(expr(part[1]));
				} catch(const exception &){
					throw TypeError{};
				}

				if(visit(typeofVisitor{},v1) == v2) {
					return v1;
				}else throw TypeError{};
			}
			else{
				throw NotExistError{};
			}
		}
		part[0].clear();part[1].clear();part[2].clear();
		post=0;

		{
			string a_s;
			bool nohap=1;
			for(const auto &ec:code){
				if(nohap&&(ec == "as")){
					a_s=ec;
					post++;
					nohap=0;
				}else{
					part[post].push_back(ec);
				}
			}
			if(a_s.empty()){}
			else if(a_s=="as"){
				value v1=expr(part[0]);
				string v2;

				try {
					v2=get<string>(expr(part[1]));
				} catch(const exception &){
					throw TypeError{};
				}

		        if(types.find(v2) == types.end()) throw TypeError{};

		        return visit(typeCastVisitor(v2), v1);
			}
			else{
				throw NotExistError{};
			}
		}
		part[0].clear();part[1].clear();part[2].clear();
		post=0;

		{
			string a_s;
			bool nohap=1;
			for(const auto &ec:code | views::reverse){
				if(nohap&&(ec[0]=='(')){
					a_s=ec;
					post++;
					nohap=0;
				}else{
					part[post].push_back(ec);
				}
			}
			part[0]=back_vec(part[0]);
			part[1]=back_vec(part[1]);
			if(a_s.empty()){}
			else{
				a_s=a_s.c_str()+1;
				a_s.erase(a_s.size()-1);
				if(part[1].empty()){}
				else if(part[1].size()==1){
					//special
					if(part[1][0]=="__out"){
						GetArgs
						auto arg=expr(args[0]);
						visit(printVisitor{},arg);
						return Unit{};
					}else if(part[1][0] == "exit"){
						exit(0);
					}else if(part[1][0] == "@") {
						//paths
						string filename;

						GetArgs
						try {
							filename = get<string>(expr(args[0]));
						} catch(const exception &){
							throw TypeError{};
						}

						ifstream ifp;
						ifp.close();
						int index = 0;

						while(!ifp) {
							if(index == paths.size()) throw PackageNotFound{};
							ifp.open(paths[index++] + filename);
							cerr << paths[index] << filename << endl;
						}

						string temp, content;
						while(getline(ifp, temp)) {
							content += (temp + "\n");
						}

						try {
							int tl = line;
							line = 0;
							run(content);
							line = tl;
						} catch(const exception &e) {
							error_trace.push_back(
								string("[Note] in file \"") +
								filename + "\"");
							throw;
						}

						return Unit{};
					}else if(part[1][0] == "typeof") {
						GetArgs
						auto arg=expr(args[0]);
						return visit(typeofVisitor{},arg);
					}
					else{
						//a lambda or a func
						goto LambdaCall;
					}
				}else{
					//a lambda or a func
					goto LambdaCall;
				}
				goto specialCallEnd;
				LambdaCall:{
					auto func=expr(part[1]);

					GetArgs
					vector<string> object;
					if(args.empty()) {
						split(match(string(" ")+get<Func>(func).body+" ", " "+get<Func>(func).arg+" ", "unit"),object);
						return expr(object);
					}

					for(auto post=0;post<args.size();post++){
						auto arg=visit(loadVisitor{},expr(args[post]));
						split(match(string(" ")+get<Func>(func).body+" ",
							" " + get<Func>(func).arg + " ",
							"( " + arg + " )"),
							object);
						func = expr(object);
					}

					return func;
				}specialCallEnd:;
			}
		}
		part[0].clear();part[1].clear();part[2].clear();
		post=0;
		
		if(code.size()==1){
			if(code[0] == "true") return true;
			else if(code[0] == "false") return false;
			else if(code[0] == "unit") return Unit{};

			if(types.find(code[0]) != types.end()) return *types.find(code[0]);

			bool onlynum=1,onlypoint=1,
			pb=code[0][0]=='(',
			fpb=code[0][0]=='{',
			str=(code[0][0]=='\"'||code[0][0]=='\'');
			if(pb){
				//()求值
				string temp=code[0].c_str()+1;
				temp.erase(temp.size()-1);
				vector<string> tv;
				split(temp,tv);
				return expr(tv);
			}else if(fpb){
				//func
				// string temp=code[0].c_str()+1;
				// temp.erase(temp.size()-1);
				
				// Lambda lbd;
				// lbd.codeBlock=match(temp,
				// 	[&,this](const string &val){
				// 		vector<string> tv;
				// 		split(val,tv);
				// 		return visit(loadVisitor{},expr(tv));
				// 	}
				// );
				// return lbd;
			}else if(str){
				string temp=code[0].c_str()+1;
				temp.erase(temp.size()-1);
				return temp;
			}
			for(auto &ec:code[0]){
				if(ec<'0'||ec>'9'){
					onlynum=0;
					if(ec!='.'){
						onlypoint=0;
						break;
					}
				}
			}
			if(onlynum){
				return stoll(code[0]);
			}else if(onlypoint){
				return stold(code[0]);
			}else if(valtb.find(code[0])!=valtb.end()){
				return valtb[code[0]];
			}
		}
		post=0;
		
		throw UncalableExpr{}; //return Unit{};

		} catch (const exception &e) {
			//错误处理
			if(error_trace.empty()) {
				error_trace.push_back(string("\033[31m") + e.what() + "\033[0m");
				error_trace.push_back(string("[Note] line ") + to_string(line));
			}

			string code_content;
			for(auto &ec: code)
				code_content += ec;
			
			error_trace.emplace_back(
				string("[Note] here: ") +
				code_content
			);
			throw;
		}
	}
};

#endif
