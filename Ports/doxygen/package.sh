#!/usr/bin/env -S bash ../.port_include.sh
port='doxygen'
version='1.9.8'
files=(
    "https://github.com/doxygen/doxygen/archive/refs/tags/Release_${version//./_}.tar.gz#77371e8a58d22d5e03c52729844d1043e9cbf8d0005ec5112ffa4c8f509ddde8"
)
workdir="${port}-Release_${version//./_}"
useconfigure='true'
configopts=(
    '-Bbuild'
    '-GNinja'
    "-DCMAKE_TOOLCHAIN_FILE=${SERENITY_BUILD_DIR}/CMakeToolchain.txt"
    '-DCMAKE_BUILD_TYPE=Release'
    '-DCMAKE_POLICY_DEFAULT_CMP0148=OLD'
)
depends=(
    'libiconv'
)

configure() {
    run cmake "${configopts[@]}"
}

build() {
    run cmake --build build -j "$MAKEJOBS"
}

install() {
    run cmake --install build --prefix "$SERENITY_INSTALL_ROOT"
}
