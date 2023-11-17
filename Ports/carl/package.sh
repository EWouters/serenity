#!/usr/bin/env -S bash ../.port_include.sh
port='carl'
version='2.2'
workdir="cryanc-${version}"
files=(
    "https://github.com/classilla/cryanc/archive/refs/tags/${version}.tar.gz#2fa267ab96305d04b6144251f3a0fd7008aecae00aef43be0fb9bc2a75db0a7f"
)

build() {
    run "${CC}" -O3 'carl.c' -o 'carl'
}

install() {
    run mkdir -p "${SERENITY_INSTALL_ROOT}/usr/local"
    run cp 'carl' "${SERENITY_INSTALL_ROOT}/usr/local/bin"
}
