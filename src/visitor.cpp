#ifndef VISITOR_CPP
#define VISITOR_CPP

#include "value.cpp"
#include "comparision.cpp"

#include <type_traits> // std::is_invocable_v
#include <typeinfo>    // typeid
#include <iomanip>
#include <iostream>

class addVisitor {
public:
	template<typename T, typename T2>
	value operator()(const T &l, const T2 &r) const {
		if constexpr (std::is_invocable_v<decltype(std::plus<>{}), const T&, const T2&>) {
			return l + r;
		} else {
			throw TypeError{};
		}
	}
};

class subVisitor {
public:
	template<typename T, typename T2>
	value operator()(const T &l, const T2 &r) const {
		if constexpr (std::is_invocable_v<decltype(std::minus<>{}), const T&, const T2&>) {
			return l - r;
		} else {
			throw TypeError{};
		}
	}
};

class mulVisitor {
public:
	template<typename T, typename T2>
	value operator()(const T &l, const T2 &r) const {
		if constexpr (std::is_invocable_v<decltype(std::multiplies<>{}), const T&, const T2&>) {
			return l * r;
		} else {
			throw TypeError{};
		}
	}
};

class divVisitor {
public:
	template<typename T, typename T2>
	value operator()(const T &l, const T2 &r) const {
		if constexpr (std::is_invocable_v<decltype(std::divides<>{}), const T&, const T2&>) {
			return l / r;
		} else {
			throw TypeError{};
		}
	}
};

class modVisitor {
public:
	template<typename T, typename T2>
	value operator()(const T &l, const T2 &r) const {
		if constexpr (std::is_invocable_v<decltype(std::modulus<>{}), const T&, const T2&>) {
			return l % r;
		} else {
			throw TypeError{};
		}
	}
};

class printVisitor {
private:
	int indent_level_;          // 当前缩进级别
	static constexpr int indent_step_ = 2; // 每次缩进的空格数
	
	// 辅助函数：生成缩进字符串
	std::string get_indent() const {
		return std::string(indent_level_ * indent_step_, ' ');
	}
	
	// 辅助函数：生成子级缩进的 Visitor（递归打印时用）
	printVisitor get_child_visitor() const {
		return printVisitor(indent_level_ + 1);
	}
	
public:
	// 构造函数：默认缩进级别为 0
	explicit printVisitor(int indent_level = 0) : indent_level_(indent_level) {}
	
	// ========== 基础类型处理 ==========
	// 1. long long
	void operator()(long long val) const {
		std::cout << val;
	}
	
	// 2. bool（打印 true/false 而非 1/0）
	void operator()(bool val) const {
		std::cout << std::boolalpha << val;
	}
	
	// 3. long double（可选：设置精度）
	void operator()(long double val) const {
		std::cout << std::fixed << std::setprecision(6) << val;
	}
	
	// 4. std::string
	void operator()(const std::string& val) const {
		std::cout<< val;
	}
	
	void operator()(const Func &val) const {
		std::cout << val.arg << " -> " << val.body;
	}

	// 10. Unit（空类型）
	void operator()(const Unit&) const {
		std::cout << "unit";
	}
	
	// 11. Object（对象类型）
	void operator()(const Object* obj) const {
		if (obj) {
			std::cout << "{";
			bool first = true;
			for (const auto& pair : obj->properties) {
				if (!first) std::cout << ", ";
				std::cout << pair.first << ": ";
				std::visit(get_child_visitor(), pair.second);
				first = false;
			}
			std::cout << "}";
		} else {
			std::cout << "null";
		}
	}
};

class loadVisitor {
public:
	// 构造函数：保持默认构造
	loadVisitor() = default;
	
	// ========== 基础类型处理（返回字符串） ==========
	// 1. long long
	std::string operator()(long long val) const {
		return std::to_string(val);
	}
	
	// 2. bool（返回 "true"/"false" 而非 "1"/"0"）
	std::string operator()(bool val) const {
		std::ostringstream oss;
		oss << std::boolalpha << val;
		return oss.str();
	}
	
	// 3. long double（保留 6 位小数的字符串）
	std::string operator()(long double val) const {
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(6) << val;
		return oss.str();
	}
	
	// 4. std::string（带引号）
	std::string operator()(const std::string& val) const {
		return "\"" + val + "\"";
	}
	
	std::string operator()(const Func& val) const {
		return "("+val.arg+" -> ( "+val.body+") )";
	}

	// 10. Unit（空类型）
	std::string operator()(const Unit&) const {
		return "unit";
	}
	
	// 11. Object（对象类型）
	std::string operator()(const Object* obj) const {
		if (obj) {
			std::string result = "{";
			bool first = true;
			for (const auto& pair : obj->properties) {
				if (!first) result += ", ";
				result += pair.first + ": " + std::visit(loadVisitor(), pair.second);
				first = false;
			}
			result += "}";
			return result;
		} else {
			return "null";
		}
	}
};

class typeofVisitor {
public:
	std::string operator()(long long val) const {
		return "Int";
	}
	std::string operator()(bool val) const {
		return "Bool";
	}
	
	std::string operator()(long double val) const {
		return "Real";
	}
	
	std::string operator()(const std::string& val) const {
		return "Str";
	}
	
	std::string operator()(const Func& val) const {
		return "Func";
	}

	std::string operator()(const Unit&) const {
		return "Unit";
	}
	
	std::string operator()(const Object*) const {
		return "Object";
	}
};

struct toi_visitor {
	// long long -> long long
	long long operator()(long long val) const {
		return val;
	}
	
	// bool -> long long（true=1，false=0）
	long long operator()(bool val) const {
		return val ? 1LL : 0LL;
	}
	
	// long double -> long long（截断小数部分）
	long long operator()(long double val) const {
		return static_cast<long long>(val);
	}
	
