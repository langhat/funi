#ifndef VISITOR_CPP
#define VISITOR_CPP

#include"value.cpp"

#include <type_traits> // std::is_invocable_v
#include <typeinfo>    // typeid
#include <iomanip>
#include <iostream>

class SyntaxError;

class TypeError:public SyntaxError{
public:
	const char *what()const noexcept{
		return "SyntaxError : Use a operator with wrong types";
	}
};

// class assignVisitor {
// 	std::map<std::string,value> *mp;
// public:
	
// 	assignVisitor(std::map<std::string,value> *p):mp(p){
		
// 	}
	
// 	template<typename T, typename T2>
// 	value operator()(const T &l, const T2 &r) const {
// 		if constexpr (std::is_same_v<std::decay_t<T>, Address>) {
// 			(*mp)[l.address]=r;
// 			return r;
// 		} else {
// 			throw TypeError{};
// 		}
// 	}
// };

class addVisitor {
public:
	value operator()(int l, int r) const {
		return l + r;
	}
	
	value operator()(double l, double r) const {
		return l + r;
	}
	
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
	value operator()(int l, int r) const {
		return l - r;
	}
	
	value operator()(double l, double r) const {
		return l - r;
	}
	
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
	value operator()(int l, int r) const {
		return l * r;
	}
	
	value operator()(double l, double r) const {
		return l * r;
	}
	
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
	value operator()(int l, int r) const {
		return l / r;
	}
	
	value operator()(double l, double r) const {
		return l / r;
	}
	
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
	value operator()(int l, int r) const {
		return l % r;
	}
	
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
	
	// ========== 容器/自定义类型处理（递归打印） ==========
	// 5. valueVec（vector<value> 包装类）
	void operator()(const valueVec& vec) const {
		const auto& raw_vec = (const std::vector<value>&)vec;
		std::cout << "[\n"; // 数组开头
		const auto child_visitor = get_child_visitor();
		
		// 遍历数组元素
		for (size_t i = 0; i < raw_vec.size(); ++i) {
			std::cout << child_visitor.get_indent(); // 子元素缩进
			std::visit(child_visitor, raw_vec[i]);   // 递归打印元素
			if (i != raw_vec.size() - 1) {
				std::cout << ","; // 非最后一个元素加逗号
			}
			std::cout << "\n";
		}
		
		std::cout << get_indent() << "]"; // 数组结尾（回退缩进）
	}
	
	// 6. valueMap（map<value, value> 包装类）
	void operator()(const valueMap& map) const {
		const auto& raw_map = (const std::map<value, value>&)(map);
		std::cout << "{\n"; // Map 开头
		const auto child_visitor = get_child_visitor();
		size_t count = 0;
		
		// 遍历键值对
		for (const auto& [key, val] : raw_map) {
			std::cout << child_visitor.get_indent();
			std::visit(child_visitor, key);   // 递归打印键
			std::cout << ": ";
			std::visit(child_visitor, val);   // 递归打印值
			if (++count != raw_map.size()) {
				std::cout << ",";
			}
			std::cout << "\n";
		}
		
		std::cout << get_indent() << "}"; // Map 结尾
	}
	
	// 7. valueObject（map<string, value> 包装类）
	void operator()(const valueObject& obj) const {
		const auto& raw_obj = (const std::map<std::string, value>&)(obj);
		std::cout << "{\n"; // 对象开头
		const auto child_visitor = get_child_visitor();
		size_t count = 0;
		
		// 遍历字符串键值对
		for (const auto& [key, val] : raw_obj) {
			std::cout << child_visitor.get_indent() << "\"" << key << "\": ";
			std::visit(child_visitor, val); // 递归打印值
			if (++count != raw_obj.size()) {
				std::cout << ",";
			}
			std::cout << "\n";
		}
		
		std::cout << get_indent() << "}"; // 对象结尾
	}
	
	// 8. Lambda（代码块）
	void operator()(const Lambda& lambda) const {
		const auto& code = (const std::string&)(lambda);
		std::cout << "Lambda: \"" << code << "\"";
	}
	
	// 9. Address（地址字符串）
	void operator()(const Address& addr) const {
		const auto& address = (const std::string&)(addr);
		std::cout << "Address: \"" << address << "\"";
	}
	
