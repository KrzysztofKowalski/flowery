#!/bin/sh
# run.sh — build if needed, then run the interactive renderer.
#
#    ./run.sh                 the default wheel (480 648 816)
#    ./run.sh 5 7 12          custom wheels
#    ./run.sh 4 7 9 5000      custom wheels and sample count
#
# Needs a display; for a headless smoke test use
#    SDL_VIDEODRIVER=offscreen ./run.sh
set -eu
cd "$(dirname "$0")"

if [ ! -x ./flowery ]; then
    printf 'flowery not built yet, building it first\n'
    ./build.sh
fi

if [ -z "${DISPLAY:-}${WAYLAND_DISPLAY:-}${SDL_VIDEODRIVER:-}" ]; then
    printf 'warning: no DISPLAY or WAYLAND_DISPLAY; try\n' >&2
    printf '         SDL_VIDEODRIVER=offscreen ./run.sh\n' >&2
fi

exec ./flowery "$@"
