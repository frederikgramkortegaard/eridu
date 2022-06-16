""" Gula - Testing Framework for the Eridu Compiler.

Usage: gula.py [-verbose] -path <folder/filepath> -src <folder containing ./compiler>

### Format of testing files ### 

    #---------------------------------#
    |    ERIDU-CODE                   |
    |//%% OUTPUT                      |
    |//First Expected Line of Output |
    |//Second Expected Line ...      |
    |//%% END
    #---------------------------------#

"""

from collections import defaultdict
import os
import sys
import subprocess

from numpy import True_




def test_file(filepath: str, compilerpath, verbose=False) -> bool:
    """ Parses the test file and compares the output to the expected output. """

    variables = defaultdict()
    with open(filepath, "r") as file:
        data = file.read()

    # @TODO : Nested try is not ideal, but it works.
    try:
        try:
            expected_lines = []
            for line in data.split('%% OUTPUT\n')[1].split('%% END\n')[0].split('\n')[:-1]:
                expected_lines.append(line[2:].replace('\n', ''))

        # If the file doesn't have an expected output, we can still check for stderr.
        except IndexError:
            pass

        if ("$$segfault" in expected_lines or "$$error" in expected_lines) and len(expected_lines) > 1:
            print("\t\033[1;31mMultiple expected lines of output are not allowed when using '$$segfault' or '$$error'\033[m  ") 
            return False


        # @TODO : Could probably be converted into a list of arguments to be passed to subprocess.
        subprocess.call(f"{compilerpath}/compiler < {os.path.abspath(filepath)}",
                        stdout=open("tmp.erd", 'w'), stderr=open("err", 'a'), shell=True)
        
        subprocess.call(f"gcc -no-pie output.s", stdout=open("tmp.erd",
                        'a'), stderr=open("err", 'a'), shell=True)
            
        subprocess.call(f"stdbuf -o0 ./a.out > tmp.erd", stderr=open("err", 'a'), shell=True)

        
        if (w := open("err", 'r').read()) != "":
            if "Segmentation fault" in w and "$$segfault" in expected_lines:
                    print(f"\033[1;32mTest Passed: '{filepath}'\033[m")
                    return True
            elif "gcc: error: output.s:" in w and "$$error" in expected_lines:
                    print(f"\033[1;32mTest Passed: '{filepath}'\033[m")
                    return True
            elif "Segmentation fault" in w and "gcc:error: output.s:" not in w:
                print(f"\033[1;31mTest Failed: '{filepath}'\033[m  ")
                print("\tRuntime Segmentation fault")
                return False
            elif "Segmentation fault" in w and "gcc:error: output.s:" in w:
                print(f"\033[1;31mTest Failed: '{filepath}'\033[m  ")
                print("\tCompiletime Segmentation fault")
                return False
                

            print(f"\033[1;31mTest Failed: '{filepath}'\033[m  ", end='')
            if(verbose):
                print('\n\t', '\n\t'.join(w.split('\n')), end='', sep='')
            print()
            return False

        if len(expected_lines) == 0:
            print(f"\033[1;32mTest Passed: '{filepath}'\033[m")
            return True


        with open("tmp.erd", "r") as f:
            lines = f.readlines()

            if len(lines) != len(expected_lines):
                print(f"\033[1;31mTest Failed: '{filepath}'\033[m  ", end='')
                if(verbose):
                    print(f"\n\tExpected {len(expected_lines)} lines but received {len(lines)}", end='', sep='') 
                print()
                return False
            for enum, line in enumerate(lines):
                line = line.rstrip()

                if expected_lines[enum].startswith("$$"):

                    if expected_lines[enum][2:] == "error":
                        pass

                    elif expected_lines[enum][2:] == "segfault":
                        pass
                    
                    elif expected_lines[enum][2:] not in variables:
                        variables[expected_lines[enum][2:]] = line
                        continue
                    else:
                        expected_lines[enum] = variables[expected_lines[enum][2:]]
                    

                if line != expected_lines[enum]:
                    print(
                        f"\033[1;31mTest Failed: '{filepath}'\033[m  ", end='')
                    if verbose:
                        print(
                            f"\t\033[1;31m\n\tExpected #{enum} Output: '{expected_lines[enum]}'\033[m ")
                        print(
                            f"\033[1;31m\tActual #{enum} Output: '{line}'\033[m  ", end='')
                    print()

                    return False
            print(f"\033[1;32mTest Passed: '{filepath}'\033[m")
            return True

    # Unexpected error, this should never happen.
    except Exception as e:
        raise e

    finally:
        with open(os.devnull, "wb") as devnull:
            subprocess.run(["rm", "output.s", "a.out", "tmp.erd","err"], stdout=devnull, stderr=devnull)

def test_folder(folderpath: str, compilerpath, verbose=False, step=False) -> bool:
    """ Runs test_file on every .erd source file in the folder. """

    files = filter(lambda name: name.endswith(".erd"), os.listdir(folderpath))

    successes = 0
    for file in files:
        successes += 1 if test_file(os.path.join(folderpath, file), compilerpath, verbose) else 0
        if step:
            input("Press Enter to continue...")

    return successes

def quit():
    print("See 'gula -help'")
    exit()


if __name__ == "__main__":

    print("Gula - Testing Framework for the Eridu Compiler.\n")
    if "-help" in sys.argv[1:]:
        print(
            """Usage: gula.py [-verbose] [-make] -path <folder/filepath> -src <folder containing ./compiler> [-step]
            \t-step: Pauses after each test.
            \t-verbose: Prints the output of the compiler.
            \t-help: Prints this help message.""")
        exit()

    if not "-src" in sys.argv[1:]:
        print("ERROR: -src must be specified to specify the source directory")
        quit()

    src_dir = sys.argv[sys.argv.index("-src") + 1]

    if "-path" not in sys.argv[1:]:
        print("Path not specified, expecting -path <path>")
        quit()

    path = sys.argv[sys.argv.index("-path") + 1]

    verbose = False
    if "-verbose" in sys.argv[1:]:
        print("Verbose mode enabled")
        verbose = True

    if not os.path.exists(path):
        print("Path does not exist.")
        quit()

    step = False
    if "-step" in sys.argv[1:]:
        print("Stepping enabled, will pause after each test.")
        step = True

    if os.path.isfile(path):
        if not path.endswith(".erd"):
            print("Path is not a .erd file.")
            quit()

        print(f"Beginning Testing Framework on File: '{path}'")
        test_file(path, src_dir, verbose)

    elif os.path.isdir(path):
        print(f"Beginning Testing Framework on Folder: '{path}'")
        dirs = []
        count = 0

        print("Recursively searching for .erd files.")
        for root, dirss, files in os.walk(path, topdown=True):
            dirs.append(root)
            count += len(files)
                
        successes = sum([test_folder(dir, src_dir, verbose, step) for dir in dirs])
        if(successes > 0):
            print(f"\n{successes}/{count} Tests Passed\n{int(successes/count*100)}% of tests passed")
        else:
            print(f"\n0 successes Tests Passed\n")
    else:
        print("Path is neither a file nor a folder.")
        quit()
