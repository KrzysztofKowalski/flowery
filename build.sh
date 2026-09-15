#!/bin/sh
# build.sh — build the flowery SDL3 renderer.
#
#    ./build.sh          build ./flowery
#    ./build.sh clean    remove build products
#
# Any extra arguments are passed straight to make.
set -eu
cd "$(dirname "$0")"

if ! command -v pkg-config >/dev/null 2>&1; then
    printf 'error: pkg-config is required to find SDL3\n' >&2
    exit 1
fi
if ! pkg-config --exists sdl3; then
    printf 'error: SDL3 development files not found.\n' >&2
    printf '       install the SDL3 dev package (libsdl3-dev / sdl3)\n' >&2
    printf '       and make sure `pkg-config --modversion sdl3` works.\n' >&2
    exit 1
fi

printf 'building flowery (C++20) against SDL3 %s\n' "$(pkg-config --modversion sdl3)"
exec make "$@"
