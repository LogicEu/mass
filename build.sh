#!/bin/bash

cc=gcc
src=src/*.c
name=libmass

flags=(
    -std=c99
    -Wall
    -Wextra
    -pedantic
    -O2
    -I.
    -Iinclude
)

libs=(
    -Llib
    -lutopia
    -lfract
)

if echo "$OSTYPE" | grep -q "darwin"; then
    dlib=(
        -dynamiclib
    )
    suffix=.dylib
elif echo "$OSTYPE" | grep -q "linux"; then
    dlib=(
        -lm
        -shared
        -fPIC
    )
    suffix=.so
else
    echo "This OS is not supported by this shell script yet..." && exit
fi

cmd() {
    echo "$@" && $@
}

lib_build() {
    [ -f lib/lib$1.a ] && return 0
    cmd pushd $1/ && cmd ./build.sh $2 && cmd cp bin/* ../lib/ && cmd popd
}

build() {
    cmd mkdir -p lib
    lib_build fract static
    lib_build utopia static
}

shared() {
    build

    cmd mkdir -p tmp
    cmd $cc -c $src ${flags[*]} && cmd mv *.o tmp/ || exit
    
    cmd mkdir -p bin
    cmd $cc tmp/*.o -o bin/$name$suffix ${libs[*]} ${dlib[*]}
}

static() {
    cmd mkdir -p tmp
    cmd $cc ${flags[*]} -c $src && cmd mv *.o tmp/ || exit
    
    cmd mkdir -p bin
    cmd ar -cr bin/$name.a tmp/*.o
}

cleand() {
    [ -d $1 ] && cmd rm -r $1
}

cleanf() {
    [ -f $1 ] && cmd rm $1
}

cleanr() {
    cmd pushd $1 && ./build.sh clean && cmd popd
}

clean() {
    cleand lib
    cleand bin
    cleand tmp
    cleanr utopia
    cleanr fract
    return 0
}

install() {
    [ "$EUID" -ne 0 ] && echo "Run with sudo to install" && exit
    
    make all -j # or shared && static
    cmd cp mass.h /usr/local/include/

    [ -f bin/$name.a ] && cmd mv bin/$name.a /usr/local/lib
    [ -f bin/$name.so ] && cmd mv bin/$name.so /usr/local/lib
    [ -f bin/$name.dylib ] && cmd mv bin/$name.dylib /usr/local/lib
    
    echo "Successfully installed $name"
    return 0
}

uninstall() {
    [ "$EUID" -ne 0 ] && echo "Run with sudo to uninstall" && exit

    cleanf /usr/local/include/mass.h
    cleanf /usr/local/lib/$name.a
    cleanf /usr/local/lib/$name.so
    cleanf /usr/local/lib/$name.dylib

    echo "Successfully uninstalled $name"
    return 0
}

case "$1" in
    "build")
        build;;
    "shared")
        shared;;
    "static")
        static;;
    "all")
        shared && static;;
    "make")
        make all -j;;
    "clean")
        clean;;
    "install")
        install;;
    "uninstall")
        uninstall;;
    *)
        echo "Run with 'shared' or 'static' to build"
        echo "Use 'install' to build and install in /usr/local"
        echo "Use 'clean' to remove local builds"
esac
