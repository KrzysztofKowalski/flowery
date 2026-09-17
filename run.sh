#!/bin/sh
# run.sh — build if needed, then run the interactive renderer.
#
#    ./run.sh                 the default wheel (480 648 816), fullscreen
#    ./run.sh 5 7 12          custom wheels
#    ./run.sh 4 7 9 5000      custom wheels and sample count
#
# Fullscreen on the panel's native resolution by default. A windowed run is
# not the gentler option it looks like here: Hyprland resizes the window
# underneath the app on its own, and only fullscreen gets the real back
# buffer. FLOWERY_FULLSCREEN=0 ./run.sh asks for a window instead.
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

# Only a default: anything already in the environment wins, so
# FLOWERY_FULLSCREEN=0 ./run.sh gives a window.
FLOWERY_FULLSCREEN="${FLOWERY_FULLSCREEN:-1}"
export FLOWERY_FULLSCREEN

exec ./flowery "$@"
