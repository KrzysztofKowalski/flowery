#!/usr/bin/env dash

time rm -fr out; 

time ./preparecommands.sh `pwd` out/images; 

time ./launch.sh 