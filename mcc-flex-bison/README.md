# mC Compiler by [Manuel Buchauer](https://git.uibk.ac.at/csat2263) and [Davide De Sclavis](https://git.uibk.ac.at/csat2264)

This repository holds all the necessary code for an mc Compiler for the course compiler construction at the University of Innsbruck: 
[compiler construction course](https://git.uibk.ac.at/c7031162/703602-Compiler-Construction).

- The used *getting started* code-base by Alexander Hirsch is given at:
[Getting Started Code-Base](https://git.uibk.ac.at/c7031162/mcc)

- The development notes are given at: 
[Development Notes](docs/development_notes.md)

- The specification is given at: 
[Specification](https://git.uibk.ac.at/c7031162/703602-Compiler-Construction/blob/master/specification.md)

We used `flex` and `bison` for parsing the code.

## Prerequisites

- [Meson](http://mesonbuild.com/) in a recent version (`0.44.0`)
  (you may want to install it via `pip3 install --user meson`)
- [Ninja](https://ninja-build.org/)
- `time`, typically located at `/usr/bin/time`, do not confuse this with the Bash built-in
- `flex` for generating the lexer
- `bison` for generating the parser
- A compiler supporting C11 — typically GCC or Clang

## Build Instructions

First, generate the build directory.

    meson builddir
    cd builddir

Meson creates Ninja build files.
Let's build.

    ninja

For cleaning.

    ninja clean

Unit tests can be run directly with Ninja (or Meson).

    ninja test

For integration testing we try to compile mC programs and compare their output for a given input.

    ../scripts/run_integration_tests

Taken from the [Meson Documentation](https://mesonbuild.com/Unit-tests.html#coverage):

> If you enable coverage measurements by giving Meson the command line flag `-Db_coverage=true`, you can generate coverage reports.
> Meson will autodetect what coverage generator tools you have installed and will generate the corresponding targets.
> These targets are `coverage-xml` and `coverage-text` which are both provided by Gcovr and `coverage-html`, which requires Lcov and GenHTML or Gcovr with html support.


## Applications
Apart from the main compiler executable mcc, additional auxiliary executables are provided:

- `.\mcc` : This is the main compiler executable.
- `.\mc_ast_to_dot` : Utility for printing an abstract syntax tree in the DOT format
- `.\mc_symbol_table` : Utility for displaying the generated symbol tables.
- `.\mc_ir` : Utility for viewing the generated intermediate representation.
- `.\mc_cfg_to_dot` : Utility for printing a control flow graph in the DOT format.
- `.\mc_asm` : Utility for printing the generated assembly code.

## Testing

### There exist 4 test files for unit testing:
- parser_test.c: Contains all necessary parser tests (including parsing of all integration test files)
- semantic_checks_test.c: Testing the semantic checks according to the specification.
- symbol_table_test.c: Testing the correct creation of symbol tables. 
- intermediate_rep_test.c: Testing the correct creation of intermidate representation. 

All Unit test run correctly.

Using ```meson test --repeat 1000 --gdb ``` we tested each unit test file 1000 times and no segfaults occured. This test took approx. 15 minutes so we decided to not increase this number.




### Integration Tests
All provided test inputs (examples) build and run successfully.

## Known Issues


