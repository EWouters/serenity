#!/usr/bin/env -S bash ../.port_include.sh
port='gettext'
version='0.22.4'
useconfigure='true'
files=(
    "https://ftpmirror.gnu.org/gettext/gettext-${version}.tar.gz#c1e0bb2a4427a9024390c662cd532d664c4b36b8ff444ed5e54b115fdb7a1aea"
)
depends=(
    'libiconv'
)
configopts=(
    '--disable-curses'
    "--with-sysroot=${SERENITY_INSTALL_ROOT}"
)
