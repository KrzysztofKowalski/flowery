#!/usr/bin/env dash
cp base.md README.md
S='!'
for a in $(ls -1 samples );
do
  [[ -a "$f" ]] || break
  s=`printf "%s\n" $a | cut -d'-' -f2-4 | cut -d'.' -f1 | tr '-' ' '`
  printf "Values %s\n%s[%s](./samples/%s)\n" $s $S $s $a >> README.md
done