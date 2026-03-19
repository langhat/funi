#ifndef VALUE_CPP
#define VALUE_CPP

#include<variant>
#include<string>
#include<vector>
#include<map>
#include<set>

class Unit;
class Func;

typedef std::variant<
    long long,  // Int
    bool,       // bool
	long double,// Real
    std::string,// Str
    Func,       // Func
    //Map,
    //Set,
    //List,
	Unit        // Unit
> value;

class Unit{};

struct Func{
    std::vector<std::string> arg_type;
    std::vector<std::string> ret_type;
    bool pure;
    std::string arg,
        body;
};

bool operator==(const value& lhs, const value& rhs) {
	if (lhs.index() != rhs.index()) {
		return false;
	}
	return std::visit([](const auto& l_val, const auto& r_val) {
		return l_val == r_val;
	}, lhs, rhs);
}

#endif
