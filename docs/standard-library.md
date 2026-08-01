# 标准库

funi 提供了一些内置的标准库，可以通过 `@()` 导入使用。

## 目录结构

```
lib/
- builtin_func.fi # default included
- functional.fi
- io.fi
- logic.fi
- loop.fi
- math.fi
- ranges.fi
- web.fi
```

### math.fi

`math.fi`是一个数学库，提供了基本的数学函数。
比如加减乘除、取余、绝对值、符号函数、负号等。

#### 模块概述
该模块封装了**数学常量**与**通用数学运算函数**，支持基础算术、数值处理、幂运算、三角函数、特殊值判断等核心数学能力，适用于数值计算场景。

---

#### 一、内置常量
| 常量名 | 含义                | 取值               |
|--------|---------------------|----------------------------|
| pi     | 圆周率              | 3.141592653589793          |
| e      | 自然常数（欧拉数）| 2.718281828459045          |
| nan    | 非数值（Not a Number） | nan |
| inf    | 无穷大              | +inf |

---

#### 二、核心函数

##### 1. 基础算术运算
| 函数名 | 原型          | 功能       | 参数说明       | 返回值       |
|--------|---------------|------------|----------------|--------------|
| add    | `a -> b -> a + b` | 加法运算   | a: 数值1；b: 数值2 | 两数之和     |
| sub    | `a -> b -> a - b` | 减法运算   | a: 被减数；b: 减数 | 两数之差     |
| mul    | `a -> b -> a * b` | 乘法运算   | a: 乘数1；b: 乘数2 | 两数之积     |
| div    | `a -> b -> a / b` | 除法运算   | a: 被除数；b: 除数 | 两数之商     |
| mod    | `a -> b -> a % b` | 取模运算   | a: 被除数；b: 除数 | 两数取模结果 |

##### 2. 数值极值/比较
| 函数名 | 原型                          | 功能       | 参数说明       | 返回值       |
|--------|-------------------------------|------------|----------------|--------------|
| min    | `a -> b -> (a if a < b else b)` | 取最小值   | a: 数值1；b: 数值2 | 较小的数值   |
| max    | `a -> b -> (a if a > b else b)` | 取最大值   | a: 数值1；b: 数值2 | 较大的数值   |

##### 3. 数值符号/处理
| 函数名 | 功能           | 参数说明 | 返回值                     |
|--------|----------------|----------|----------------------------|
| abs    | 求绝对值       | x: 数值  | 非负绝对值                 |
| sign   | 求符号位       | x: 数值  | 正数=1；负数=-1；零=0      |
| neg    | 数值取反       | x: 数值  | 原数的相反数               |

##### 4. 幂运算与开方
| 函数名 | 功能       | 参数说明               | 返回值       |
|--------|------------|------------------------|--------------|
| pow    | 幂运算     | base: 底数；exp: 指数  | 底数的指数幂 |
| sqrt   | 平方根运算 | k: 非负实数            | 数值的平方根 |

##### 5. 特殊值判断
| 函数名 | 功能           | 参数说明   | 返回值       |
|---------|----------------|------------|--------------|
| if_nan  |判断是否为NaN | k: 任意数值 | 布尔值（是/否） |
| if_inf  |判断是否为无穷大 | k: 任意数值 | 布尔值（是/否） |

##### 6. 对数与三角函数
| 函数名 | 功能       | 参数说明   | 返回值       |
|--------|------------|------------|--------------|
| log    | 自然对数   | k: 正实数  | 自然对数值   |
| sin    | 正弦函数   | k: 弧度值  | 正弦值       |
| cos    | 余弦函数   | k: 弧度值  | 余弦值       |
| tan    | 正切函数   | k: 弧度值  | 正切值       |
| asin   | 反正弦函数 | k: [-1,1]  | 弧度值       |
| acos   | 反余弦函数 | k: [-1,1]  | 弧度值       |
| atan   | 反正切函数 | k: 任意实数| 弧度值       |

### builtin_func.fi
一个封装文件, 因为内置函数不是一个callable, 所以做了这个封装

### `functional` 库文档

#### 概述

---

#### `compose`
从右向左组合两个函数。  
**参数**：`f` (函数), `g` (函数), `x` (任意值)  
**返回**：`f(g(x))`  
**示例**：
```
add1 = x -> x + 1
double = x -> x * 2
add1ThenDouble = functional.compose(double)(add1)
add1ThenDouble(3)  # 8
```

