#!/bin/sh

if [ -z "$1" ]
then
PREFIX=/usr/local/lib/dquest
else
PREFIX=$1
fi

cd src
make distclean
qmake PREFIX=/
make INSTALL_ROOT=${PREFIX}  install
