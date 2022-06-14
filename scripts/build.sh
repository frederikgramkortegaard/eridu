cd ..

if [ -e "./dev/src/compiler" ]; then
    rm -rf ./build/compiler
    cp -r ./dev/src/compiler ./build/compiler

    typeset -i variable=$(cat ./build/version)
    ((variable += 1))
    echo "Building compiler version $variable"
    echo $variable > ./build/version
    echo $(date) > ./build/date


fi