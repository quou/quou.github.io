# Designing My Own Programming Language

*September 17, 2022*

For all of the time I have spent programming in C, I have pondered what I might change if I were
to re-design and re-implement it from scratch (as have most programmers at one point with their
favourite languages, I expect; This is partly how languages such as Rust or Go come into existence).
So, I have decided to create my own programming language, which is what I have been doing for the
past week. So far, there isn't very much to show; It can compile functions to LLVM IR that can
perform basic arithmetic. Here I will talk about what features the language will have and my
justifications for including them.

The plan is for this language to be very syntactically similar to C or C++, as that is what I am
used to programming with.

First, major differences from C:
 - The dereference when accessing the member of a pointer to a struct is inferred. I find the
`->` syntax that C and C++ utilise to be very ugly. While it is more explicit, there are no
ambigous cases that could be caused by inferring it.
 - Structs can have methods and operator overloads, implemented in a manner similar to C++.
They do not support private data or methods, or constructors and deconstructors.
 - Memory is allocated in a similar style to C, by obtaining a pointer from a `malloc` function,
which is then passed to a `free` function when it is no longer needed. However, a `new` operator,
similar to C++'s, is provided for more conveniently allocating structs. There is no `delete`
operator.
 - Generics, similar to C++ templates, are supported, but in a more restrictive and simple
manner. Generic structs work the same as they would in C++, but generic functions and methods
are not supported.
