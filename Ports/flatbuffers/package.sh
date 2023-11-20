#!/usr/bin/env -S bash ../.port_include.sh

port='flatbuffers'
version='23.5.26'
files=(
    "git+https://github.com/google/flatbuffers.git#v${version}"
)
useconfigure='true'
# Since we are cross-compiling, we cannot build the tests, because we need
# the flatbuffers compiler to build them
configopts=(
    "-DCMAKE_TOOLCHAIN_FILE=${SERENITY_BUILD_DIR}/CMakeToolchain.txt"
    '-DFLATBUFFERS_BUILD_TESTS=off'
)

configure() {
    run cmake "${configopts[@]}"
}

install() {
    run make install
}
