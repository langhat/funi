# 示例代码

这里提供了 funi 编程语言的各种使用示例。

## 基础示例

### 1. Hello World

```funi
__out("Hello, World!")
```

运行输出：
```
Hello, World!
```

### 2. 基本算术

```funi
# 加法
result = 1 + 2
__out(result)  # 3

# 乘法
result = 4 * 5
__out(result)  # 20

# 除法
result = 10 / 2
__out(result)  # 5

# 取模
result = 7 % 3
__out(result)  # 1
```

### 3. 变量声明

```funi
name = "funi"
age = 3
pi = 3.14159
is_active = true

__out(name)
__out(age)
__out(pi)
__out(is_active)
```

### 4. 函数定义

```funi
# 简单函数
add = a -> b -> a + b
__out(add(2, 3))  # 5

# 柯里化
add5 = add(5)
__out(add5(3))    # 8

# 高阶函数
apply = f -> x -> f(x)
square = x -> x * x
__out(apply(square, 5))  # 25
```

## 条件表达式

### 5. 简单条件

```funi
max = a -> b -> a if a > b else b
__out(max(3, 5))  # 5

min = a -> b -> a if a < b else b
__out(min(3, 5))  # 3

abs = x -> 0 - x if x < 0 else x
__out(abs(0 - 5)) # 5
__out(abs(3))     # 3
```

### 6. 嵌套条件

```funi
sign = x -> 1 if x > 0 else 0 - 1 if x < 0 else 0
__out(sign(5))    # 1
__out(sign(0 - 5))# -1
__out(sign(0))    # 0
```

## 对象系统

### 7. 简单对象

```funi
person = {
    name: "Alice",
    age: 30,
    greet: self -> __out("Hello, " + self.name)
}

__out(person.name)      # Alice
person.greet(person)    # Hello, Alice
```

### 8. 对象复制

```funi
# 略
```

### 9. 类型转换（cast）

```funi
Animal = {
    name: "unknown",
    speak: unit,
    cast: a -> {
        name: a.name,
        speak: a.speak,
        cast: Animal.cast
    }
}

Dog = {
    name: "Dog",
    speak: self -> __out("Woof!"),
}

myDog = new(Dog)
myDog.speak(myDog)  # Woof!

# 类型转换
animal = myDog as Animal
__out(animal.name)  # Dog
```

## 综合示例

### 10. 阶乘函数

```funi
factorial = n -> 1 if n == 0 else n * factorial(n - 1)

__out(factorial(5))  # 120
__out(factorial(10)) # 3628800
```

### 11. Fibonacci 数列

```funi
fib = n -> n if n <= 1 else fib(n - 1) + fib(n - 2)

__out(fib(0))  # 0
__out(fib(1))  # 1
__out(fib(5))  # 5
__out(fib(10)) # 55
```

### 15. 温度转换

```funi
CelsiusToFahrenheit = c -> c * 9 / 5 + 32
FahrenheitToCelsius = f -> (f - 32) * 5 / 9

__out(CelsiusToFahrenheit(0))   # 32
__out(CelsiusToFahrenheit(100)) # 212
__out(FahrenheitToCelsius(32))  # 0
__out(FahrenheitToCelsius(212)) # 100
```

### 16. 矩形面积计算

```funi
Rectangle = {
    width: 0,
    height: 0,
    area: self -> self.width * self.height,
    perimeter: self -> 2 * (self.width + self.height)
}

rect = copy(Rectangle)
rect.width = 10
rect.height = 5

__out(rect.area(rect))      # 50
__out(rect.perimeter(rect)) # 30
```

### 17. 学生成绩管理

```funi
Student = {
    name: "",
    scores: {math: 0, english: 0, science: 0},
    average: self -> (self.scores.math + self.scores.english + self.scores.science) / 3
}

alice = copy(Student)
alice.name = "Alice"
alice.scores.math = 95
alice.scores.english = 88
alice.scores.science = 92

__out(alice.name)          # Alice
__out(alice.average(alice))# 91.666667
```

### 18. 简单计算器

