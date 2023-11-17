#!/usr/bin/env -S bash ../.port_include.sh
port='bash'
version='5.2.21'
useconfigure='true'
configopts=(
    '--disable-nls'
    '--without-bash-malloc'
)
files=(
    "https://ftpmirror.gnu.org/gnu/bash/bash-${version}.tar.gz#c8e31bdc59b69aaffc5b36509905ba3e5cbb12747091d27b4b977f078560d5b8"
)

post_configure() {
    run_replace_in_file 's/define GETCWD_BROKEN 1/undef GETCWD_BROKEN/' config.h
    run_replace_in_file 's/define CAN_REDEFINE_GETENV 1/undef CAN_REDEFINE_GETENV/' config.h
}

post_install() {
    mkdir -p "${SERENITY_INSTALL_ROOT}/bin"
    ln -sf '/usr/local/bin/bash' "${SERENITY_INSTALL_ROOT}/bin/bash"
}
