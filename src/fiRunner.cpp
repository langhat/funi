#ifndef fiRunner_CPP
#define fiRunner_CPP

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<ranges>
#include<functional>
#include<fstream>

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

class LessSemicolon:public SyntaxError{
public:
	const char *what()const noexcept{
		return "SyntaxError : Expect a semicolon int the end of the line but not";
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

class BindFailed:public SyntaxError{
	const char *what()const noexcept{
		return "SyntaxError : `bind` failed";
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
	std::map<std::string, value> valtb;
	std::string vtoa(const std::vector<std::string> &vec){
		std::string ret;
		for(auto &ec: vec){
			ret+=ec+" ";
		}
		return ret;
	}
public:
	fiRunner(){
	}
	void run(const std::string &code){
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
				auto result=this->expr(expr);
				expr.clear();
			}
		}
		
		if(!expr.empty()){
			auto result=this->expr(expr);
			expr.clear();
			//throw LessSemicolon{};
		}
	}
	value expr(const std::vector<std::string> &code){
		using namespace std;
		
		vector<string> part[3];
		int post=0;
		
		{
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
					v2+=ec;
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
					auto func=get<Func>(expr(part[1]));
					
					//prepare for args
					GetArgs
					// for(auto post=0;post<args.size();post++){
					// 	expr(args[post]);
					// }
					auto arg=visit(loadVisitor{},expr(args[0]));

					vector<string> object;
					split(match(func.body, func.arg, arg),object);
					return expr(object);

				}specialCallEnd:;
			}
		}
		part[0].clear();part[1].clear();part[2].clear();
		post=0;
		
		if(code.size()==1){
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
		
		return Unit{};
	}
};

#endif
