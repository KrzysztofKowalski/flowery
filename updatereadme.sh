#!/usr/bin/env sh
cp base.md README.md
S='!'
for a in $(ls -1 samples );
do
  s=`printf "%s\n" $a | cut -d'-' -f2-5 | cut -d'.' -f1 | tr '-' ' '`
  printf "Values X: %s Y: %s Z: %s, P: %s \n%s[%s Y: %s Z: %s, P: %s](./samples/%s)\n" $s "$S" $s "$a" >> README.md
done