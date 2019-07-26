#!/usr/bin/env dash

basedir=$1
imgdir=$2

sa=1
sb=1
za=55
zb=999

mkdir -p $basedir 
mkdir -p $imgdir

plotsize=2001

cmf="parallelcommands.sh"

cat /dev/null > $cmf

for a in `seq $sa $za`
do
	printf "%s %s %s %s %s %s %s\n" ./subprepare.sh $a $plotsize $sb $zb $basedir  >> $cmf
done

parallel -j 12 --bar --progress --eta < $cmf