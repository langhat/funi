# 语言规范

## 概述

funi 是一种强类型、函数式、纯编程语言。

## 基本类型

### Int（整数）

64位有符号整数：

```funi
x = 42
y = 0
z = 1000000000
```

### Real（实数）

64位浮点数：

```funi
pi = 3.14159
e = 2.71828
```

### Bool（布尔值）

```funi
flag = true
result = false
```

### Str（字符串）

```funi
name = "funi"
greeting = 'Hello'
```

### Unit（单元）

表示无返回值：

```funi
void_func = _ -> unit
```

### Func（函数）

函数类型：

```funi
add = a -> b -> a + b
```

### Object（对象）

对象类型：

```funi
person = {
    name: "Alice",
    age: 30
}
```

### Array（离散化数组）
本质上是一个Object

```funi
a = [0, 3, 6, 7]
N = { // 自然数
    item: k -> k ..Int
}
R = { // 实数
    item: k -> k ..Real
}
b = {
    _0: 9,
    _9: 1,
    _6: 7,
    _7: 8,
}
a[2] // 6
N[123456789] // 123456789
R[math.pi] // 3.141592653589793
b[9] // 1
```

## 表达式

### 算术表达式

支持的运算符：

| 运算符 | 描述 | 示例 |
|--------|------|------|
| `+` | 加法 | `1 + 2` |
| `-` | 减法 | `5 - 3` |
| `*` | 乘法 | `4 * 5` |
| `/` | 除法 | `10 / 2` |
| `%` | 取模 | `7 % 3` |

### 比较表达式

| 运算符 | 描述 | 示例 |
|--------|------|------|
| `==` | 等于 | `x == 5` |
| `!=` | 不等于 | `x != 0` |
| `<` | 小于 | `x < 10` |
| `>` | 大于 | `x > 0` |
| `<=` | 小于等于 | `x <= 100` |
| `>=` | 大于等于 | `x >= 0` |

### 条件表达式

```funi
true_expr if condition else false_expr
```

### 函数表达式

```funi
param -> body
```

示例：

```funi
add = a -> b -> a + b
square = x -> x * x
```

### 对象表达式

```funi
{
    key1: value1,
    key2: value2,
    ...
}
```

示例：

```funi
point = {
    x: 10,
    y: 20,
    distance: self -> self.x * self.x + self.y * self.y
}
```

#### 对象规范
1. 对象的属性和方法是 public 的。
2. 对象的属性和方法可以被其他对象调用。
3. 静态方法可以被调用，不需要实例化对象。第一个参数不能是self。
4. 动态方法需要实例化对象，第一个参数必须是self。
5. 抽象方法应该写为unit类型，表示无。
6. 对象语法也可以用于写一个命名空间，不一定是一个对象。

### 成员访问

```funi
object.member
```

示例：

```funi
point.x          // 获取属性
point.distance(point)  // 调用方法
```

### 类型转换 / 断言

```funi
expression as Type
expression .. Type // 如果不符的话TypeError
```

示例：

```funi
x = 3.14 as Int   // 3
y = 5 as Real     // 5.0
```

## 语句

### 变量赋值

```funi
name = value
```

示例：

```funi
x = 42
message = "Hello"
```

### 函数调用

```funi
function(arg1, arg2, ...)
```

示例：

```funi
add(2, 3)
__out("Hello")
```

注意:
> 函数调用是一个语法糖, f(x, y) = f(x)(y), 但我们推荐使用语法糖。
同时另一个语法糖是f() = f(unit),
这里要注意, f规范上是参数名为_的

### 导入语句

```funi
@("filename.fi")
```

示例：

```funi
@("math.fi")
@("array.fi")
```

## 内置函数

### __out

输出函数（调试用）：

```funi
__out(value)
```

### exit

退出程序：

```funi
exit()
```

### typeof

获取类型：

```funi
typeof(value)
```

### copy / new

复制对象：

```funi
copy(object)
new(object)
```

### for

循环：

```funi
for(init, iterable, func)
```

## 类型系统

### 强类型

funi 是强类型语言，类型错误会在运行时抛出异常。

### 类型检查

运算符要求操作数类型匹配：

```funi
1 + 2      # 正确
1 + "2"    # TypeError
```

### 类型转换 / 类型断言

```funi
# 3.14 .. Int   # TypeError, 3.14 is a Real
3.14 as Int # (cast) 3
3 + 2.5 # 5.5
```

## 错误处理

运行时错误会抛出异常并显示错误信息：

```funi
try {
    # 可能出错的代码
} catch (TypeError) {
    # 处理类型错误
}
```

> 注意：当前版本的异常处理语法可能有限，请参考最新实现。