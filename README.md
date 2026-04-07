# funi
`funi` is a funny functional pl

## why is funi?

### Strong Types
not like `js`, `funi` has strong types, you will never be annoied by `eq`

eg.
```funi
foo = k -> k .. Int + 1
foo2 = k -> k + 1
foo(1) // 2
foo(1.2) // TypeError
foo2(1.2) //2.2
```

### Strong RePL
~~most of pl has a repl, so it even no matter~~<br>
`funi` \'s Repl provide functions\(means '功能' here\) as strong as file runner

### Functions as First
```funi
addInt = a1 -> a2 -> (a1 + a2) .. Int
addInt(5) // a2 -> (5 + a2) .. Int
addInt(1, 6) // 7
```

### Pure
no varient here, everything is pure(*unless u use \_\_out this debug function*)

## Use It from Now on

### Clone
```bash
git clone http://github.com/langhat/funi.git
```

### Complie
```bash
g++ src/main.cpp -o funi -std=c++20
```

### Run RePL for Try
```bash
./funi repl
```