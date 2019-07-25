#!/usr/bin/env dash

basedir=$1
imgdir=$2

mkdir -p $basedir 
mkdir -p $imgdir

plotsize=2001

cmf="parallelcommands.sh"

cat /dev/null > $cmf

for a in `seq 0 555`
do
	printf "%s %s %s\n" ./subprepare.sh $a $plotsize >> $cmf
done

parallel -j 48 --bar --progress --eta < $cmf