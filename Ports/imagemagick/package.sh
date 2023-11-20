#!/usr/bin/env -S bash ../.port_include.sh
port='imagemagick'
version='7.1.1-21'
useconfigure='true'
files=(
    "git+https://github.com/ImageMagick/ImageMagick.git#${version}"
)
configopts=(
    "--with-sysroot=${SERENITY_INSTALL_ROOT}"
)
depends=(
    'libjpeg'
    'libpng'
    'libtiff'
)
