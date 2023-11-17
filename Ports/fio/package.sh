#!/usr/bin/env -S bash ../.port_include.sh
port='fio'
version='3.36'
files=(
    "https://brick.kernel.dk/snaps/${port}-${version}.tar.gz#0a07354876ca4d23518f8aa88682f23866455bbd2ff2d0f055d6e4b72f156553"
)
depends=(
    'zlib'
)

export LDFLAGS='-ldl'
