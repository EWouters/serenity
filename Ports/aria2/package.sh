#!/usr/bin/env -S bash ../.port_include.sh
port='aria2'
version='1.37.0'
files=(
    "https://github.com/aria2/aria2/releases/download/release-${version}/aria2-${version}.tar.xz#60a420ad7085eb616cb6e2bdf0a7206d68ff3d37fb5a956dc44242eb2f79b66b"
)
depends=(
    'c-ares'
    'libssh2'
    'libuv'
    'libxml2'
    'openssl'
    'sqlite'
    'zlib'
)
useconfigure='true'
configopts+=(
    '--with-libuv'
    "--with-sysroot=${SERENITY_INSTALL_ROOT}"
    '--without-libcares'
)
