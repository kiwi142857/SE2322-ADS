# Lab1 handout introduction

## File structure

.\Lab1-handout
├── compressed_radix_tree.cpp
├── compressed_radix_tree.hpp
├── compressed_radix_tree.o
├── expected
├── output
├── input
├── grade.ps1
├── grade.sh
├── main.cpp
├── Makefile
├── radix_tree.cpp
├── radix_tree.hpp
├── README.md
├── testUtil.cpp
├── tree.hpp
├── util.hpp
├── ycsb.cpp

## Compile instruction

makefile is provided. You can compile the program by running `make` in the Lab1-handout directory.

``make main`` will compile the main program.

``make ycsb`` will compile the ycsb program.

``make grade`` will compile the grading program.

## Test instruction

``./ycsb`` will run the ycsb program.

``` c
Usage: ./ycsb.exe <tree_type> <workload> <testtimes>
tree_type: RadixTree, CompressedRadixTree RedBlackTree
workload: 1, 2, 3
testtimes: 10, 20, 30
Example: ./ycsb.exe RadixTree 1 60
```

The result will be recorded in the file ``log.csv`` in the dictionary ``./Log``.
