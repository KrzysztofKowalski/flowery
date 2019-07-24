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

for a in `seq $sa $ea`
do
	aformat=`printf "%04d" $a `
	printf "%s\n" "mkdir -p ${basedir}/image_${aformat}"
	for b in `seq $sb $eb`
	do
		bformat=`printf "%04d" $b `
		printf "%s\n" "mkdir -p ${basedir}/image_${aformat}/${bformat}"
		for c in `seq $sc $ec`
		do
			x=$(( $a * $m ))
			y=$(( $b * $m ))
			z=$(( $c * $m ))
			printf "%s %s %s %s %s\n" "${cxdir}/cx.sh" $x $y $z "${basedir}/image_${aformat}/${bformat}"
		done
	done
done