---

#### `pipe`
从左向右组合两个函数。  
**参数**：`f` (函数), `g` (函数), `x` (任意值)  
**返回**：`g(f(x))`  
**示例**：
```
add1 = x -> x + 1
double = x -> x * 2
add1ThenDouble = functional.pipe(add1)(double)
add1ThenDouble(3)  # 8
```

---

#### `flip`
交换二元函数的前两个参数顺序。  
**参数**：`f` (二元函数), `a`, `b`  
**返回**：`f(b, a)`  
**示例**：
```
sub = (x, y) -> x - y
flippedSub = functional.flip(sub)
flippedSub(5)(3)  # -2  (实际计算 sub(3,5))
```

---

#### `identity`
返回输入值本身。  
**参数**：`x`  
**返回**：`x`  
**示例**：
```
functional.identity(42)      # 42
functional.identity("hello") # "hello"
```

---

#### `const`
常量函数，忽略第二个参数，始终返回第一个参数。  
**参数**：`x`, `_` (任意值，被忽略)  
**返回**：`x`  
**示例**：
```
alwaysFive = functional.const(5)
alwaysFive("anything")  # 5
```

---

#### `apply`
将一个函数应用于一个值。  
**参数**：`f` (函数), `x` (任意值)  
**返回**：`f(x)`  
**示例**：
```
add1 = x -> x + 1
functional.apply(add1)(10)  # 11
```

### logic.fi
一些逻辑运算

### io.fi

`io.fi` 提供了文件输入输出功能，包括句柄管理、读取操作和写入操作。

#### 一、句柄管理

| 函数 | 原型 | 功能 | 参数说明 | 返回值 |
|------|------|------|----------|--------|
| open | `volatile(filename -> @open(filename))` | 打开文件 | filename: Str（文件路径） | 文件句柄 |
| close | `volatile(filename -> @close(filename))` | 关闭文件 | filename: Str（文件路径） | Unit |
| is_open | `volatile(filename -> @is_open(filename))` | 检查文件是否已打开 | filename: Str（文件路径） | Bool |

#### 二、读取操作

| 函数 | 原型 | 功能 | 参数说明 | 返回值 |
|------|------|------|----------|--------|
| read_all | `volatile(filename -> @read_all(filename))` | 读取文件全部内容 | filename: Str（文件路径） | Str |
| read_line | `volatile(filename -> @read_line(filename))` | 读取文件一行 | filename: Str（文件路径） | Str |
| get | `filename -> volatile(type_name -> @get(filename, type_name))` | 按类型读取值 | filename: Str；type_name: Str（类型名） | 指定类型值 |
| tellg | `filename -> volatile(@tellg(filename))` | 获取读取位置 | filename: Str | Int |
| seekg | `filename -> volatile(pos -> @seekg(filename, pos))` | 设置读取位置 | filename: Str；pos: Int | Unit |
| eof | `filename -> volatile(@eof(filename))` | 检查是否到达文件末尾 | filename: Str | Bool |
| size | `filename -> volatile(@size(filename))` | 获取文件大小 | filename: Str | Int |

#### 三、写入操作

| 函数 | 原型 | 功能 | 参数说明 | 返回值 |
|------|------|------|----------|--------|
| write | `filename -> data -> volatile(@write(filename, data))` | 写入数据到文件 | filename: Str；data: 任意 | Unit |
| append | `filename -> data -> volatile(@append(filename, data))` | 追加数据到文件 | filename: Str；data: 任意 | Unit |
| put | `filename -> value -> volatile(@put(filename, value))` | 按类型写入值 | filename: Str；value: 任意 | Unit |
| flush | `filename -> volatile(@flush(filename))` | 刷新文件缓冲区 | filename: Str | Unit |

#### 示例

```funi
@("io.fi")

io.write(io.open("output.txt"), "Hello, World!")

content = io.read_all("output.txt")
__out(content)

io.open("data.txt")
io.read_line("data.txt")
io.close("data.txt")
```

### loop.fi

`loop.fi` 提供了循环控制的辅助工具，用于配合 `for` 表达式实现累加、遍历等功能。

| 函数 | 原型 | 功能 | 参数说明 | 返回值 |
|------|------|------|----------|--------|
| unordered | `{unordered: unit}` | 无序遍历标记，for 循环不保证顺序 | 无 | Object |
| reg | `k -> {reg: k}` | 注册累加器初始值 | k: 初始值 | Object |
| result | `k -> {for_result_spec: k}` | 指定循环结果值 | k: 结果值 | Object |

