#pragma once

#include"value.cpp"
#include"spliter.cpp"

class TypeError:public SyntaxError{
public:
	const char *what()const noexcept{
		return "SyntaxError : Use a operator with wrong types";
	}
};

class greaterVisitor {
public:
	template<typename T, typename T2>
	value operator()(const T &l, const T2 &r) const {
		if constexpr (std::is_invocable_v<decltype(std::greater<>{}), const T&, const T2&>) {
			return l > r;
		} else {
			throw TypeError{};
		}
	}
};

class greaterEqualVisitor {
public:
	template<typename T, typename T2>
	value operator()(const T &l, const T2 &r) const {
		if constexpr (std::is_invocable_v<decltype(std::greater_equal<>{}), const T&, const T2&>) {
			return l >= r;
		} else {
			throw TypeError{};
		}
	}
};

class lessVisitor {
public:
	template<typename T, typename T2>
	value operator()(const T &l, const T2 &r) const {
		if constexpr (std::is_invocable_v<decltype(std::less<>{}), const T&, const T2&>) {
			return l < r;
		} else {
			throw TypeError{};
		}
	}
};

class lessEqualVisitor {
public:
	template<typename T, typename T2>
	value operator()(const T &l, const T2 &r) const {
		if constexpr (std::is_invocable_v<decltype(std::less_equal<>{}), const T&, const T2&>) {
			return l <= r;
		} else {
			throw TypeError{};
		}
	}
};

class equalVisitor {
public:
	template<typename T, typename T2>
	value operator()(const T &l, const T2 &r) const {
		if constexpr (std::is_invocable_v<decltype(std::equal_to<>{}), const T&, const T2&>) {
			return l == r;
		} else {
			throw TypeError{};
		}
	}
};

class notEqualVisitor {
public:
	template<typename T, typename T2>
	value operator()(const T &l, const T2 &r) const {
		if constexpr (std::is_invocable_v<decltype(std::not_equal_to<>{}), const T&, const T2&>) {
			return l != r;
		} else {
			throw TypeError{};
		}
	}
};