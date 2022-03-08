#!/bin/bash

cc=gcc
src=src/*.c
name=libmass

flags=(
    -std=c99
    -Wall
    -Wextra
    -O2
)

inc=(
    -I.
    -Iutopia/
    -Ifract/
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
    $cc ${flags[*]} ${inc[*]} ${lib[*]} -dynamiclib $src -o $name.dylib 
}

linux_dlib() {
    $cc -shared ${flags[*]} ${inc[*]} ${lib[*]} -lm -fPIC $src -o $name.so 
}

lib_build() {
    pushd $1/ && ./build.sh $2 && popd && mv $1/lib$1.a lib/lib$1.a
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
    $cc ${flags[*]} ${inc[*]} -c $src && ar -crv $name.a *.o && rm *.o
}

cleanf() {
    [ -f $1 ] && rm $1
}

clean() {
    cleanf $name.dylib
    cleanf $name.so
    cleanf $name.a
    [ -d lib ] && rm -r lib
}

case "$1" in
    "-build")
        build;;
    "-d")
        build && dlib;;
    "-s")
        slib;;
    "-clean")
        clean;;
    *)
        fail_op;;
esac
