# funi

`funi` 是一种有趣的函数式编程语言。

## 为什么是 funi？

### 强类型

不同于 `js`，`funi` 拥有强类型，你永远不会被 `eq` 这类问题所困扰。

例如：
```funi
foo = k -> k .. Int + 1
foo2 = k -> k + 1
foo(1)   // 2
foo(1.2) // TypeError
foo2(1.2) // 2.2
```

### 强大的 RePL

~~大多数编程语言都有 repl，所以这其实无关紧要~~<br>
`funi` 的 Repl 提供的功能与文件运行器一样强大。

### 函数是一等公民

```funi
addInt = a1 -> a2 -> (a1 + a2) .. Int
addInt(5)     // a2 -> (5 + a2) .. Int
addInt(1, 6)  // 7
```

### 纯函数

这里没有变量，一切都是纯的（**除非你使用 `__out` 这个调试函数**）。

### 对象

```funi
obj = {
	a: 1,
	foo : self -> self.a
}

__out(obj)
__out(obj.foo(obj)) // 1
```

#### `..` 和 `as`

```
ClassA = {
	a: 0,
	foo: unit,      // unit 表示一个虚函数
	cast: unit
}
ClassB = {
	b: 0
}
ClassC = {
	toClassA: _ -> new(ClassA) // _ 表示 self，但此处未使用，因此应命名为 _
}
KidA = {
	a: 0,
	foo: self -> self.a,
	cast: objectB -> {
		a: objectB ..ClassB . a, // 空格不是必须的，但我建议你加上
		foo: KidA.foo,
		cast: KidA.cast
	}
}
objectKidA = new(KidA) // 默认复制，除非 KidA 重写了 new
objectB = new(ClassB)
objectB as KidA   // 与 objectKidA 相同
objectB.. KidA    // TypeError
```

## 现在开始使用它

### 克隆

```bash
git clone http://github.com/langhat/funi.git
```

### 编译

```bash
g++ src/main.cpp -o funi -std=c++20 -lpthread -lboost_system
```

### 运行 RePL 进行尝试

```bash
./funi repl
```

### 阅读文档

点击[这里](./docs/index.md)

### 致谢 & 声明

#### 贡献者
<a href="https://github.com/langhat/funi/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=langhat/funi" />
</a>

#### 使用的开源项目
[Crow](https://github.com/CrowCpp/Crow) - for routes