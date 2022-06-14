# Eridu
C-Like Languages featuring support for abstract data types, a custom intermediate representation and optimized x86 ASM output.

:pushpin: [Thesis](https://github.com/frederikgram/eridu/blob/master/Thesis.pdf)
:pushpin: [Project Description](https://github.com/frederikgram/eridu/blob/master/projectdescription.pdf)

NOTE: This compiler is only expected to work on Linux based distros such as Ubuntu. The correctness on other operating systems can not be guaranteed.

## User Guide
### How to compile
It is possible to compile Eridu from source, by navigating to the `./dev/src` folder and running `make`.
running `make clean` before any attempt at compilation is recommended. 
### How to run examples (patterns etc.)
To use the compiler, the following command can be run:
`./path/to/compiler < ./path/to/file.erd`.

From here, the newly created `output.s` file can be constructed into a runnable by using `gcc`. Here it is important to not that the `-no-pie` flag for GCC is required when using strings.

We advice the reader to test the following examples:
- `./build/compiler < examples/generic_vehicle_simulator.erd`

- `./build/compiler < ./examples/iterator.erd`

Followed by:

`gcc -no-pie output.s`

`./a.out`
### How to run tests
Tests can be run using our testing framework _Gula_, to use this framework, Python 3.8 and the numpy package is required. To see the help menu, use the command:

`python3 ./tests/gula.py -help`

And for a complete test pass, run:

`python3 ./tests/gula.py -src ../build -path ./tests -verbose`
### How to perform benchmarking
Benchmarking can be performed by running:

`python3 ./benchmarks/utu.py -src ./build -path . -verbose`
