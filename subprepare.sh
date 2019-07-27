#!/usr/bin/env dash
aIteration=$1; plotSize=$2; bStart=$3; bEnd=$4; baseDirectory=$5; imageDirectory=$6;
incrementSize=$7; thirdLoopStart=$8; thirdLoopEnd=$9; aIterationFormatted=`printf "%04d" $aIteration `
for b in `seq $bStart $bEnd`
do
  bformat=`printf "%04d" $b`
	outpuCommandDirectory="out/commands/${aIterationFormatted}/"
	mkdir -p $outpuCommandDirectory
	outputCommandPath="${outpuCommandDirectory}/${bformat}.sh"
	printf "%s %s\n" "#!/usr/bin/env" "dash" > $outputCommandPath
	./runner.sh $imageDirectory $aIteration $b $thirdLoopStart $aIteration $b $thirdLoopEnd $incrementSize $baseDirectory $plotSize $outputCommandPath >> $outputCommandPath
	chmod +x $outputCommandPath
done