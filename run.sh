#!/usr/bin/dash

time rm -fr out; 

time ./preparecommands.sh `pwd` out/images; 

time ./launch.sh 