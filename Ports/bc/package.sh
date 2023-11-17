#!/usr/bin/env -S bash ../.port_include.sh
port='bc'
version='6.7.2'
files=(
    "https://github.com/gavinhoward/bc/releases/download/${version}/bc-${version}.tar.xz#0888d4739caa855c40eab655ae562e900fe7f45a7308ee2feeb95416ec13c7f1"
)
useconfigure='true'
configscript='configure.sh'
configopts=(
    '--prefix=/usr/local'
    '--disable-nls'
)

export CFLAGS='-O3 -flto'

configure() {
    run ./"${configscript}" "${configopts[@]}"
}
