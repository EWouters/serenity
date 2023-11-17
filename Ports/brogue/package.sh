#!/usr/bin/env -S bash ../.port_include.sh
port='brogue'
depends=(
    'SDL2'
    'SDL2_image'
)
version='1.13'
workdir="BrogueCE-${version}"
files=(
    "https://github.com/tmewett/BrogueCE/archive/refs/tags/v${version}.tar.gz#4c63e91639902d58565ab3c2852d89a4206cdd60200b585fa9d93d6a5881906c"
)
makeopts+=(
    'bin/brogue'
)
launcher_name='Brogue'
launcher_category='Games'
launcher_command='/usr/local/bin/brogue'
icon_file='bin/assets/icon.png'

install() {
    datadir="${SERENITY_INSTALL_ROOT}/usr/local/share/games/brogue/assets"
    mkdir -p "${SERENITY_INSTALL_ROOT}/usr/local/bin"
    mkdir -p "${datadir}"
    cp "${workdir}"/bin/assets/* "${datadir}"
    cp "${workdir}/bin/brogue" "${SERENITY_INSTALL_ROOT}/usr/local/bin"
}
