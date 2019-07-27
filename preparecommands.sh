#!/usr/bin/env dash

basedir=$1
imgdir=$2
plotsize=$3
firstloopstart=1
secondloopstart=1
firstend=1
secondend=1
mkdir -p $basedir 
mkdir -p $imgdir
howmanyiterationsofthirdloopyouneed = 3333
cmf="parallelcommands.sh"
cat /dev/null > $cmf
increment=1

for a in `seq $firstloopstart $firstend`
do
	printf "%s %s %s %s %s %s %s %s %s %s %s\n" "./subprepare.sh" "$a" "$plotsize" "$secondloopstart" "$secondend" "$basedir" "$imgdir" "${howmanyiterationsofthirdloopyouneed}" >> $cmf
done

cat $cmf

parallel -j 36 --bar --progress --eta < $cmf