```funi
Calculator = {
    add: a -> b -> a + b,
    subtract: a -> b -> a - b,
    multiply: a -> b -> a * b,
    divide: a -> b -> a / b,
    power: base -> exp ->
        1 if exp == 0 else base * Calculator.power(base, exp - 1)
}

calc = copy(Calculator)
__out(calc.add(5, 3))      # 8
__out(calc.multiply(4, 6)) # 24
__out(calc.power(2, 8))    # 256
```

## 导入和模块化

### 19. 创建和使用自定义库

```funi
# utils.fi
Utils = {
    isEven: n -> n % 2 == 0,
    isOdd: n -> n % 2 != 0,
    sum: a -> b -> a + b
}
```

```funi
# main.fi
@("utils.fi")

__out(Utils.isEven(4)) # true
__out(Utils.isOdd(5))  # true
__out(Utils.sum(2, 3)) # 5
```

## 调试技巧

### 20. 使用 __out 调试

```funi
debug = msg -> value -> {
    __out("DEBUG: " + msg + " = ")
    __out(value)
    value
}

result = debug("intermediate")(5 + 3)
__out("Final: " + result)
```

## 最佳实践

### 21. 纯函数

```funi
# 纯函数（推荐）
add = a -> b -> a + b

# 避免副作用
# 不推荐：修改全局状态
total = 0
addToTotal = x -> total = total + x
```

### 22. 函数组合

```funi
compose = f -> g -> x -> f(g(x))

double = x -> x * 2
increment = x -> x + 1

doubleThenIncrement = compose(increment)(double)
__out(doubleThenIncrement(5))  # 11
```

### 23. 错误处理

```funi
safeDivide = a -> b -> __out("Division by zero") if b == 0 else a / b

__out(safeDivide(10, 2)) # 5
safeDivide(10, 0)        # Division by zero
```

## 性能优化

### 24. 尾递归优化

```funi
# 尾递归版本
factorialTail = n -> acc ->
    n if n == 0 else factorialTail(n - 1)(n * acc)

factorial = n -> factorialTail(n)(1)
__out(factorial(5))  # 120
```

## 常见模式

### 25. 单例模式

```funi
Singleton = {
    instance: unit,
    getInstance: _ ->
        {value: 42} if Singleton.instance == unit else Singleton.instance
}

inst1 = Singleton.getInstance(Singleton)
inst2 = Singleton.getInstance(Singleton)
# inst1 和 inst2 是同一个对象
```

### 26. 工厂模式

```funi
ShapeFactory = {
    createCircle: radius -> {
        type: "circle",
        radius: radius,
        area: self -> 3.14159 * self.radius * self.radius
    },
    createRectangle: width -> height -> {
        type: "rectangle",
        width: width,
        height: height,
        area: self -> self.width * self.height
    }
}

circle = ShapeFactory.createCircle(5)
rect = ShapeFactory.createRectangle(10, 5)

__out(circle.area(circle)) # 78.53975
__out(rect.area(rect))     # 50
```

## 测试示例

### 27. 简单测试框架

```funi
Test = {
    assertEqual: expected -> actual ->
        __out("PASS") if expected == actual else __out("FAIL"),
    assertNotEqual: expected -> actual ->
        __out("PASS") if expected != actual else __out("FAIL"),
    assertTrue: condition ->
        __out("PASS") if condition else __out("FAIL"),
    assertFalse: condition ->
        __out("PASS") if !condition else __out("FAIL")
}

# 测试
Test.assertEqual(Test)(2)(1 + 1)
Test.assertTrue(Test)(1 == 1)
Test.assertFalse(Test)(1 == 2)
```

## 注意事项

1. **类型安全**：funi 是强类型语言，确保操作数类型匹配
2. **纯函数**：尽量使用纯函数，避免副作用
3. **递归深度**：注意递归调用的深度，可能导致栈溢出
4. **性能**：对于大量数据处理，考虑使用高效算法

## 更多资源

- [语言规范](./language-spec.md)
- [标准库](./standard-library.md)
- [API 参考](./api-reference.md)