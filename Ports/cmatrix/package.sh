#!/usr/bin/env -S bash ../.port_include.sh
port='cmatrix'
useconfigure='true'
version='ac78ef9b78948d4da162795fb4043e874dfd1ee8'
depends=(
    'ncurses'
)
configopts=(
    "-DCMAKE_TOOLCHAIN_FILE=${SERENITY_BUILD_DIR}/CMakeToolchain.txt"
)
files=(
    "git+https://github.com/abishekvashok/cmatrix.git#${version}"
)
launcher_name='cmatrix'
launcher_category='Games'
launcher_command='/usr/local/bin/cmatrix'
launcher_run_in_terminal='true'

configure() {
    run cmake "${configopts[@]}"
}

install() {
    mkdir -p "${SERENITY_INSTALL_ROOT}/usr/local/bin"
    run cp cmatrix "${SERENITY_INSTALL_ROOT}/usr/local/bin"
}
