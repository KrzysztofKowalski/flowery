#!/usr/bin/env dash
baseDirectory=$1; imageDirectory=$2; plotSize=$3; aStart=1; bStart=1; aEnd=1; bEnd=1; cStart=1; cEnd=99999
mkdir -p $baseDirectory
mkdir -p $imageDirectory
parallelCommandsFile="parallelcommands.sh"
cat /dev/null > $parallelCommandsFile
loopMultiplier=1
for a in `seq $aStart $aEnd`
do
	printf "%s %s %s %s %s %s %s %s %s %s\n" "./subprepare.sh" "$a" "$plotSize" "$bStart" "$bEnd" "$baseDirectory" "$imageDirectory" "${loopMultiplier}" "$cStart" "$cEnd" >> $parallelCommandsFile
done
cat $parallelCommandsFile
parallel -j 36 --bar --progress --eta < $parallelCommandsFile