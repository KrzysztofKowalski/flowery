#!/bin/sh
# test.sh — build and run the flowery tests.
#
#    ./test.sh                    unit tests + every legacy gnuplot sample
#    FLOWERY_SVG_TESTS=0 ./test.sh    unit tests only
#    ./test.sh <dir-or-file>...   compare specific files instead
#
# The legacy SVG files are the reference output of the original gnuplot
# pipeline, so they double as a fidelity check on the port.
#
# The test binary is built by the same rule (and so the same flags) as the
# application, via make -- otherwise the tests could pass against a scalar
# build of code that ships vectorised, or the other way round.
set -eu
cd "$(dirname "$0")"

TEST=.build/test_flowery

printf 'building %s\n' "$TEST"
make -s "$TEST"

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
