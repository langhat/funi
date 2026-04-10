#ifndef VALUE_CPP
#define VALUE_CPP

#include<variant>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<memory>

class Unit;
class Func;
class Type;

using value = std::variant<
    long long,  // Int
    bool,       // Bool
	long double,// Real
    std::string,// Str
    Func,       // Func
    //Map,
    //Set,
    //List,
	Unit       // Unit
    // ,std::map<std::string, value*> // Object
>;

class Unit {};

struct Func {
    std::vector<std::string> arg_type;
    std::vector<std::string> ret_type;
    bool pure;
    std::string arg,
        body;
};

#endif
