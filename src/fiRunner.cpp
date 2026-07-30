#ifndef fiRunner_CPP
#define fiRunner_CPP

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<ranges>
#include<functional>
#include<fstream>
#include <cmath>

// #define DEBUG_TRACE do {cout<<"$";\
// 	for(auto &ec: code)\
// 		cout<<ec<<" ";\
// 	\
// 	cout<<"$\n";\
// } while(0)

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

class ConfigError:public SyntaxError{
	const char *what()const noexcept{
		return "SyntaxError : Config file grammer was wrong";
	}
};

class MatchMiss:public SyntaxError{
	const char *what()const noexcept{
		return "SyntaxError : a uncountable `match`";
	}
};

class BadMemory:public SyntaxError{
	const char *what()const noexcept{
		return "SyntaxError : Memory operation failed";
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
	std::set<std::string> inced;

	std::map<std::vector<std::string>, value> cache;
	std::map<std::string, std::fstream> file_handle;

public:
	std::vector<std::string> error_trace;
	int line;
	
	std::vector<std::string> paths;

	fiRunner(): paths{"lib/"} {
		using namespace std;
		line = 0;

		ifstream ifp("funi_config.fi");
		if(!ifp) return;

		string temp, content;
		while(getline(ifp, temp)) {
			content += temp + "\n";
		}

		fiRunner fir(0); // avoid seg error
		vector<string> object;
		split(content, object);
		try {
			const auto config = get<Object *>(fir.expr(object))->properties;
			for(const auto &[note, path]: get<Object *>(config.find("path")->second)->properties) {
				paths.push_back(get<string>(path));
			}
			split(get<Func>((config.find("init")->second)).body, object);
			this->expr(object);
		}catch(const exception &) {
			throw ConfigError{};
		}
	}

	fiRunner(int li): line(li) {}

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
		if(cache.find(code) != cache.end()){
			// cout << "Cache: ";
			// for (auto &ec: code) {
			// 	cout << ec << " ";
			// }
			// cout << endl;
			return cache.find(code)->second;
		}

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
			string a_s;
			bool nohap=1;
			for(const auto &ec:code){
				if(nohap&&(ec==",")){
					a_s=ec;
					post++;
					nohap=0;
				}else{
					part[post].push_back(ec);
				}
			}
			if(a_s==","){
				expr(part[0]);
				return expr(part[1]);
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
					return cache[code] = expr(part[0]);
				}
				return cache[code] = expr(part[2]);

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
				return cache[code] = visit(addVisitor{},v2,v1);
			}else if(a_s.empty()){}
			else if(a_s=="-"){
				value v1=expr(part[0]);
				value v2=expr(part[1]);
				if(part[1].empty())v2 = 0;
				return cache[code] = visit(subVisitor{},v2,v1);
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
				return cache[code] = visit(mulVisitor{},v2,v1);
			}else if(a_s.empty()){}
			else if(a_s=="/"){
				value v1=expr(part[0]);
				value v2=expr(part[1]);
				return cache[code] = visit(divVisitor{},v2,v1);
			}else if(a_s=="%"){
				value v1=expr(part[0]);
				value v2=expr(part[1]);
				return cache[code] = visit(modVisitor{},v2,v1);
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
					if(part[1][0][0] == '@' && part[1][0] != "@"){
						return specCall(part[1][0], a_s);
					}else if(part[1][0]=="__out"){
						GetArgs
						auto arg=expr(args[0]);
						visit(printVisitor{},arg);
						cout << '\n';
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
						bool ninctw = true;

						while(!ifp) {
							if(index == paths.size() && ninctw) throw PackageNotFound{};
							if(!inced.count(paths[index++] + filename)){
								ifp.open(paths[index-1] + filename);
								if(ifp) inced.insert(paths[index-1] + filename);
							}else {
								cerr << "[Warning] Include \'" << filename << "\' for twice\n";
								ninctw = false;
							}
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

						return cache[code] = Unit{};
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
					}else if(part[1][0] == "for"){
						GetArgs
						// string
						try{
							auto val = expr(args[0]);
							try {
								auto str = get<string>(val);
								auto fnc = get<Func>(expr(args[1]));
								bool is_unordered = false, reg_on = false;
								value reg = Unit{};
								if(args.size() >= 4) {
									{
										const Object *flag = get<Object *>(expr(args[2]));
										if(auto it = flag->properties.find("unordered"); it != flag->properties.end()) {
											is_unordered = true;
										}else if(auto it = flag->properties.find("reg"); it != flag->properties.end()) {
											reg_on = true;
											reg = it->second;
										}else {
											throw TypeError{};
										}
									}
									{
										const Object *flag = get<Object *>(expr(args[3]));
										if(auto it = flag->properties.find("unordered"); it != flag->properties.end()) {
											is_unordered = true;
										}else if(auto it = flag->properties.find("reg"); it != flag->properties.end()) {
											reg_on = true;
											reg = it->second;
										}else {
											throw TypeError{};
										}
									}
								}else if(args.size() >= 3){
									const Object *flag = get<Object *>(expr(args[2]));
									if(auto it = flag->properties.find("unordered"); it != flag->properties.end()) {
										is_unordered = true;
									}else if(auto it = flag->properties.find("reg"); it != flag->properties.end()) {
										reg_on = true;
										reg = it->second;
									}else {
										throw TypeError{};
									}
								}
								if (reg_on) {
									//忽略unordered
									for (const auto &ec: str) {
										auto res = expr("(" + visit(loadVisitor{}, value(fnc)) + ")(\"" + string(1, ec) + "\"," + 
											visit(loadVisitor{}, reg) +")");
										try {
											const auto &prop = get<Object *>(res)->properties;
											if(auto it = prop.find("for_result_spec");
												it != prop.end()){
												return it->second;
											}
										}catch(const exception &) {}
										reg = res;
									}
									return reg;
								} else {
									if (is_unordered) {

									}else for (const auto &ec: str) {
										auto res = expr("(" + visit(loadVisitor{}, value(fnc)) + ")(\"" + string(1, ec) + "\")");
										try {
											const auto &prop = get<Object *>(res)->properties;
											if(auto it = prop.find("for_result_spec");
												it != prop.end()){
												return it->second;
											}
										}catch(const exception &) {}
									}
									return Unit{};
								}
							}catch(const exception &){}

							throw TypeError{};
						}catch(const exception &){
							//throw TypeError{};
						}

						// normal array
						try{
							auto val = expr(args[0]);
							try {
								const auto &obj = get<Object *>(val)->properties;

								if (obj.find("item") != obj.end() || obj.find("begin") != obj.end()) throw TypeError{};
								//check

								auto fnc = get<Func>(expr(args[1]));
								bool is_unordered = false, reg_on = false;
								value reg = Unit{};
								if(args.size() >= 4) {
									{
										const Object *flag = get<Object *>(expr(args[2]));
										if(auto it = flag->properties.find("unordered"); it != flag->properties.end()) {
											is_unordered = true;
										}else if(auto it = flag->properties.find("reg"); it != flag->properties.end()) {
											reg_on = true;
											reg = it->second;
										}else {
											throw TypeError{};
										}
									}
									{
										const Object *flag = get<Object *>(expr(args[3]));
										if(auto it = flag->properties.find("unordered"); it != flag->properties.end()) {
											is_unordered = true;
										}else if(auto it = flag->properties.find("reg"); it != flag->properties.end()) {
											reg_on = true;
											reg = it->second;
										}else {
											throw TypeError{};
										}
									}
								}else if(args.size() >= 3){
									const Object *flag = get<Object *>(expr(args[2]));
									if(auto it = flag->properties.find("unordered"); it != flag->properties.end()) {
										is_unordered = true;
									}else if(auto it = flag->properties.find("reg"); it != flag->properties.end()) {
										reg_on = true;
										reg = it->second;
									}else {
										throw TypeError{};
									}
								}
								if (reg_on) {
									//忽略unordered
									for (const auto &[key, ec]: obj) {
										if (key.size() == 0) continue;
										if (key[0] != '_' || [](string &&s){
											for (auto &c: s) if(c < '0' || c > '9') return true;
											return false;
										}(string(key.c_str()+1))) continue;

										auto res = expr("(" + visit(loadVisitor{}, value(fnc)) + ")(" + visit(loadVisitor{}, ec) + "," + 
											visit(loadVisitor{}, reg) +")");
										try {
											const auto &prop = get<Object *>(res)->properties;
											if(auto it = prop.find("for_result_spec");
												it != prop.end()){
												return it->second;
											}
										}catch(const exception &) {}
										reg = res;
									}
									return reg;
								} else {
									if (is_unordered) {

									}else for (const auto &[key, ec]: obj) {
										if (key.size() == 0) continue;
										if (key[0] != '_' || [](string &&s){
											for (auto &c: s) if(c < '0' || c > '9') return true;
											return false;
										}(string(key.c_str()+1))) continue;

										auto res = expr("(" + visit(loadVisitor{}, value(fnc)) + ")(" + visit(loadVisitor{}, ec) + ")");
										try {
											const auto &prop = get<Object *>(res)->properties;
											if(auto it = prop.find("for_result_spec");
												it != prop.end()){
												return it->second;
											}
										}catch(const exception &) {}
									}
									return Unit{};
								}
							}catch(const exception &){}

							throw TypeError{};
						}catch(const exception &){
							// throw TypeError{};
						}

						// esp array
						try{
							auto val = expr(args[0]);
							try {
								const auto &obj = get<Object *>(val)->properties;

								if (obj.find("item") == obj.end() || obj.find("begin") == obj.end() || obj.find("end") == obj.end()) throw TypeError{};
								//check

								auto fnc = get<Func>(expr(args[1]));
								bool is_unordered = false, reg_on = false;
								value reg = Unit{};
								if(args.size() >= 4) {
									{
										const Object *flag = get<Object *>(expr(args[2]));
										if(auto it = flag->properties.find("unordered"); it != flag->properties.end()) {
											is_unordered = true;
										}else if(auto it = flag->properties.find("reg"); it != flag->properties.end()) {
											reg_on = true;
											reg = it->second;
										}else {
											throw TypeError{};
										}
									}
									{
										const Object *flag = get<Object *>(expr(args[3]));
										if(auto it = flag->properties.find("unordered"); it != flag->properties.end()) {
											is_unordered = true;
										}else if(auto it = flag->properties.find("reg"); it != flag->properties.end()) {
											reg_on = true;
											reg = it->second;
										}else {
											throw TypeError{};
										}
									}
								}else if(args.size() >= 3){
									const Object *flag = get<Object *>(expr(args[2]));
									if(auto it = flag->properties.find("unordered"); it != flag->properties.end()) {
										is_unordered = true;
									}else if(auto it = flag->properties.find("reg"); it != flag->properties.end()) {
										reg_on = true;
										reg = it->second;
									}else {
										throw TypeError{};
									}
								}
								if (reg_on) {
									//忽略unordered
									long long i =
										get<long long>(expr("(" + (visit(loadVisitor{}, obj.find("begin")->second) +
										")(" + visit(loadVisitor{}, val) + ")"))),
										e =
										get<long long>(expr("(" + (visit(loadVisitor{}, obj.find("end")->second) +
										")(" + visit(loadVisitor{}, val) + ")")));
									for (;i < e;i++) {
										auto ec =
											expr("(" + (visit(loadVisitor{}, obj.find("item")->second) +
											")(" + to_string(i) + ")"));

										auto res = expr("(" + visit(loadVisitor{}, value(fnc)) + ")(" + visit(loadVisitor{}, ec) + "," + 
											visit(loadVisitor{}, reg) +")");
										try {
											const auto &prop = get<Object *>(res)->properties;
											if(auto it = prop.find("for_result_spec");
												it != prop.end()){
												return it->second;
											}
										}catch(const exception &) {}
										reg = res;
									}
									return reg;
								} else {
									if (is_unordered) {

									}else {
										long long i =
											get<long long>(expr("(" + (visit(loadVisitor{}, obj.find("begin")->second) +
											")(" + visit(loadVisitor{}, val) + ")"))),
										e =
											get<long long>(expr("(" + (visit(loadVisitor{}, obj.find("end")->second) +
											")(" + visit(loadVisitor{}, val) + ")")));
										for (;i < e;i++) {
											auto ec =
												expr("(" + (visit(loadVisitor{}, obj.find("item")->second) +
												")(" + to_string(i) + ")"));

											auto res = expr("(" + visit(loadVisitor{}, value(fnc)) + ")(" + visit(loadVisitor{}, ec) + ")");
											try {
												const auto &prop = get<Object *>(res)->properties;
												if(auto it = prop.find("for_result_spec");
													it != prop.end()){
													return it->second;
												}
											}catch(const exception &) {}
										}
									}
									return Unit{};
								}
							}catch(const exception &){}

							throw TypeError{};
						}catch(const exception &){
							// throw TypeError{};
						}
						// vector<string> object;
						// auto result = expr(args[0]);
						// auto obj = expr(args[1]);
						// auto list = get<Object *>(obj); //list->properties["begin"]

						// value begin = list->properties["begin"];
						// split(match(string(" ")+get<Func>(begin).body+" ",
						// 		" "+get<Func>(begin).arg+" ",
						// 		visit(loadVisitor{}, obj))
						// 	,object);
						// begin = expr(object);
						// object.clear();

						// value end = list->properties["end"];
						// split(match(string(" ")+get<Func>(end).body+" ",
						// 		" "+get<Func>(end).arg+" ",
						// 		visit(loadVisitor{}, obj))
						// 	,object);
						// end = expr(object);
						// object.clear();

						// auto func = expr(args[2]);


						// while(get<bool>(visit(notEqualVisitor{}, begin, end))) {
						// 	value item;
						// 	//calc
						// 	if(begin.index() == 0) {
						// 		item = get<long long>(begin);
						// 	}else if(begin.index() == 6) {
						// 		if(get<Object *>(begin)->properties.find("get") !=
						// 			get<Object *>(begin)->properties.end()){

						// 			auto getf = get<Object *>(begin)->properties["get"];
						// 			split(match(string(" ")+get<Func>(getf).body+" ",
						// 					" "+get<Func>(getf).arg+" ",
						// 					visit(loadVisitor{}, begin))
						// 				,object);
						// 			item = expr(object);
						// 			object.clear();
						// 		}
						// 	}else{
						// 		throw TypeError{};
						// 	}

						// 	split(match(string(" ")+get<Func>(func).body+" ",
						// 			" "+get<Func>(func).arg+" ",
						// 			visit(loadVisitor{},result))
						// 		,object);
						// 	result = expr(object);
						// 	object.clear();

						// 	split(match(string(" ")+get<Func>(result).body+" ",
						// 			" "+get<Func>(result).arg+" ",
						// 			visit(loadVisitor{},item))
						// 		,object);
						// 	result = expr(object);
						// 	object.clear();

						// 	//step
						// 	if(begin.index() == 0) {
						// 		begin = get<long long>(begin) + 1;
						// 	}else if(begin.index() == 6) {
						// 		if(get<Object *>(begin)->properties.find("next") !=
						// 			get<Object *>(begin)->properties.end()){
						// 			auto nextf = get<Object *>(begin)->properties["next"];
						// 			split(match(string(" ")+get<Func>(nextf).body+" ",
						// 					" "+get<Func>(nextf).arg+" ",
						// 					visit(loadVisitor{}, begin))
						// 				,object);
						// 			begin = expr(object);
						// 			object.clear();
						// 		}
						// 	}else{
						// 		throw TypeError{};
						// 	}
						// }
						// return cache[code] = result;
					}else if(part[1][0] == "match") {
						GetArgs
						auto val=visit(loadVisitor{}, expr(args[0]));
						vector<string> object;
						for(int index = 1; index < args.size(); index+=2) {
							const auto cond = get<Func>(expr(args[index]));
							split(match(string(" ")+cond.body+" ",
									" "+cond.arg+" ",
									val)
								,object);
							if(get<bool>(expr(object))) {
								return expr(args[index + 1]);
							}
						}
						throw MatchMiss{};
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
						const auto &tmp = expr(object);
						if (!get<Func>(func).is_volatile)
							cache[code] = tmp;
						return tmp;
					}

					bool is_volatile = false;
					for(auto post=0;post<args.size();post++){
						auto arg=visit(loadVisitor{},expr(args[post]));
						split(match(string(" ")+get<Func>(func).body+" ",
							" " + get<Func>(func).arg + " ",
							"( " + arg + " )"),
							object);
						is_volatile = is_volatile || get<Func>(func).is_volatile;
						func = expr(object);
					}
					if (!is_volatile)
						cache[code] = func;
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
						return cache[code] = obj->properties[v2];
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
						return cache[code] = v1;
					}
					v2=get<string>(tmp);
				} catch(const exception &){
					throw TypeError{};
				}

