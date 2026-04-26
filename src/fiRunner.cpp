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

class WrongArgsNum:public SyntaxError{
	const char *what()const noexcept{
		return "SyntaxError : Wrong args number";
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
		"Object",
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
	std::set<value *> rubbish;
public:
	std::vector<std::string> error_trace;
	int line;
	
	std::vector<std::string> paths;

	fiRunner(): paths{"lib/"} {
		line = 0;
	}

	~fiRunner() {
		for(const auto &each: rubbish) {
			delete each;
		}
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
							//cerr << paths[index] << filename << endl;
						}

						string temp, content;
						while(getline(ifp, temp)) {
							content += (temp + "\n\n");
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
					}else if(part[1][0] == "copy" || part[1][0] == "new") {
					    GetArgs

					    if (args.size() != 1) {
					        throw WrongArgsNum{};
					    }

					    value original_val = expr(args[0]);
					    

					    value copied_val = std::visit([this](auto&& arg) -> value {
					        using Type = std::decay_t<decltype(arg)>;

					        if constexpr (
					            std::is_same_v<Type, long long>    || // Int
					            std::is_same_v<Type, long double>  || // Real
					            std::is_same_v<Type, bool>         || // Bool
					            std::is_same_v<Type, std::string>  || // Str
					            std::is_same_v<Type, Func>         || // Func
					            std::is_same_v<Type, Unit>            // Unit
					        ) {
					            return arg;
					        }

					        else if constexpr (std::is_same_v<Type, Object*>) {
					            if (arg == nullptr) return nullptr;

								if(arg->properties.find("new") !=
									arg->properties.end()) {
									
									auto func=arg->properties.find("new")->second;

									vector<string> object;
									split(match(string(" ")+get<Func>(func).body+" ",
											" "+get<Func>(func).arg+" ",
											"unit")
										,object);
									return expr(object);
								}

					            Object* new_obj = new Object();

					            function<value(value&&)> lbd;
					            lbd = [&lbd](auto&& v) -> value {
					                return v;//std::visit(lbd, v);
					            	//object light copy: WARN TODO
					            };

					            for (const auto& [key, val] : arg->properties) {
					                new_obj->properties[key] = std::visit(lbd, val);
					            }

					            rubbish.insert(new value(new_obj));
					            return new_obj;
					        }

					        else {
					            throw TypeError{};
					        }
					    }, original_val);

					    return copied_val;
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
		

		{
			string a_s;
			bool nohap=1;
			for(const auto &ec:code | views::reverse){
				if(nohap&&(ec == ".")){
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
			else if(a_s=="."){
				value v1=expr(part[1]);
				string v2=part[0][0];

				// 检查v1是否为Object类型
				if(std::holds_alternative<Object*>(v1)){
					Object* obj = std::get<Object*>(v1);
					// 查找成员
					if(obj->properties.find(v2) != obj->properties.end()){
						return obj->properties[v2];
					}else{
						throw NotExistError{};
					}
				}else{
					throw TypeError{};
				}
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
					auto tmp = expr(part[1]);
					if(tmp.index() == 6) { //Object
						if(v1.index() != 6) throw TypeError{};
						for(const auto &[key, _]: get<Object *>(tmp)->properties){
							if(get<Object *>(v1)->properties.find(key) == get<Object *>(v1)->properties.end())
								throw TypeError{};
						}
						return v1;
					}
					v2=get<string>(tmp);
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
					auto tmp = expr(part[1]);
					if(tmp.index() == 6) { //Object
						if(v1.index() == 6) { // first use v1 's type-cast function
							if(get<Object *>(v1)->properties.find("to"+part[1][0])
								!= get<Object *>(v1)->properties.end()) {
								
								auto func=(get<Object *>(v1)->properties.find("to"+part[1][0]))->second;

								vector<string> object;
								split(match(string(" ")+get<Func>(func).body+" ",
										" "+get<Func>(func).arg+" ",
										visit(loadVisitor{}, v1))
									,object);
								return expr(object);
							}
						}
						if(get<Object *>(tmp)->properties.find("cast") !=
							get<Object *>(tmp)->properties.end()) {
							
							auto func=get<Object *>(tmp)->properties.find("cast")->second;

							vector<string> object;
							split(match(string(" ")+get<Func>(func).body+" ",
									" "+get<Func>(func).arg+" ",
									visit(loadVisitor{}, v1))
								,object);
							return expr(object);
						}
						throw TypeError{};
					}
					v2=get<string>(tmp);
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
				// Object literal
				string temp=code[0].c_str()+1;
				temp.erase(temp.size()-1);
				
				// 创建一个新的Object对象
				Object* obj = new Object();
				
				// 解析Object字面量中的token
				vector<string> tokens;
				split(temp, tokens);
				
				// 遍历tokens，提取键值对
				for (size_t i = 0; i < tokens.size();) {
					// 跳过逗号
					if (tokens[i] == ",") {
						i++;
						continue;
					}
					
					// 键值对格式：key : value
					if (i + 2 < tokens.size() && tokens[i+1] == ":") {
						string key = tokens[i];
						
						// 移除键的引号（如果是字符串字面量）
						if (!key.empty() && key.size() >= 2 && 
						    ((key.front() == '"' && key.back() == '"') || (key.front() == '\'' && key.back() == '\''))) {
							key = key.substr(1, key.size() - 2);
						}
						
						// 解析值表达式（从i+2开始，直到下一个逗号或结束）
						vector<string> valueExpr;
						for (size_t j = i + 2; j < tokens.size() && tokens[j] != ","; j++) {
							valueExpr.push_back(tokens[j]);
						}
						
						// 解析值表达式
						value val = expr(valueExpr);
						
						// 将键值对添加到Object的properties中
						obj->properties[key] = val;
						
						// 移动到下一个键值对
						i += 2 + valueExpr.size();
					} else {
						// 格式错误，跳过当前token
						i++;
					}
				}
				
				// 将Object*添加到rubbish中
				rubbish.insert(new value(obj));
				
				return obj;
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
