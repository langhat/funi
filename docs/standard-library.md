# 标准库

funi 提供了一些内置的标准库，可以通过 `@()` 导入使用。

## 目录结构

```
lib/
- math.fi
```

### math.fi

math.fi 是一个数学库，提供了基本的数学函数。
比如加减乘除、取余、绝对值、符号函数、负号等。

## 创建自定义库

你可以创建自己的 `.fi` 文件作为库使用。

### 库文件结构

```funi
# mylib.fi
MyLib = {
    greeting: "Hello from MyLib",
    
    sayHello: name -> __out("Hello, " + name),
    
    double: x -> x * 2
}
```

### 使用自定义库

```funi
@("mylib.fi")

__out(MyLib.greeting)
MyLib.sayHello("funi")
__out(MyLib.double(5))
```

## 导入路径

`@()` 函数会在以下路径搜索文件：

1. `lib/` 目录
2. 添加的路径

### 添加自定义路径

> none

## 库开发指南

### 命名规范

- 库文件名使用小写，用短横线分隔：`math-lib.fi`
- 命名空间使用 TitleCase：`MathLib`
- 函数和属性使用 snake_case：`my_function`

### 最佳实践

1. 使用对象封装相关功能
2. 提供清晰的接口文档
3. 处理错误情况
4. 保持函数纯（无副作用）

### 示例库

```funi
# string-utils.fi
StringUtils = {
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