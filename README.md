# Sudoko Solver

I find Sudoko lame. Any "puzzle" that can be solved algorithmically and without creativity
isn't worth the time. So I wrote a solver.

## Building

Any reasonably modern C++ compiler should work. It works with gcc 6.4 and clang 3.6.2 for sure.
Compilation is trivial:

`g++ sudoku.cpp -o sudoko`

or

`clang++ -std=c++11  sudoku.cpp -o sudoku`

## Running

It reads the "puzzle" from stdin. There are examples in the test subdirectory. Error checking is
minimal. But it's like this:

`sudoku < test/test1.dat`

I'm already bored, that's enough.