	// 10. Unit（空类型）
	void operator()(const Unit&) const {
		std::cout << "unit";
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
	
	// ========== 容器/自定义类型处理（返回格式化字符串） ==========
	// 5. valueVec：格式 vector(val1,val2,...)
	std::string operator()(const valueVec& vec) const {
		const auto& raw_vec = (const std::vector<value>&)vec;
		std::string result = "vector(";
		
		// 遍历数组元素，拼接逗号分隔的字符串
		for (size_t i = 0; i < raw_vec.size(); ++i) {
			result += std::visit(loadVisitor{}, raw_vec[i]);
			if (i != raw_vec.size() - 1) {
				result += ",";
			}
		}
		
		result += ")";
		return result;
	}
	
	// 6. valueMap：格式 map(key1,val1,key2,val2,...)
	std::string operator()(const valueMap& map) const {
		const auto& raw_map = (const std::map<value, value>&)(map);
		std::string result = "map(";
		size_t count = 0;
		
		// 遍历键值对，依次拼接 key,val
		for (const auto& [key, val] : raw_map) {
			result += std::visit(loadVisitor{}, key);   // 拼接键的字符串
			result += ",";
			result += std::visit(loadVisitor{}, val);   // 拼接值的字符串
			if (++count != raw_map.size()) {
				result += ",";
			}
		}
		
		result += ")";
		return result;
	}
	
	// 7. valueObject：格式 object(key1,val1,key2,val2,...)
	std::string operator()(const valueObject& obj) const {
		const auto& raw_obj = (const std::map<std::string, value>&)(obj);
		std::string result = "object(";
		size_t count = 0;
		
		// 遍历字符串键值对，key 不带引号
		for (const auto& [key, val] : raw_obj) {
			result += key; // key 直接拼接（无引号）
			result += ",";
			result += std::visit(loadVisitor{}, val);   // 拼接值的字符串
			if (++count != raw_obj.size()) {
				result += ",";
			}
		}
		
		result += ")";
		return result;
	}
	
	// 8. Lambda：格式 {Lambda值}
	std::string operator()(const Lambda& lambda) const {
		const auto& code = (const std::string&)(lambda);
		return "{" + code + "}";
	}
	
	// 9. Address：保持原有格式字符串
	std::string operator()(const Address& addr) const {
		const auto& address = (const std::string&)(addr);
		return "Address: \"" + address + "\"";
	}
	
	// 10. Unit（空类型）
	std::string operator()(const Unit&) const {
		return "unit";
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
	long long operator()(const valueVec&) const { throw TypeError(); }
	long long operator()(const valueMap&) const { throw TypeError(); }
	long long operator()(const valueObject&) const { throw TypeError(); }
	long long operator()(const Lambda&) const { throw TypeError(); }
	long long operator()(const Address&) const { throw TypeError(); }
	long long operator()(const Unit&) const { throw TypeError(); }
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
	bool operator()(const valueVec&) const { throw TypeError(); }
	bool operator()(const valueMap&) const { throw TypeError(); }
	bool operator()(const valueObject&) const { throw TypeError(); }
	bool operator()(const Lambda&) const { throw TypeError(); }
	bool operator()(const Address&) const { throw TypeError(); }
	bool operator()(const Unit&) const { throw TypeError(); }
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
	long double operator()(const valueVec&) const { throw TypeError(); }
	long double operator()(const valueMap&) const { throw TypeError(); }
	long double operator()(const valueObject&) const { throw TypeError(); }
	long double operator()(const Lambda&) const { throw TypeError(); }
	long double operator()(const Address&) const { throw TypeError(); }
	long double operator()(const Unit&) const { throw TypeError(); }
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
	std::string operator()(const valueVec&) const { throw TypeError(); }
	std::string operator()(const valueMap&) const { throw TypeError(); }
	std::string operator()(const valueObject&) const { throw TypeError(); }
	std::string operator()(const Lambda&) const { throw TypeError(); }
	std::string operator()(const Address&) const { throw TypeError(); }
	std::string operator()(const Unit&) const { throw TypeError(); }
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

#endif 
