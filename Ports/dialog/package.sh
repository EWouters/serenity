#!/usr/bin/env -S bash ../.port_include.sh
port='dialog'
version='1.3-20231002'
files=(
    "https://invisible-mirror.net/archives/dialog/dialog-${version}.tgz#315640ab0719225d5cbcab130585c05f0791fcf073072a5fe9479969aa2b833b"
)
useconfigure='true'
use_fresh_config_sub='true'
configopts=(
    '--prefix=/usr/local'
    '--with-ncurses'
    "--with-curses-dir=${SERENITY_INSTALL_ROOT}/usr/local/include/ncurses"
)
depends=(
    'ncurses'
)
