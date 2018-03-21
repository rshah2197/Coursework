The y86 emulator parses through the .y86 files which creates memory based on the values such as .size, .text etc.  -H prints out a message explaining the usage of the program.  We also had to implement error checking.

After the memory is done populating (loop), that's where the program checks for the opcodes.

Populating the text was key because those are were the opcodes are.
Each op code tells you the instruction that needs to be done, whether its arithmatic or just the standard return/push/pop procedures as well as mov/comp/write/read instructions.

When trying to figure out the code for each instruction, I had to visualize what both registers were doing.

We had to use flags to detect overflow and  use status symbols.


prog2.y86 does a factorial calculation prompting the user to enter a number and returning the result.

prog1.y86 reads a message from the TA and Professor Russell.