#### 示例

```funi
@("loop.fi")

A = {0, 8, 10, 11}

for(A, item -> loop.result(item) ? item == 10 : __out(item))

for(A, item -> reg -> item + reg, loop.reg(0))

for(A, item -> reg -> item * reg, loop.reg(1))

N = {item: index -> index}
my_range = b -> e -> {begin: _ -> b, end: _ -> e}
for(N with my_range(2, 5), __out)
```

### web.fi

`web.fi` 提供了简单的 Web 服务器功能，支持路由注册和应用启动。

| 函数 | 原型 | 功能 | 参数说明 | 返回值 |
|------|------|------|----------|--------|
| apply_app | `routes_obj -> port -> @apply_app(routes_obj, port)` | 启动 Web 应用 | routes_obj: 路由对象；port: Int（端口号） | Unit |
| route | `path -> handler -> {_0: path, _1: handler}` | 创建单个路由条目 | path: Str（路径）；handler: 函数 | Object |

### ranges.fi

`ranges.fi` 提供了范围操作工具，用于为Array添加范围限制、转换和切片功能。

| 函数 | 原型 | 功能 | 参数说明 | 返回值 |
|------|------|------|----------|--------|
| take | `e -> {begin: _ -> 0, end: _ -> e}` | 创建从 0 开始到 `e` 结束的范围 | e: Int（结束值） | Object |
| trans | `ori -> proc -> {item: index -> proc(ori[index])}` | 对Array进行转换 | ori: 原始Array；proc: 转换函数 | Object |
| slice | `ori -> ib -> ie -> {item: index -> ori[index + ib], begin: _ -> 0, end: _ -> ie - ib}` | 对遍历器进行切片 | ori: 原始遍历器；ib: Int（起始偏移）；ie: Int（结束偏移） | Object |
| range | `b -> e -> {begin: _ -> b, end: _ -> e}` | 创建从 `b` 到 `e` 的范围 | b: Int（起始值）；e: Int（结束值） | Object |

#### 示例

```funi
@("ranges.fi")

N = {item: k -> k ..Int}

# 取前 5 个自然数
for(N with ranges.take(5), __out)
# stdout: 0, 1, 2, 3, 4

# 取 2 到 5 的范围
for(N with ranges.range(2,5), __out)
# stdout: 2, 3, 4

# 对元素进行转换
for(ranges.trans(N with ranges.take(3))(x -> x * 10), __out)
# stdout: 0, 10, 20

# 切片：从索引 2 开始取 3 个元素
for(ranges.slice(N)(2,5), __out)
# stdout: 2, 3, 4
```


## 创建自定义库

你可以创建自己的 `.fi` 文件作为库使用。

### 库文件结构

```funi
# my_lib.fi
my_lib = {
    greeting: "Hello from MyLib",
    
    sayHello: name -> __out("Hello, " + name),
    
    double: x -> x * 2
}
```

### 使用自定义库

```funi
@("my_lib.fi")

__out(my_lib.greeting)
my_lib.sayHello("funi")
__out(my_lib.double(5))
```

## 导入路径

`@()` 函数会在以下路径搜索文件：

1. `lib/` 目录
2. 添加的路径

### 添加自定义路径

> 调整`funi/funi_config.fi`中的`path`字段

funi_config.fi:
```funi
{
    path: {
        lib: "lib/",
        pwd: "",
        <说明>: <路径> // 在这里添加
    },
    init: _ -> @("builtin_func.fi") // 在启动时执行
}
```

## 库开发指南

### 命名规范

- 库文件名使用小写，用`_`分隔：`math_lib.fi`
- 命名空间使用 snake_case：`math_lib`
- 函数和属性使用 snake_case：`my_function`

### 最佳实践

1. 使用对象封装相关功能
2. 提供清晰的接口文档
3. 处理错误情况
4. 保持函数纯（无副作用）

### 示例库

```funi
# string_utils.fi
string_utils = {
    length: s -> 0,  # 占位符
    
    concat: a -> b -> a + b,
    
    toUpper: s -> s,  # 占位符
    toLower: s -> s   # 占位符
}
```

## 注意事项

1. 库文件中的全局变量会影响主程序的命名空间

## 未来计划

- 标准库扩展
- 模块化系统
- 包管理器支持