				if(visit(typeofVisitor{},v1) == v2) {
					return cache[code] = v1;
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
								return cache[code] = expr(object);
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
							return cache[code] = expr(object);
						}
						throw TypeError{};
					}
					v2=get<string>(tmp);
				} catch(const exception &){
					throw TypeError{};
				}

				if(types.find(v2) == types.end()) throw TypeError{};

				return cache[code] = visit(typeCastVisitor(v2), v1);
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
				if(nohap&&(ec.size() > 0)&&(ec[0] == '[')){
					a_s=ec;
					post++;
					nohap=0;
				}else{
					part[post].push_back(ec);
				}
			}
			if(!a_s.empty()) {
				part[0] = back_vec(part[0]);
				part[1] = back_vec(part[1]);
				if(part[1].empty()){
					a_s.erase(a_s.size()-1);
					a_s = a_s.c_str() + 1;
					GetArgs
					string builder = "{";
					for(int index = 0; index < args.size(); index++) {
						builder += "_" + to_string(index) + " : ";
						builder += visit(loadVisitor{}, expr(args[index]));
						builder += ", ";
					}
					builder += "}";
					return expr(builder);
				}
				Object *obj;
				int index;
				try {
					obj= get<Object *>(expr(part[1]));
					a_s.erase(a_s.size()-1);
					a_s = a_s.c_str() + 1;
					vector<string> object;
					split(a_s, object);
					index= get<long long>(expr(object));
				}catch(const exception &){
					throw TypeError{};
				}
				if(auto it = obj->properties.find("_" + to_string(index)); it != obj->properties.end()) {
					return it->second;
				}else {
					if(auto it = obj->properties.find("item"); it != obj->properties.end()) {
						return expr(visit(loadVisitor{}, it->second) + "(" + to_string(index) + ")");
					}
					throw BadMemory{};
				}
			}
		}
		part[0].clear();part[1].clear();part[2].clear();
		post=0;

		if(code.size()==1){
			if(code[0] == "true") return true;
			else if(code[0] == "false") return false;
			else if(code[0] == "unit") return Unit{};

			if(types.find(code[0]) != types.end()) return cache[code] = *types.find(code[0]);

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
				
				return cache[code] = obj;
			}else if(str){
				string temp=code[0].c_str()+1;
				temp.erase(temp.size()-1);
				return cache[code] = temp;
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
				return cache[code] = stoll(code[0]);
			}else if(onlypoint){
				return cache[code] = stold(code[0]);
			}else if(valtb.find(code[0])!=valtb.end()){
				return cache[code] = valtb[code[0]];
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
	value specCall(const std::string &part, const std::string &a_s) {
		using namespace std;
		GetArgs

		if(part == "@pow") {
			try {
				return pow(get<long double>(expr(args[0])), get<long double>(expr(args[1])));
			}catch(const exception &){
				throw TypeError{};
			}
		}else if(part == "@if_nan") {
			try {
				return isnan(get<long double>(expr(args[0])));
			}catch(const exception &){
				throw TypeError{};
			}
		}else if(part == "@get_nan") {
			try {
				return nan("");
			}catch(const exception &){
				throw TypeError{};
			}
		}else if(part == "@if_inf") {
			try {
				return isinf(get<long double>(expr(args[0])));
			}catch(const exception &){
				throw TypeError{};
			}
		}else if(part == "@get_inf") {
			try {
				return INFINITY;
			}catch(const exception &){
				throw TypeError{};
			}
		}else if(part == "@log") {
			try {
				return log(get<long double>(expr(args[0])));
			}catch(const exception &){
				throw TypeError{};
			}
		}else if(part == "@sin") {
			try {
				return sin(get<long double>(expr(args[0])));
			}catch(const exception &){
				throw TypeError{};
			}
		}else if(part == "@cos") {
			try {
				return cos(get<long double>(expr(args[0])));
			}catch(const exception &){
				throw TypeError{};
			}
		}else if(part == "@tan") {
			try {
				return tan(get<long double>(expr(args[0])));
			}catch(const exception &){
				throw TypeError{};
			}
		}else if(part == "@asin") {
			try {
				return asin(get<long double>(expr(args[0])));
			}catch(const exception &){
				throw TypeError{};
			}
		}else if(part == "@acos") {
			try {
				return acos(get<long double>(expr(args[0])));
			}catch(const exception &){
				throw TypeError{};
			}
		}else if(part == "@atan") {
			try {
				return atan(get<long double>(expr(args[0])));
			}catch(const exception &){
				throw TypeError{};
			}
		}else if(part == "@apply_app" || part == "@apply_flask_app") {
			try {
				const auto &obj = get<Object *>(expr(args[0]))->properties;
				const int port_num = int(get<long long>(expr(args[1])));
				const auto mem = "routes";
				if(auto it = obj.find(mem); it != obj.end()) {
					const auto &arr = get<Object *>(it->second)->properties;
					string flaskapp = "from flask import Flask\nimport subprocess\napp = Flask(__name__)\n";
					for(const auto &[_, pair]: arr) {
						const auto &route_pair = get<Object *>(pair)->properties;
						const string route_name = get<string>(route_pair.find("_0")->second);
						const string process_func = visit(loadVisitor{}, route_pair.find("_1")->second);
						string argstr, argstrs, argstrs_ = "{";
						for(const auto &ec: process_func) {
							if(ec == '-') {
								if(argstr == "_"){
									argstr = "";
									continue;
								}
								argstrs += argstr + ",";
								argstrs_ += argstr + "},{";
								argstr = "";
							}else if(ec == '>') {

							}else if((ec >= 'A' && ec <= 'Z') || (ec >= 'a' && ec <= 'z')){
								argstr += string(1, ec);
							}
						}
						argstrs = argstrs.substr(0, argstrs.size() - 1);
						argstrs_ = argstrs_.substr(0, argstrs_.size() - 2);

						if (argstrs_ == "{}")argstrs_ = {};

						const string route_str =
						"@app.route('" + route_name + "')\ndef route_" + to_string(rand()) + "(" + argstrs + "):\n" +
						" result = subprocess.run([\"./funi\", \"expr\", f'''(" + process_func + ")(\""+ argstrs_ + "\")'''], capture_output=True, text=True).stdout\n" +
						" return result\n";

						flaskapp += route_str;
					}
					flaskapp += "if __name__ == '__main__':\n app.run(debug=False, port=" + to_string(port_num) + ")\n";
					ofstream ofp("temp_flask_app.py");
					ofp << flaskapp;
					ofp.close();
					exit(system("python"
#ifndef _WIN32
						"3"
#endif
						" temp_flask_app.py"));
				} else {
					throw TypeError{};
				}
			} catch (const exception &) {
				throw;
				throw TypeError{};
			}
		}else if(part == "@open") {
			try {
				const auto file_name = get<string>(expr(args[0]));
				file_handle[file_name] = fstream(file_name);
				return file_name;
			}catch(const exception &) {
				throw TypeError{};
			}
		}else if(part == "@is_open") {
			try {
				const auto file_name = get<string>(expr(args[0]));
				return file_handle.find(file_name) != file_handle.end();
			}catch(const exception &) {
				throw TypeError{};
			}
		}else if(part == "@close") {
			try {
				const auto file_name = get<string>(expr(args[0]));
				if(const auto it = file_handle.find(file_name);it != file_handle.end()) {
					it->second.close();
					file_handle.erase(it);
				}
				return Unit{};
			}catch(const exception &) {
				throw TypeError{};
			}
		}else if (part == "@read_all") {
			try {
				const auto file_name = get<string>(expr(args[0]));
				auto it = file_handle.find(file_name);
				if (it == file_handle.end() || !it->second.is_open())
					throw runtime_error("file not open");
				auto &fs = it->second;
				fs.seekg(0, ios::end);
				streamsize size = fs.tellg();
				fs.seekg(0, ios::beg);
				string content(size, '\0');
				fs.read(content.data(), size);
				return content;
			} catch (const exception &) {
				throw TypeError{};
			}
		}else if (part == "@write") {
			try {
				const auto file_name = get<string>(expr(args[0]));
				const auto data = get<string>(expr(args[1]));
				auto it = file_handle.find(file_name);
				if (it == file_handle.end() || !it->second.is_open())
					throw runtime_error("file not open");

				auto &fs = it->second;
				// 关闭并重新打开（覆盖模式）
				fs.close();
				fs.open(file_name, ios::out | ios::trunc);  // 必须确保打开模式正确
				if (!fs) throw runtime_error("reopen failed");
				fs << data;   // 或 fs.write(data.c_str(), data.size());
				fs.flush();
				return Unit{};
			} catch (const exception &) {
				throw TypeError{};
			}
		}else if (part == "@append") {
			try {
				const auto file_name = get<string>(expr(args[0]));
				const auto data = get<string>(expr(args[1]));
				auto it = file_handle.find(file_name);
				if (it == file_handle.end() || !it->second.is_open())
					throw runtime_error("file not open");
				auto &fs = it->second;
				fs.seekp(0, ios::end);
				fs.write(data.c_str(), data.size());
				fs.flush();
				return Unit{};
			} catch (const exception &) {
				throw TypeError{};
			}
		}else if (part == "@read_line") {
			try {
				const auto file_name = get<string>(expr(args[0]));
				auto it = file_handle.find(file_name);
				if (it == file_handle.end() || !it->second.is_open())
					throw runtime_error("file not open");
				string line;
				if (getline(it->second, line))
					return line;
				else
					return string{};
			} catch (const exception &) {
				throw TypeError{};
			}
		}else if (part == "@tellg") {
			try {
				const auto file_name = get<string>(expr(args[0]));
				auto it = file_handle.find(file_name);
				if (it == file_handle.end() || !it->second.is_open())
					throw runtime_error("file not open");
				return static_cast<long long>(it->second.tellg());
			} catch (const exception &) {
				throw TypeError{};
			}
		}else if (part == "@seekg") {
			try {
				const auto file_name = get<string>(expr(args[0]));
				const auto pos = get<long long>(expr(args[1]));
				auto it = file_handle.find(file_name);
				if (it == file_handle.end() || !it->second.is_open())
					throw runtime_error("file not open");
				it->second.seekg(static_cast<streamoff>(pos));
				return Unit{};
			} catch (const exception &) {
				throw TypeError{};
			}
		}else if (part == "@eof") {
			try {
				const auto file_name = get<string>(expr(args[0]));
				auto it = file_handle.find(file_name);
				if (it == file_handle.end() || !it->second.is_open())
					throw runtime_error("file not open");
				return it->second.eof();
			} catch (const exception &) {
				throw TypeError{};
			}
		}else if (part == "@flush") {
			try {
				const auto file_name = get<string>(expr(args[0]));
				auto it = file_handle.find(file_name);
				if (it == file_handle.end() || !it->second.is_open())
					throw runtime_error("file not open");
				it->second.flush();
				return Unit{};
			} catch (const exception &) {
				throw TypeError{};
			}
		}else if (part == "@size") {
			try {
				const auto file_name = get<string>(expr(args[0]));
				auto it = file_handle.find(file_name);
				if (it == file_handle.end() || !it->second.is_open())
					throw runtime_error("file not open");
				auto &fs = it->second;
				auto cur = fs.tellg();
				fs.seekg(0, ios::end);
				auto size = fs.tellg();
				fs.seekg(cur);
				return static_cast<long long>(size);
			} catch (const exception &) {
				throw TypeError{};
			}
		}else if (part == "@get") {
			try {
				const auto file_name = get<string>(expr(args[0]));
				const auto data_type = get<string>(expr(args[1]));
				
				auto it = file_handle.find(file_name);
				if (it == file_handle.end() || !it->second.is_open())
					throw runtime_error("file not open");

				auto &fs = it->second;

				if (data_type == "Int") {
					long long val;
					fs >> val;
					if(fs.fail()) throw TypeError{};
					return val;
				} 
				else if (data_type == "Real") {
					double val;
					fs >> val;
					if(fs.fail()) throw TypeError{};
					return val;
				} 
				else if (data_type == "String") {
					string val;
					fs >> val;
					return val;
				} 
				else {
					throw TypeError{};
				}
			} catch (const exception &) {
				throw TypeError{};
			}
		}else if(part == "@put") {
			try {
				const auto file_name = get<string>(expr(args[0]));
				const auto data = visit(tos_visitor{},expr(args[1]));
				
				auto it = file_handle.find(file_name);
				if (it == file_handle.end() || !it->second.is_open())
					throw runtime_error("file not open");

				auto &fs = it->second;
				fs << data;
			} catch (const exception &) {
				throw TypeError{};
			}
		}else if(part == "@volatile") {
			try {
				auto func = get<Func>(expr(args[0]));
				func.is_volatile = true;
				return func;
			} catch (const exception &) {
				throw TypeError{};
			}
		}
		return Unit{};
	}
	value expr(const std::string &code) {
		std::vector<std::string> object;
		split(code, object);
		return expr(object);
	}
};

#endif
