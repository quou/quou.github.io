# Quou's C

## Links
 - [Git repository](https://codeberg.org/quou/qc)

## About

This is a custom programming language that I'm working on. It's a compiled, statically
typed, C-like language that uses LLVM to compile to native code. It's in the early stages
and very work in progress, but right now it has the following features:
 - Integer maths for 8, 16, 32 and 64 bit integers.
 - Floating point maths for 32 and 64 bit floating point types.
 - Booleans and related operators like && and ||, as well as comparison operators like < and ==.
 - Pointers, pointer arithmetic, referencing and dereferencing.
 - If statements.
 - A Rust-style infinite loop, while and for loops, with break and continue statements.
 - Procedures and C-style prototypes.
 - String literals.
 - Compile-time type checking.
 - Casting.
 - Procedures with external linkage.
 - Name spaces.
 - Fancy error messages that point to the erroneous token.

Currently, it doesn't generate its own entry point and relies on linking to a C program
to provide a main function. That will change soon, but I'm working on getting the basic
features working, first. The compiler is written in C++, but I would really, really like
to get this language to the point where it is able to self-host.

I have plans to add these features:
 - Write unit tests for all of the current features.
 - Structs with support for methods.
 - Generic structs, constructs and deconstructors and operator overloading.
 - A basic standard library including things like IO, file system and data structures like
   vectors, hash tables and strings.
 - Implicit casts for integers and floats.
 - Type aliases.
 - Switch statements.
 - Bitwise operators and increment & decrement operators.
 - Integer literal suffixes to specify the size of an integer literal.
 - Module system.
 - Windows support.
 - Create bindings for some of my C libraries (like [Corrosion](corrosion.html), maybe), just for fun.

The syntax is very C-like, but with some minor differences, including:
 - The parentheses on statements such as if, for and while are optional.
 - Casting is done more similarly to GLSL, like this: `int(my_val)`, rather
   than C which does: `(int)my_val`.
 - `int` represents a 64-bit value rather than a 32-bit one, as does `float`.
   32-bit versions of these values can be obtained with `i32` and `f32`. This is
   similar to Odin on a 64 bit system.
 - `elif` is used in place of `else if`.

The errors print in a style similar to that of GCC or Clang, where it points to the token that
caused the error:

![Screenshot](media/error01.png)
![Screenshot](media/error02.png)

Here is what Fizz Buzz looks like in my language:

```
extern i32 puts(char* s);
extern void print_int(int v);

void fizz_buzz(int n) {
	for int i = 1; i < n; i = i + 1 {
		if i % 3 == 0 {
			if i % 5 == 0 {
				puts("FizzBuzz");
			} else {
				puts("Fizz");
			}
		} elif i % 5 == 0 {
			puts("Buzz");
		} else {
			print_int(i);
		}
	}
}
```

The above program relies on the C standard library function `puts`, as well as a
C function I made that simply uses `printf` to print an integer.

I started writing this language by following the [LLVM Tutorial](https://llvm.org/docs/tutorial/).
I quickly branched away from this tutorial, however, as I found it fairly easy to add my own
features such as type checking once I had a base for parsing and code generation to work off of.
One of the biggest challenges I encountered was the fact that LLVM doesn't allow terminators
(such as `ret` or `br`) to be in the middle of a block, so I had to write systems to detect
when a return instruction is generated so that it stops generating code at that point to avoid
errors from LLVM. I need to write unit tests for everything to ensure it's all correct, though,
which I have started doing but that still needs a lot of work.
