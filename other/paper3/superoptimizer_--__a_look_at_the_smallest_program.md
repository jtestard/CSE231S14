## Superoptimizer --  A Look at the Smallest Program

1. **Summary of the paper:**

	The goal of a superoptimizer is to generate a program that has the exact same functionality as the original program written in machine language but it has a significantly smaller size. The way used by the superoptimizer to generate a new program is by performing and exhaustive search over all possible programs, however it performs that search for a limited set of instructions. The opcodes of that set of instructions are stored in a lookup table and the superoptimizer uses this table to perform the generation of instructions with length that varies (it starts from 1 and it keeps increasing). After the generation of this program the superoptimizer checks that it has the same functionality with the original and if it does it prints the new program and halts. This test is performed with a probabilistic test (faster but less precise, it checks output for the same input set) and a boolean test (it compares the boolean representations of the programs, more precise but slow)

2. **Limitations:**
   * exhaustive search grows exponentially with the number of instructions (12 instructions -> several hours)
   * Difficult to use boolean expression with pointers since it might point anywhere.
   * Currently, only the 68020 version of the superoptimizer runs the probabilistic test, the machine independent version is limited to very short programs.
   
3. ** Future Work: **

	The author hopes to furtherly explore the idea of the relations between arithmetic and logical instructions and be able to compile a list of useful arithmetic-logican idioms that can be used to form optimal programs.