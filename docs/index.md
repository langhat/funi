# funi 语言文档

欢迎来到 funi 编程语言的官方文档！

## 文档结构

- [入门指南](./getting-started.md) - 快速开始使用 funi
- [语言规范](./language-spec.md) - funi 的语法和语义
- [标准库](./standard-library.md) - 内置函数和库
- [API 参考](./api-reference.md) - 完整的 API 文档
- [示例代码](./examples.md) - 使用示例
- [约定俗成](./regulation.md) - 一些标准和规定

## 关于 funi

`funi` 是一种有趣的函数式编程语言，具有以下特点：

- **强类型** - 编译时类型检查
- **函数式** - 函数是一等公民
- **纯函数** - 无副作用（除非使用 `__out`）
- **面向对象** - 支持对象和继承

## 快速开始

```bash
# 编译
g++ src/main.cpp -o funi -std=c++20

# 运行 REPL
./funi repl

# 运行文件
./funi run your_file.fi
```

## 联系我们

如有问题或建议，请提交 Issue 或 PR。