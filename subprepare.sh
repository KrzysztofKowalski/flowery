#!/usr/bin/env dash

a=$1
plotsize=$2

aformat=`printf "%04d" $a `
for b in `seq 0 100`
do
	bformat=`printf "%04d" $b`
	commanddir="out/commands/${aformat}/"
	
	mkdir -p $commanddir
	
	x=$(( $a ))
	y=$(( $b ))
	z=1
	g=$x
	h=$y
	j=999
	increment=1
	
	output="${commanddir}/${bformat}.sh"
	
	./runner.sh $imgdir $x $y $z $g $h $j $increment $basedir $plotsize > $output
	
	chmod +x $output
done