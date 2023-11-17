#!/usr/bin/env -S bash ../.port_include.sh
port='c-ares'
version='1.22.0'
files=(
    "https://c-ares.org/download/c-ares-${version}.tar.gz#ad2e205088083317147c9f9eab5f24b82c3d50927c381a7c963deeb1182dbc21"
)
useconfigure='true'

configure() {
    mkdir -p c-ares-build
    cmake -G Ninja \
        -DCMAKE_TOOLCHAIN_FILE="${SERENITY_BUILD_DIR}/CMakeToolchain.txt" \
        -S "${workdir}" \
        -B c-ares-build
}

build() {
    ninja -C c-ares-build "${makeopts}"
}

install() {
    ninja -C c-ares-build install
}
