#!/usr/bin/env dash

basedir=$1

imgdir=$2

plotsize=$3

sa=1

sb=1

za=55

zb=999

mkdir -p $basedir 
mkdir -p $imgdir



cmf="parallelcommands.sh"

cat /dev/null > $cmf

for a in `seq $sa $za`
do
	printf "%s %s %s %s %s %s %s %s\n" "./subprepare.sh" "$a" "$plotsize" "$sb" "$zb" "$basedir" "$imgdir" >> $cmf
done

cat $cmf

parallel -j 36 --bar --progress --eta < $cmf

