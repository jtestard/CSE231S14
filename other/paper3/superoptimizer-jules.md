### Summary of the paper

Given an instruction set and a small input program written using the instruction set, the super optimizer finds the shortest (with smallest number of assembly instructions) equivalent program using an exhaustive search approach in which each generated candidate program is checked for equivalence with the input program using first a very efficient probability-based assertion technique followed by (if warranted) a slower code inspection. 

### Limitations

 - The exhaustive search is exponentially growing with the input, limiting the use of the superoptimizer with larger programs.
 - Pointer instructions are not very well supported because of their expensive search space requirements.
 - The superoptimizer must be written using the same instruction set as the program being optimized.

### Future Work

The author hopes to use the superoptimizer to explore the relationships betweeen arithmetic and logical operators, in order to compile a list of useful idioms that can be used to optimize programs.