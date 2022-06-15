# Eridu | C-Like Programming Language
Eridu is a fully fledged C-Like programming language featuring various abstract data types, functions as first-class citizens, and multi-paradigm functionality. Every step including semantic analysis, machine independent optimization, custom intermediate representations, and x86 Assembly code generation were written from scratch in C. Testing and benchmarking was performed using Python based frameworks written from scratch, as shown in sections 6. and 8. of the thesis in which a formal description of Eridu can be found.

:pushpin: [Thesis](https://github.com/frederikgram/eridu/blob/master/Thesis.pdf)

:pushpin: [Code Examples](https://github.com/frederikgram/eridu/tree/master/examples)

:pushpin: [Project Description](https://github.com/frederikgram/eridu/blob/master/projectdescription.pdf)

:speech_balloon: The reader is advised to look at section 8. regarding performance evaluation in the attached thesis to get an overview of the viability of the language as well as comparisons to C and C++ with regards to run-time and scalability metrics.

---
### Using the Eridu compiler
To use the compiler, the following command can be run:
```
./path/to/compiler < ./path/to/file.erd
```

From here, the newly created `output.s` file can be constructed into a runnable by using `gcc`. Here it is important to note that the `-no-pie` flag for GCC is required when using strings.

We advice the reader to test the following examples:
```
./build/compiler < ./examples/generic_vehicle_simulator.erd`

./build/compiler < ./examples/iterator.erd
```
Followed by:

```
gcc -no-pie output.s
./a.out
```
### Compiling Eridu from Source
It is possible to compile Eridu from source, by navigating to the `./dev/src` folder and running `make`. Verbose output from the compilation process such as Abstract Syntax Tree structures or the code generation process can be exposed by using `make debug` instead. 

### Testing
Tests can be run using our testing framework _Gula_, to use this framework, Python 3.8 and the numpy package is required. To see the help menu, use the command:
```
python3 ./tests/gula.py -help
```

And for a complete test pass, run:

```
python3 ./tests/gula.py -src ../build -path ./tests -verbose
```

### Benchmarking
Benchmarking can be performed using our benchmarking framework _Utu_ by running the following command:
```
python3 ./benchmarks/utu.py -src ./build -path . -verbose
```

Too see all the available options, run:

```
python3 ./benchmarks/utu.py -help
```
