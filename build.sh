#!/bin/bash

name=libmass

flags=(
    -std=c99
    -Wall
    -Wextra
    -O2
)

inc=(
    -I.
    -Iinclude/
)

lib=(
    -Llib/
    -lfract
    -lutopia
)

fail_op() {
    echo "Run with -d to build dynamically, or -s to build statically."
    exit
}

fail_os() {
    echo "OS is not supported yet..."
    exit
}

mac_dlib() {
    gcc ${flags[*]} ${inc[*]} ${lib[*]} -dynamiclib src/*.c -o $name.dylib
    install_name_tool -id @executable_path/$name.dylib $name.dylib 
}

linux_dlib() {
    gcc -shared ${flags[*]} ${inc[*]} ${lib[*]} -lm -fPIC src/*.c -o $name.so 
}

lib_build() {
    pushd $1/
    ./build.sh $2
    popd
    mv $1/lib$1.a lib/lib$1.a
}

build() {
    mkdir lib/
    lib_build fract -s
    lib_build utopia -slib
}

dlib() {
    if echo "$OSTYPE" | grep -q "darwin"; then
        mac_dlib
    elif echo "$OSTYPE" | grep -q "linux"; then
        linux_dlib
    else
        fails_os
    fi
}

slib() {
    gcc ${flags[*]} ${inc[*]} -c src/*.c
    ar -crv $name.a *.o
    rm *.o
}

clean() {
    rm -r lib/
}

if [[ $# < 1 ]]; then 
    fail_op
elif [[ "$1" == "-build" ]]; then
    build
elif [[ "$1" == "-d" ]]; then
    build
    dlib
    clean
elif [[ "$1" == "-s" ]]; then
    slib
elif [[ "$1" == "-clean" ]]; then
    clean
else
    fail_op
fi 
