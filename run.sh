#!/usr/bin/env dash

printf "%s" "Cleaniing..."

time rm -fr out; 

printf "%s" "Cleaned!"

printf "%s" "Generation"

time ./preparecommands.sh `pwd` out/images; 

printf "%s" "Commands ready"

printf "%s" "Launching"

time ./launch.sh 