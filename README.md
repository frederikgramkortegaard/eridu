# Eridu - C-Like Programming Language
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://github.com/frederikgram/eridu/blob/master/LICENSE)

Eridu is a fully fledged C-Like programming language featuring various abstract data types, functions as first-class citizens, and multi-paradigm functionality, written from scratch in C , targeting x86 AT&T Assembly.

Testing and benchmarking was performed using a custom Python framework written from scratch as shown in sections 6. and 8. of the thesis in which a formal description of Eridu can also be found.

:pushpin: [Thesis](https://github.com/frederikgram/eridu/blob/master/Thesis.pdf)

:pushpin: [Code Examples](https://github.com/frederikgram/eridu/tree/master/examples)

:pushpin: [Project Description](https://github.com/frederikgram/eridu/blob/master/projectdescription.pdf)

:speech_balloon: The reader is advised to look at section 8. regarding performance evaluation in the attached thesis to get an overview of the viability of the language as well as comparisons to C and C++ with regards to run-time and scalability metrics.

---
## Using the Eridu compiler
To use the compiler, the following command can be run:
```bash
$ ./path/to/compiler < ./path/to/file.erd
```

From here, the newly created `output.s` file can be constructed into a runnable by using `gcc`. Here it is important to note that the `-no-pie` flag for GCC is required when using strings.

We advice the reader to test the following examples:
```bash
$ ./build/compiler < ./examples/generic_vehicle_simulator.erd
$ gcc -no-pie output.s
$ ./a.out
```
```bash
$ ./build/compiler < ./examples/iterator.erd
$ gcc -no-pie output.s
$ ./a.out
```


## Code Samples
### Map/Filter
```c
define int * map(int * arr, int size, <int> callable <int> func) {

    int i = 0;
    int * new_arr = allocate(int, size);
    while(i < size) {
        new_arr[i] = func(arr[i]);
        i = i + 1;
    };

    return new_arr;
};
```
```c
define int * filter(int * arr, int size, <int> callable <int> cond) {

    int i = 0;
    int j = 0;

    int * new_arr = allocate(int, size);

    while(i < size) {
        if(cond(arr[i]) == 1) {
            new_arr[j] = arr[i];
            j = j + 1;
        };
        i = i + 1;
    };

    return realloc(new_arr, size, j);
};
```
### Structs and Callables (Functions as First-Class Citizens)
```c
struct Iterator = {
    char ** values,
    int size,
    int current,
    <str> callable <struct Iterator *> next,
};

// Create dummy data
str * names = allocate(str, 5);
names[0] = "Michael";
names[1] = "Peter";
names[2] = "Hamurabe";
names[3] = "Wolfeschlegelsteinhausenberger";
names[4] = "Jackson";

struct Iterator * iter = create_iterator(names, 5);

// Test "next larger" iterator, starting from "Michael"
iter.next = _next_larger;
next = iter.next;
iter.current = 0;

str b = next(iter);
print(b); // Hamurabe
b = next(iter);
print(b); // Wolfeschlegelsteinhausenberger

```

## Compiling Eridu from Source
It is possible to compile Eridu from source, by navigating to the `./dev/src` folder and running `make`. Verbose output from the compilation process such as Abstract Syntax Tree structures or the code generation process can be exposed by using `make debug` instead. 

```bash 
$ cd ./dev/src && make
```



## Testing
Tests can be run using our testing framework _Gula_, to use this framework, Python 3.8 and the numpy package is required. To see the help menu, use the command:
```bash
$ python3 ./tests/gula.py -help
```

And for a complete test pass:

```bash
$ python3 ./tests/gula.py -src ../build -path ./tests -verbose
```

## Benchmarking
Benchmarking can be performed using our benchmarking framework _Utu_ by running the following command:
```bash
$ python3 ./benchmarks/utu.py -src ./build -path . -verbose
```

Too see all the available options, run:

```bash
$ python3 ./benchmarks/utu.py -help
```
