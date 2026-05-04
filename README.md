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
no varient here, everything is pure(**unless u use `\_\_out` this debug function**)

### Object
```funi
obj = {
	a: 1,
	foo : self -> self.a
}

__out(obj)
__out(obj.foo(obj)) // 1
```

#### .. and as
```
ClassA = {
	a: 0,
	foo: unit, //unit means a virtrual function
	cast: unit
}
ClassB = {
	b: 0
}
ClassC = {
	toClassA: _ -> new(ClassA) //_ means self but it isn't used there so u should name it _
}
KidA = {
	a: 0,
	foo: self -> self.a,
	cast: objectB -> {
		a: objectB ..ClassB . a, // space is not nessary, but i introduce u add it
		foo: KidA.foo,
		cast: KidA.cast
	}
}
objectKidA = new(KidA) // default copy it unless KidA rewrite new
objectB = new(ClassB)
objectB as KidA // the same as objectKidA
objectB.. KidA //TypeError
```

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

### Read Docs
click [here](./docs/index.md)