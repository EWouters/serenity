#!/usr/bin/env -S bash ../.port_include.sh
port=dropbear
version=2022.83
files=(
    "https://mirror.dropbear.nl/mirror/releases/dropbear-${version}.tar.bz2#bc5a121ffbc94b5171ad5ebe01be42746d50aa797c9549a4639894a16749443b"
)
useconfigure=true
use_fresh_config_sub=true
# don't care about zlib, less deps is better
configopts=("--disable-zlib")
# Serenity's utmp is not fully compatible with what dropbear expects.
configopts+=("--disable-utmp" "--disable-wtmp" "--disable-login" "--disable-lastlog")
