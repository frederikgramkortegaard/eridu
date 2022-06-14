""" Utu - Benchmarking Framework for the Eridu Compiler.

Usage: utu.py [-verbose] -path <folder/filepath> -src <folder containing ./compiler> -step -runs <number of runs>
"""

import os
import sys
import json
import subprocess
import timeit
from collections import defaultdict
import datetime
import time


# Defaults
timeit.default_timer = time.process_time
folder_blacklist = ["./.git", "./resources", "./data", "./__pycache__", ".", "./results"]
supported_file_types = [".erd", ".c", ".cs", ".cpp"]

# Testers
def test_erd(path, src):
    """ Test an erd file. """
    subprocess.call(f"{src}/compiler < {os.path.abspath(path)}",
                    stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, shell=True)
    subprocess.call(f"gcc -no-pie output.s", stdout=subprocess.DEVNULL,
                    stderr=subprocess.DEVNULL, shell=True)

    time = timeit.timeit(stmt=f"subprocess.call('./a.out', stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)",
                  setup="import subprocess", number=number_of_runs)
    subprocess.run(["rm", f"a.out", f"output.s"])
    return time


def test_c(path, src):
    """ Test a C file. """
    subprocess.call(["gcc","-O3","-static","-o", "test", path, "-lm"])
    time = timeit.timeit(stmt="subprocess.call('./test', stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)",
                  setup="import subprocess", number=number_of_runs)
    subprocess.run(["rm", "test"])
    return time


def test_cpp(path, src):
    """ Test a C file. """
    subprocess.call(["g++","-O3","-static","-o", "test", path, "-lm"])
    time = timeit.timeit(stmt="subprocess.call('./test', stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)",
                  setup="import subprocess", number=number_of_runs)
    subprocess.run(["rm", "test"])
    return time

def test_cs(path, str):
    """ Test C# File """

    subprocess.call(["mcs","-optimize","-out:out.exe", path])
    time = timeit.timeit(stmt="subprocess.call(['mono', 'out.exe'], stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)",
                  setup="import subprocess", number=number_of_runs)
    subprocess.run(["rm", "out.exe"])
    return time


def get_tester(extension):
    """ Get the tester for a file. """

    if(extension.startswith('.')):
        extension = extension[1:]
    try:
        return globals()["test_" + extension]
    except:
        return None


def test_folder(path, src, verbose=False):
    """ Test a folder. """

    times = defaultdict(float)

    for file in os.listdir(path):

        _, file_extension = os.path.splitext(os.path.join(path, file))

        tester = get_tester(file_extension)
        if tester is None:
            continue

        times[file] = tester(os.path.join(path, file), src) / number_of_runs

        if verbose:
            print(f"\t{file_extension}\t {times[file]}")

    return times


if __name__ == "__main__":

    print("Utu - Benchmarking Framework for the Eridu Compiler.")
    if "-help" in sys.argv[1:]:
        print(
            """Usage: utu.py [-verbose] [-make] -path <folder> -src <folder containing ./compiler> -runs <number of runs>
            \t-step: Pauses after each benchmark.
            \t-verbose: Prints the output of the compiler.
            \t-runs: Number of runs to benchmark.
            \t-help: Prints this help message.""")

        print(f"\nSupported File Types: {supported_file_types}")
        exit()

    if not "-src" in sys.argv[1:]:
        print("ERROR: -src must be specified to specify the source directory")
        quit()

    src_dir = sys.argv[sys.argv.index("-src") + 1]

    if not "-runs" in sys.argv[1:]:
        number_of_runs = 1000
    else:
        number_of_runs = int(sys.argv[sys.argv.index("-runs") + 1])

    print(f"\nBenchmarking on {number_of_runs} runs")

    if "-path" not in sys.argv[1:]:
        print("Path not specified, expecting -path <path>")
        quit()

    path = sys.argv[sys.argv.index("-path") + 1]

    verbose = False
    if "-verbose" in sys.argv[1:]:
        print("Verbose mode enabled")
        verbose = True

    step = False
    if "-step" in sys.argv[1:]:
        print("Stepping enabled, will pause after each benchmark.")
        step = True
        if verbose == False:
            print("Enabling verbose mode for stepping.")
        verbose = True

    if os.path.isdir(path):

        # Benchmark all tests found in path
        if path == '.':
            times = defaultdict(dict)
            for root, dirss, files in os.walk(path, topdown=True):
                if root in folder_blacklist:
                    continue
                
                print(f"Benchmarking '{root}'")
                times[root] = test_folder(root, src_dir, verbose)
                if step:
                    input("Press Enter to continue...")


            times["time"] = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            times["runs"] = number_of_runs 
            json.dump(times, open(f"./results/{times['time']}.json", "w"))

        # Benchmark test found in path
        else:
            print(f"Testing '{path}'")

            times = test_folder(path, src_dir, verbose)
            # Sort times by filetype
            times = sorted(times.items(), key=lambda x: x[1])
            print(f"Average times over {number_of_runs} runs in seconds:")
            for file, time in times:
                print(f"\t{file}\t {time}")

    else:
        print("Path is not a directory")
        quit()
