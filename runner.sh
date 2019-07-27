#!/usr/bin/env dash
baseDirectory=${1}; Start=${2}; bStart=${3}; cStart=${4}; aEnd=${5}; bEnd=${6}; cEnd=${7};
multiplier=${8}; computeExecutionDir=${9}; plotSize=${10}; makeCommand=${11}
makeDirectoryFile=`printf "%s.mkdir" $makeCommand`
touch $makeDirectoryFile
printf "%s%s/usr/bin/env dash\n" '#' '!' > $makeDirectoryFile
chmod +x $makeDirectoryFile
for a in `seq $Start $aEnd`
do
	aformat=`printf "%04d" $a`
	printf "%s\n" "mkdir -p ${baseDirectory}/image_${aformat}" >> $makeDirectoryFile
	for b in `seq $bStart $bEnd`
	do
		bformat=`"%04d" $b`
		printf "%s %s %s $s/image_%s/%b\n" "mkdir" "-p"" "${baseDirectory}" "${aformat}" "${bformat}"" >> $makeDirectoryFile
		for c in `seq $cStart $cEnd`
		do
			x=$(( $a * $multiplier ))
			y=$(( $b * $multiplier ))
			z=$(( $c * $multiplier ))
			printf "%s %s %s %s %s %s\n" "${computeExecutionDir}/cx.sh" $x $y $z \
				"${baseDirectory}/image_${aformat}/${bformat}" $plotSize
		done
	done
done