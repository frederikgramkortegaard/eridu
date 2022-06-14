cd ../dev/src
make clean > /dev/null 2>&1
find . -name '*.c' | sed 's/.*/"&"/' | xargs  wc -l
make debug