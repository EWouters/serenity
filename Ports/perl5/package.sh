#!/usr/bin/env -S bash ../.port_include.sh
port='perl5'
version='5.38.0'
useconfigure='true'
files="https://www.cpan.org/src/5.0/perl-${version}.tar.xz perl5-${version}.tar.xz eca551caec3bc549a4e590c0015003790bdd1a604ffe19cc78ee631d51f7072e
https://github.com/arsv/perl-cross/releases/download/1.5/perl-cross-1.5.tar.gz perl-cross.1.5.tar.gz d744a390939e2ebb9a12f6725b4d9c19255a141d90031eff90ea183fdfcbf211"
configopts=(
    "-Dosname=serenity"
    "--target=${SERENITY_ARCH}-pc-serenity"
    "--targetarch=${SERENITY_ARCH}"
    "--build=$(cc -dumpmachine)"
    "--buildarch=$(uname -m)"
    "--sysroot="${SERENITY_INSTALL_ROOT}""
)
workdir="perl-${version}"

post_fetch() {
    run cp -r "$PWD/perl-cross-1.5/"* "$PWD/perl-${version}/"
    run cat <<- 'EOH' > $PWD/perl-${version}/cnf/hints/serenity
libs='-ldl -lm -lcrypt -lcore'

# Use OS's malloc() by default.
case "$usemymalloc" in
'') usemymalloc='n' ;;
esac

# uses gcc.
cc='gcc'
ld='gcc'

# as of the latest some symbols are confusing the nm lookup
case "$usenm" in
'') usenm='undef' ;;
esac

# enable nanosleep
d_nanosleep='define'

# don't try to test min/max of gmtime/localtime
sGMTIME_max=2147483647
sGMTIME_min=-2147481748
sLOCALTIME_max=2147483647
sLOCALTIME_min=-2147481748
EOH
}
