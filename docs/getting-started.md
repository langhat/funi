# 入门指南

## 安装

### 编译

funi 使用 C++20 编写，需要支持 C++20 的编译器：

```bash
g++ src/main.cpp -o funi -std=c++20
```

### 运行

编译成功后，会生成 `funi` 可执行文件。

## 运行模式

funi 支持多种运行模式：

### REPL 模式

交互式解释器模式：

```bash
./funi repl
```

进入后可以直接输入代码：

```funi
>>> 1 + 2
3
>>> add = a -> b -> a + b
>>> add(3, 5)
8
```

### 运行文件模式

运行 `.fi` 文件：

```bash
./funi run your_file.fi
```

### 表达式模式

直接计算表达式：

```bash
./funi expr "1 + 2 * 3"
```

### 访问模式

运行文件并计算表达式：

```bash
./funi visit your_file.fi "expression"
```

## 第一个程序

创建一个简单的 `hello.fi` 文件：

```funi
# 输出 Hello World
__out("Hello, World!")

# 定义一个函数
greet = name -> __out("Hello, " + name)

# 调用函数
greet("funi")
```

运行：

```bash
./funi run hello.fi
```

输出：

```
Hello, World!
Hello, funi
```

## 基本语法

### 变量声明

```funi
x = 42
name = "funi"
flag = true
```

### 函数定义

```funi
add = a -> b -> a + b
mul = a -> b -> a * b
```

### 对象

```funi
person = {
    name: "Alice",
    age: 30,
    greet: self -> __out("Hello, " + self.name)
}

person.greet(person)
```

### 条件表达式

```funi
max = a -> b -> if a > b -> a -> b
```

### 类型转换

```funi
x = 3.14 as Int  # 结果为 3
y = 5 .. Real    # TypeError, `..`代表类型断言
```

### 导入文件

```funi
@("math.fi")  # 导入 lib/math.fi
__out(Math.PI)
```

## 下一步

- 阅读 [语言规范](./language-spec.md) 了解完整语法
- 查看 [标准库](./standard-library.md) 了解可用的库函数
- 参考 [示例代码](./examples.md) 获取更多示例