#!/usr/bin/env dash

basedir=$1
sa=$2
sb=$3
sc=$4
ea=$5
eb=$6
ec=$7
m=$8
cxdir=$9
plotsize=${10}

for a in `seq $sa $ea`
do
	
	printf "%04d" $a -- "aformat"
	printf "%s\n" "mkdir -p ${basedir}/image_${aformat}"
	for b in `seq $sb $eb`
	do
		printf "%04d" $b -- "$bformat"
		printf "%s\n" "mkdir -p ${basedir}/image_${aformat}/${bformat}"
		for c in `seq $sc $ec`
		do
			x=$(( $a * $m ))
			y=$(( $b * $m ))
			z=$(( $c * $m ))
			printf "%s %s %s %s %s %s\n" "${cxdir}/cx.sh" $x $y $z \
				"${basedir}/image_${aformat}/${bformat}" $plotsize
		done
	done
done