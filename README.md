# Eridu | C-Like Programming Language
Eridu is a C-Like Language featuring Abstract Data Types, Function Pointers, a Custom Intermediate Representation and Optimized x86 AT&T ASM Output.

:pushpin: [Thesis](https://github.com/frederikgram/eridu/blob/master/Thesis.pdf)

:pushpin: [Code Examples](https://github.com/frederikgram/eridu/tree/master/examples)

:pushpin: [Project Description](https://github.com/frederikgram/eridu/blob/master/projectdescription.pdf)

:speech_balloon: The reader is advised to look at section _8.x_ regarding performance evaluation in the attached thesis to get an overview of the viability of the language as well as comparisons to C and C++.

---

### How to compile
It is possible to compile Eridu from source, by navigating to the `./dev/src` folder and running `make`, it is recommended to run `make clean` before any attempt to compile Eridu. 
### Running the pre-made code examples
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
