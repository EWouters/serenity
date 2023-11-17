#!/usr/bin/env -S bash ../.port_include.sh

port='cfunge'
version='b7bf640b57f90f7b514a3289920954a05e7e224d'
useconfigure='true'
files=(
    "https://github.com/VorpalBlade/cfunge/archive/${version}.zip#c90303439f4bd0141d0e8a0f10b27efbe2f6358009825769670aad0096eae498"
)

configure() {
    run cmake -B build "${configopts[@]}"
}

build() {
    run make -C build "${makeopts[@]}"
}

install() {
    run cp build/cfunge "${SERENITY_INSTALL_ROOT}/bin"
}
