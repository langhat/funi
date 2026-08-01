# 约定俗成
一些进行funi开发时你需要注意的

## 项目结构

```
project/
- docs/
-- index.md
-- ...
- src/
-- main.fi
-- test.fi
-- ...
- .gitignore
- LICENSE
- README.md
- project.fi
```

### project.fi的编写

```funi
{
	name: "projectA",	// name
	version: 1145,	// unreadable version
	version_readable: "1.4.5",	// readable version
	main: "src/main.fi",	// main file
	depend: [	//depend list
		{
			name: "pacA",
			url: "url"
		}
	]
}
```

### src/main.fi的编写

eg.
```funi
@("a.fi")
@("math.fi")

main = a -> b -> a.foo(a ..Int, b ..Real)
```

### test file的编写

eg.
```funi
@("a.fi")

test(["test part 1",a.foo(5, 1.0) == a.foo(1, 5.0)])
```

### 模块的编写

eg.
```funi
// a.fi
a = {
	foo: a -> b -> a ..Int + b ..Real
}
```

## 关于`Func`

不要使用`__out`等有副作用的函数, 也不要写有副作用的函数!

### 参数
\(如果是一个模板函数请忽略\)
每个参数在第一次作为某个类型的变量使用时, 请使用`..`进行类型断言!

## 风格指南

本风格指南并不是强制性的，仅作为一份参考！

### 空白

> 采用Tab缩进
尽量在同一行打开大括号，除非需要换行
如果一个变量（常量）包含的元素数量超过`2`，请将每个元素放在单独的行上。

### 命名

简单来说，分别使用 `驼峰命名法`、`TitleCase`命名法、`蛇形命名法`。

> 类型声明使用 TitleCase 命名法（除非是一个 0 字段的 Object，此时它被视为一个命名空间，应使用`蛇形命名法`）
如果`x`是可以被调用的，并且它返回一个类型或在工厂模式中，那么使用`TitleCase`命名法
如果`x`是可被调用，并且返回非类型且非工厂模式，应使用`蛇形命名法`
其他情况下，应该使用`蛇形命名法`

文件名应使用 蛇形命名法，目录名称也应使用 蛇形命名法

以上的约束是在一般情况下，如果已经有了内部约定，请使用内部约定！

### 文档注释指南

> 根据名称省略冗余信息，即当可以立即从命名推断出其用途时，无需注释其用途