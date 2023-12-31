Cache Simulator
================

This program tries to simulate the behavior of a cache memory. It takes as input a cache configuration file, containing 
the parameters of the cache memory 'cache.config' and a file containing information about accesses to the cache 'cache.access'. 
The program outputs the details of each access, such as the set index, whether it was a hit or a miss, and the tag value.

Compilation
-----------
1. Navigate to the project directory.
2. Run 'make' to compile the program. This will create an executable named 'simulator'.

Execution:
---------
1. Put the cache configuration details in a file named 'cache.config' and the access details in a file named 'cache.access'.
2. Run './simulator' to execute the program.
3. The program will write the output to a file named 'output.txt'.

