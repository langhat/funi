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

typedef std::variant<
    long long,  // Int
    bool,       // Bool
	long double,// Real
    std::string,// Str
    Func,       // Func
    //Map,
    //Set,
    //List,
	Unit,       // Unit
    Type,       // Type
> value;

class Unit {};

struct Func {
    std::vector<std::string> arg_type;
    std::vector<std::string> ret_type;
    bool pure;
    std::string arg,
        body;
};

enum BaseType{
    Int,
    Bool,
    Real,
    Str,
    Unit_t,
    Type_t
};

using Type = std::variant <
    std::pair<std::unique_ptr<Type>, std::unique_ptr<Type> > ,
    BaseType
>;

#endif
