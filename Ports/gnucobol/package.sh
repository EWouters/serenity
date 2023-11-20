#!/usr/bin/env -S bash ../.port_include.sh
port='gnucobol'
version='3.2'
useconfigure='true'
depends=(
    'gmp'
    'gcc'
    'bash'
    'ncurses'
)
files=(
    "https://ftpmirror.gnu.org/gnu/gnucobol/gnucobol-${version}.tar.bz2#73d2b8801cb30cdb371a1fb4297fb185582439d024f7c0dce425f9373c43f22d"
)
configopts=(
    "--with-sysroot=${SERENITY_INSTALL_ROOT}"
    '--prefix=/usr/local'
    '--enable-hardening'
    '--disable-rpath'
    '--disable-nls'
    '--with-gnu-ld'
    '--with-dl'
    '--with-math=gmp'
    '--with-curses=ncurses'
    '--with-db=no'
    '--with-json=no'
)
