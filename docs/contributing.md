# 文档贡献指南

欢迎为 funi 项目贡献！

## 文档结构

```
docs/
├── index.md           # 文档首页
├── getting-started.md # 入门指南
├── language-spec.md   # 语言规范
├── standard-library.md # 标准库文档
├── api-reference.md   # API 参考
├── examples.md        # 示例代码
├── regulation.md      # 惯例
└── contributing.md    # 贡献指南（本文件）
```

## 编写规范

### 格式

- 使用 Markdown 格式
- 使用 UTF-8 编码
- 每行不超过 80 字符

### 语言

- 中文和英文均可
- 保持语言简洁清晰
- 使用统一的术语

### 代码示例

- 使用 ```funi 标记代码块
- 提供运行结果示例
- 代码应可运行

### 目录结构

- 按功能模块组织
- 使用清晰的标题层级
- 提供目录导航

## 提交流程

### 1. Fork 仓库

```bash
git clone https://github.com/langhat/funi.git
cd funi
```

### 2. 创建分支

```bash
git checkout -b docs-update
```

### 3. 编写文档

编辑或创建文档文件。

### 4. 预览文档

确保 Markdown 格式正确。

### 5. 提交变更

```bash
git add docs/
git commit -m "Update documentation"
git push origin docs-update
```

### 6. 创建 PR

提交 Pull Request 到主仓库。

## 样式指南

### 标题

```markdown
# 一级标题
## 二级标题
### 三级标题
#### 四级标题
```

### 代码

```markdown
`inline code`

```funi
code block
```
```

### 列表

```markdown
- 无序列表项1
- 无序列表项2

1. 有序列表项1
2. 有序列表项2
```

### 表格

```markdown
| 列1 | 列2 |
|-----|-----|
| 值1 | 值2 |
```

### 链接

```markdown
[链接文本](链接地址)
[参考文档](./other-doc.md)
```

## 最佳实践

1. **保持更新**：文档应与代码同步更新
2. **清晰易懂**：避免过于技术化的术语
3. **提供示例**：代码示例帮助理解
4. **检查错误**：确保代码示例可运行
5. **版本说明**：注明功能的版本要求

## 需要帮助？

如有问题或建议，请提交 Issue。

## 许可证

文档使用与项目相同的许可证（GPL-3.0-only）。
