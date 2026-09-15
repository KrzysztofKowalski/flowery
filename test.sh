#!/bin/sh
# test.sh — build and run the flowery tests.
#
#    ./test.sh                    unit tests + every legacy gnuplot sample
#    FLOWERY_SVG_TESTS=0 ./test.sh    unit tests only
#    ./test.sh <dir-or-file>...   compare specific files instead
#
# The legacy SVG files are the reference output of the original gnuplot
# pipeline, so they double as a fidelity check on the port.
set -eu
cd "$(dirname "$0")"

CC=${CC:-cc}
CFLAGS=${CFLAGS:--O2 -std=c11 -Wall -Wextra}
BUILD=.build
TEST=$BUILD/test_flowery

printf 'building %s\n' "$TEST"
mkdir -p "$BUILD"
# shellcheck disable=SC2086  # CFLAGS is a list on purpose
$CC $CFLAGS -I src -o "$TEST" tests/test_flowery.c src/flowery.c -lm

printf 'unit tests\n'
"$TEST"

if [ "$#" -gt 0 ]; then
    printf 'comparing against %s\n' "$*"
    "$TEST" --svg "$@"
elif [ "${FLOWERY_SVG_TESTS:-1}" = 1 ] && [ -d legacy/samples ]; then
    printf 'comparing against the legacy gnuplot samples\n'
    "$TEST" --svg legacy/samples
fi

printf 'all tests passed\n'
