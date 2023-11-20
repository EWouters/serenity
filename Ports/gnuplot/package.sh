#!/usr/bin/env -S bash ../.port_include.sh
port='gnuplot'
version='5.4.9'
useconfigure='true'
# Note: gnuplot's source code is hosted on SourceForge, but using the GitHub mirror makes checking out a version easier.
files=(
    "git+https://github.com/gnuplot/gnuplot.git#${version}"
)
depends=(
    'libgd'
    'lua'
)
configopts=(
    "--prefix=${SERENITY_INSTALL_ROOT}/usr/local"
    '--with-qt=no'
    '--with-readline=builtin'
    '--without-cairo'
    '--without-latex'
    'libgd_LIBS=-liconv -lfreetype -lfontconfig -lpng'
)

pre_configure() {
    run ./prepare
}

install() {
    run make install-strip
}
