#!/usr/bin/env dash

printf "%s\n" "Cleaning..."

time rm -fr out; 

printf "%s\n" "Cleaned!"

printf "%s\n" "Generation"

time ./preparecommands.sh `pwd` out/images; 

printf "%s\n" "Commands ready"

printf "%s\n" "Launching"

time ./launch.sh 