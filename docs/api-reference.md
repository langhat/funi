# API 参考

## 内置函数

### __out

**语法**：`__out(value)`

**描述**：输出值到控制台（调试用）

**参数**：
- `value` - 任意类型的值

**返回值**：`Unit`

**示例**：

```funi
__out("Hello, World!")
__out(42)
__out({name: "Alice"})
```

### exit

**语法**：`exit()`

**描述**：立即退出程序

**参数**：无

**返回值**：无（程序终止）

**示例**：

```funi
exit()
```

### typeof

**语法**：`typeof(value)`

**描述**：获取值的类型名称

**参数**：
- `value` - 任意类型的值

**返回值**：`Str` - 类型名称

**示例**：

```funi
typeof(42)        # "Int"
typeof(3.14)      # "Real"
typeof("hello")   # "Str"
typeof(true)      # "Bool"
typeof(unit)      # "Unit"
```

### copy

**语法**：`copy(object)`

**描述**：复制对象

**参数**：
- `object` - `Object` 类型

**返回值**：`Object` - 复制后的对象

**示例**：

```funi
original = {value: 42}
copied = copy(original)
```

### new

**语法**：`new(object)`

**描述**：创建对象的新实例

**参数**：
- `object` - `Object` 类型

**返回值**：`Object` - 新实例

**示例**：

```funi
Class = {
    value: 0,
    new: _ -> {value: 42}
}
instance = new(Class)
```

### @

**语法**：`@("filename.fi")`

**描述**：导入库文件

**参数**：
- `filename` - `Str` 类型，文件名

**返回值**：`Unit`

**示例**：

```funi
@("math.fi")
@("array.fi")
```

### for

**语法**：`for(init, iterable, func)`

**描述**：循环遍历可迭代对象

**参数**：
- `init` - 初始值
- `iterable` - 可迭代对象（必须有 `begin` 和 `end` 属性）
- `func` - 回调函数

**返回值**：最后一次调用 func 的结果

**示例**：

```funi
arr = makeArray({1, 2, 3})
result = for(0, arr, i -> i)
```

## 运算符

### 算术运算符

| 运算符 | 描述 | 类型要求 |
|--------|------|----------|
| `+` | 加法 | 两个 `Int` 或两个 `Real` |
| `-` | 减法 | 两个 `Int` 或两个 `Real` |
| `*` | 乘法 | 两个 `Int` 或两个 `Real` |
| `/` | 除法 | 两个 `Int` 或两个 `Real` |
| `%` | 取模 | 两个 `Int` |

### 比较运算符

| 运算符 | 描述 | 类型要求 |
|--------|------|----------|
| `==` | 等于 | 相同类型 |
| `!=` | 不等于 | 相同类型 |
| `<` | 小于 | 两个 `Int` 或两个 `Real` |
| `>` | 大于 | 两个 `Int` 或两个 `Real` |
| `<=` | 小于等于 | 两个 `Int` 或两个 `Real` |
| `>=` | 大于等于 | 两个 `Int` 或两个 `Real` |

### 类型运算符

| 运算符 | 描述 | 示例 |
|--------|------|------|
| `..` | 类型断言 | `obj as Object` |
| `as` | 类型转换 | `3.14 .. Int` |

### 成员访问

| 运算符 | 描述 | 示例 |
|--------|------|------|
| `.` | 成员访问 | `obj.property` |

## 类型系统

### 基本类型

| 类型 | 描述 | 示例 |
|------|------|------|
| `Int` | 64位整数 | `42`, `0`, `-100` |
| `Real` | 64位浮点数 | `3.14`, `2.718` |
| `Bool` | 布尔值 | `true`, `false` |
| `Str` | 字符串 | `"hello"`, `'world'` |
| `Unit` | 空值 | `unit` |
| `Func` | 函数 | `a -> a + 1` |
| `Object` | 对象 | `{name: "Alice"}` |

### 类型检查

funi 是强类型语言，运行时会进行类型检查：

```funi
1 + 2      # ✓ 正确
1 + "2"    # ✗ TypeError
```

### 类型转换

```funi
3.14 as Int   # 结果: 3
5 as Real     # 结果: 5.0
```

## 对象系统

### 对象字面量

```funi
obj = {
    key1: value1,
    key2: value2,
    method: self -> self.key1
}
```

### 成员访问

```funi
obj.key1           # 获取属性
obj.method(obj)    # 调用方法
```

### 对象转换

通过 `cast` 属性实现类型转换：

```funi
Parent = {
    value: 0,
    cast: unit
}

Child = {
    value: 1,
    cast: p -> {value: p.value}
}

child = new(Child)
parent = child as Parent
```

## 函数系统

### 函数定义

```funi
add = a -> b -> a + b
square = x -> x * x
```

### 函数调用

```funi
add(2, 3)        # 5
square(5)        # 25
```

### 柯里化

funi 支持自动柯里化：

```funi
add = a -> b -> a + b
addOne = add(1)   # b -> 1 + b
addOne(5)         # 6
```

### 高阶函数

```funi
apply = f -> x -> f(x)
apply(square, 5)  # 25
```

## 错误类型

### TypeError

类型错误，当操作符收到不兼容的类型时抛出。

### NotExistError

不存在错误，当简易长矛被气球使用时抛出。

### SyntaxError

语法错误，当解析代码时遇到无效语法时抛出。

### WrongArgsNum

参数数量错误，当函数收到错误数量的参数时抛出。

### PackageNotFound

包未找到错误，当导入不存在的文件时抛出。

## 特殊变量

### self

约定俗成的方法第一个参数名称，代表当前对象。

### _

表示忽略的参数或未使用的变量。

## 保留关键字

| 关键字 | 用途 |
|--------|------|
| `if` | 条件表达式 |
| `else` | 条件表达式的 else 分支 |
| `unit` | Unit 类型字面量 |
| `true` | 布尔真值 |
| `false` | 布尔假值 |

## 注释

### 单行注释

```funi
# 这是单行注释
x = 42  # 这也是注释
```

### 块注释

当前版本不支持块注释。

## 命名规范

按照 zig style 来

## 代码风格建议

按照 zig style 来:

1. 使用空格分隔运算符
2. 在 `->` 前后使用空格
3. 多行对象字面量每行一个属性
4. 使用注释说明复杂逻辑

## 性能注意事项

1. 对象复制是浅拷贝
2. 递归调用可能导致栈溢出
3. 大量计算应使用高效算法

## 兼容性

当前版本的 funi 语言仍在开发中，API 可能会有变化。