#!/usr/bin/env dash
printf "Generating directories\n"
find out -type f -iname '*.sh.mkdir' | sort -n > makedirectories.sh
parallel --progress --bar -eta -j 24 < makedirectories.sh
printf "Going with execution...\n"
find out -type f -iname '*.sh' | sort -n > commands.sh
for a in `cat commands.sh`
do
  printf "Executing %s\b" %a
  parallel --progress --bar -eta -j 64 < $a
done