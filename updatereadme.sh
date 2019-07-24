#!/usr/bin/env bash

cp base.md README.md

for a in `ls -1 samples `; do s=`echo $a|cut -d'-' -f2-4|cut -d'.' -f1|tr '-' ' '|xargs echo Values: `; echo Values: $s >> README.md; echo >>README.md ; S='!'; echo "$S[$s](./samples/$a)" >> README.md; echo >> README.md ; done;

