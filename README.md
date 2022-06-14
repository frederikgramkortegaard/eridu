# Eridu
### A C-Like Language featuring Abstract Data Types, Function Pointers, a Custom Intermediate Representation and Optimized x86 AT&T ASM Output


:pushpin: [Thesis](https://github.com/frederikgram/eridu/blob/master/Thesis.pdf)

:pushpin: [Project Description](https://github.com/frederikgram/eridu/blob/master/projectdescription.pdf)

:pushpin: [Code Examples](https://github.com/frederikgram/eridu/tree/master/examples)

:speech_balloon: The reader is advised to look at the section 8: _"performance evaluation"_ in the attached thesis to get an overview of the viability of the language. 

---

### How to compile
It is possible to compile Eridu from source, by navigating to the `./dev/src` folder and running `make`, it is recommended to run `make clean` before any attempt to compile Eridu. 
### How to run examples (patterns etc.)
To use the compiler, the following command can be run:
```
./path/to/compiler < ./path/to/file.erd
```

From here, the newly created `output.s` file can be constructed into a runnable by using `gcc`. Here it is important to note that the `-no-pie` flag for GCC is required when using strings.

We advice the reader to test the following examples:
```
./build/compiler < examples/generic_vehicle_simulator.erd`

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
