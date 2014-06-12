

#4/10: Customization: optimizing compiler technology for SELF, a dynamically-typed object-oriented programming language

##Summary
This paper presents the optimizations supported by the compiler of the Object Oriented language SELF. It mainly focuses on the optimization of the "message passing". Specifically, the supported optimizations are:

- Customized Compilation
- Message Splitting &
- Type prediction

However it also support more tranditional optimizations such as: constant folding, dead code elimination, type & data flow analysis, common subexpression elimination, global register allocation and instruction scheduling thus resulting in a better performance than other OO languages, such as Smalltalk.

##Limitations
The main limitation of the compiler is that it **doesn't support method arguments**. If it gets extended so that they would get supported it would lead to a better generated code. Other limitations are:

- That the message inliner doesn't make the optimal decision about whether to inline a method or not &
- That the presented implementation of the compiler does not perform well enough for it to be integrated to an interactive programming environment.

##Future Work
 
The goal for the future is to perform optimizations on the combined type analyzer and extended message splitter that would enable the compiler to split off entire sections of the control flow graph that correspond to the most common data types. This will lead to maximally-inlined code with no run-time checks.

