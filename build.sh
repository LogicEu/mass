#!/bin/bash

cc=gcc
src=src/common/*.c src/2D/*.c src/3D/*.c
name=libmass

flags=(
    -std=c99
    -Wall
    -Wextra
    -O2
)

inc=(
    -I.
    -Iinclude
)

lib=(
    -Llib
    -lfract
    -lutopia
)

lib_build() {
    pushd $1/ && ./build.sh $2 && popd && mv $1/lib$1.a lib/lib$1.a
}

build() {
    [ ! -d lib/ ] && mkdir lib/
    lib_build fract static
    lib_build utopia static
}

shared() {
    if echo "$OSTYPE" | grep -q "darwin"; then
        $cc ${flags[*]} ${inc[*]} ${lib[*]} -dynamiclib $src -o $name.dylib 
    elif echo "$OSTYPE" | grep -q "linux"; then
        $cc -shared ${flags[*]} ${inc[*]} ${lib[*]} -lm -fPIC $src -o $name.so 
    else
        echo "This OS is not supported yet..." && exit
    fi
}

static() {
    $cc ${flags[*]} ${inc[*]} -c $src && ar -cr $name.a *.o && rm *.o
}

cleanf() {
    [ -f $1 ] && rm $1 && echo "deleted $1"
}

cleand() {
    [ -d $1 ] && rm -r $1 & echo "deleted $1"
}

clean() {
    cleanf $name.a
    cleanf $name.so
    cleanf $name.dylib
    cleand lib
    return 0
}

install() {
    [ "$EUID" -ne 0 ] && echo "Run with sudo to install" && exit

    build && shared && static
    cp mass.h /usr/local/include/

    [ -f $name.a ] && mv $name.a /usr/local/lib/
    [ -f $name.so ] && mv $name.so /usr/local/lib/
    [ -f $name.dylib ] && mv $name.dylib /usr/local/lib/

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
        build && shared;;
    "static")
        static;;
    "clean")
        clean;;
    "install")
        install;;
    "uninstall")
        uninstall;;
    *)
        echo "Run with 'static' or 'shared' to build."
        echo "Use 'install' to build and install in /usr/local"
        echo "Use 'clean' to remove local builds.";;
esac