	// std::string -> long long（解析失败抛异常）
	long long operator()(const std::string& val) const {
		try {
			return std::stoll(val);
		} catch (const std::invalid_argument&) {
			throw TypeError();
		} catch (const std::out_of_range&) {
			throw TypeError();
		}
	}
	
	// 自定义类型（仅占位，抛TypeError，避免编译报错）
	long long operator()(const Func&) const { throw TypeError(); }
	long long operator()(const Unit&) const { throw TypeError(); }
	long long operator()(const Object*) const { throw TypeError(); }
};

// ====================== 2. tob_visitor：转换为 bool（仅基本类型） ======================
struct tob_visitor {
	// bool -> bool
	bool operator()(bool val) const {
		return val;
	}
	
	// long long -> bool（非0为true，0为false）
	bool operator()(long long val) const {
		return val != 0LL;
	}
	
	// long double -> bool（非0.0为true，0.0为false）
	bool operator()(long double val) const {
		return val != 0.0L;
	}
	
	// std::string -> bool（空字符串=false，非空=true；可选支持"true"/"false"解析）
	bool operator()(const std::string& val) const {
		std::string lower_val = val;
		for (char& c : lower_val) c = tolower(c);
		if (lower_val == "true") return true;
		if (lower_val == "false") return false;
		return !val.empty();
	}
	
	// 自定义类型（仅占位，抛TypeError，避免编译报错）
	bool operator()(const Func&) const { throw TypeError(); }
	bool operator()(const Unit&) const { throw TypeError(); }
	bool operator()(const Object*) const { throw TypeError(); }
};

// ====================== 3. tod_visitor：转换为 long double（仅基本类型） ======================
struct tod_visitor {
	// long double -> long double
	long double operator()(long double val) const {
		return val;
	}
	
	// long long -> long double
	long double operator()(long long val) const {
		return static_cast<long double>(val);
	}
	
	// bool -> long double（true=1.0L，false=0.0L）
	long double operator()(bool val) const {
		return val ? 1.0L : 0.0L;
	}
	
	// std::string -> long double（解析失败抛异常）
	long double operator()(const std::string& val) const {
		try {
			return std::stold(val);
		} catch (const std::invalid_argument&) {
			throw TypeError();
		} catch (const std::out_of_range&) {
			throw TypeError();
		}
	}
	
	// 自定义类型（仅占位，抛TypeError，避免编译报错）
	long double operator()(const Func&) const { throw TypeError(); }
	long double operator()(const Unit&) const { throw TypeError(); }
	long double operator()(const Object*) const { throw TypeError(); }
};

// ====================== 4. tos_visitor：转换为 std::string（仅基本类型） ======================
struct tos_visitor {
	// long long -> std::string
	std::string operator()(long long val) const {
		return std::to_string(val);
	}
	
	// bool -> std::string（返回"true"/"false"，更友好）
	std::string operator()(bool val) const {
		return val ? "true" : "false";
	}
	
	// long double -> std::string（保留精度，避免数据丢失）
	std::string operator()(long double val) const {
		std::ostringstream oss;
		oss.precision(6); // 设置浮点精度
		oss << val;
		return oss.str();
	}
	
	// std::string -> std::string
	std::string operator()(const std::string& val) const {
		return val;
	}
	
	// 自定义类型（仅占位，抛TypeError，避免编译报错）
	std::string operator()(const Func&) const { throw TypeError(); }
	std::string operator()(const Unit&) const { throw TypeError(); }
	std::string operator()(const Object*) const { throw TypeError(); }
};

// ====================== 便捷调用函数（简化使用） ======================
long long toi(const value& v) {
	return std::visit(toi_visitor(), v);
}

bool tob(const value& v) {
	return std::visit(tob_visitor(), v);
}

long double tod(const value& v) {
	return std::visit(tod_visitor(), v);
}

std::string tos(const value& v) {
	return std::visit(tos_visitor(), v);
}

struct typeCastVisitor {
    std::string targetType;

    explicit typeCastVisitor(std::string t) : targetType(std::move(t)) {}

    // Int -> 其他
    value operator()(long long v) const
    {
        if(targetType == "Int")    return v;
        if(targetType == "Real")   return (long double)v;
        if(targetType == "Bool")   return v != 0;
        if(targetType == "Str")    return std::to_string(v);
        throw TypeError{};
    }

    // Bool -> 其他
    value operator()(bool v) const
    {
        if(targetType == "Bool")   return v;
        if(targetType == "Int")    return v ? 1LL : 0LL;
        if(targetType == "Real")   return v ? 1.0L : 0.0L;
        if(targetType == "Str")    return v ? "true" : "false";
        throw TypeError{};
    }

    // Real -> 其他
    value operator()(long double v) const
    {
        if(targetType == "Real")   return v;
        if(targetType == "Int")    return (long long)v;
        if(targetType == "Bool")   return v != 0.0L;
        if(targetType == "Str")    return std::to_string(v);
        throw TypeError{};
    }

    // Str -> 其他
    value operator()(const std::string& v) const
    {
        if(targetType == "Str")    return v;
        if(targetType == "Int")    return std::stoll(v);
        if(targetType == "Real")   return std::stold(v);
        if(targetType == "Bool")
        {
            if(v == "true")  return true;
            if(v == "false") return false;
            throw TypeError{};
        }
        throw TypeError{};
    }

    // Func 禁止任何转换
    value operator()(const Func&) const
    {
        throw TypeError{};
    }

    // Unit 仅能转 Unit
	value operator()(const Unit&) const
	{
		if(targetType == "Unit") return Unit{};
		throw TypeError{};
	}
	
	// Object 禁止任何转换
	value operator()(const Object*) const
	{
		throw TypeError{};
	}
};

#endif 
