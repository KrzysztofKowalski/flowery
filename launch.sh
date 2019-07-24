#!/usr/bin/env dash

find out -type f -iname '*.sh' | sort -n > commands.sh

parallel --progress --bar -eta -j 24 < commands.sh 