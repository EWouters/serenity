#!/usr/bin/env -S bash ../.port_include.sh
port='awk'
version='2ndEdition'
files=(
    "https://github.com/onetrueawk/awk/archive/refs/tags/${version}.tar.gz#6c3dbd72b6c4dd58340686df0b189a7813cf49aeeb9aa14846dfbeca1bc8d3a7"
)

build() {
    run make "${makeopts[@]}"
    run mv a.out awk
}

install() {
    run mkdir -p "${SERENITY_INSTALL_ROOT}/usr/local/bin/"
    run cp awk "${SERENITY_INSTALL_ROOT}/usr/local/bin/"
}
