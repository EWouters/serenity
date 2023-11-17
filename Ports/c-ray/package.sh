#!/usr/bin/env -S bash ../.port_include.sh
port='c-ray'
version='feff2638cdb4d44a16bd7401c4ef7d4c0d4b7fde'
useconfigure='true'
files=(
    "git+https://github.com/vkoskiv/c-ray.git#${version}"
)
depends=(
    'SDL2'
)

configure() {
    run cmake \
        -DCMAKE_TOOLCHAIN_FILE="${SERENITY_BUILD_DIR}/CMakeToolchain.txt"
}

install() {
    mkdir -p "${SERENITY_INSTALL_ROOT}/home/anon/c-ray"
    cp -r "${workdir}"/* "${SERENITY_INSTALL_ROOT}/home/anon/c-ray"
}
