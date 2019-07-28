#!/usr/bin/env dash
#  Copyright (C) 2019 Krzysztof Kowalski
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.
aIteration=$1; plotSize=$2; bStart=$3; bEnd=$4; baseDirectory=$5; imageDirectory=$6;
multiplier=$7; thirdLoopStart=$8; thirdLoopEnd=$9; aIterationFormatted=`printf "%04d" $aIteration `
for b in `seq $bStart $bEnd`
do
  bformat=`printf "%04d" $b`
	outpuCommandDirectory="out/commands/${aIterationFormatted}/"
	mkdir -p $outpuCommandDirectory
	outputCommandPath="${outpuCommandDirectory}/${bformat}.sh"
	printf "%s %s\n" "#!/usr/bin/env" "dash" > $outputCommandPath
	./runner.sh $imageDirectory $aIteration $b $thirdLoopStart $aIteration $b $thirdLoopEnd $multiplier $baseDirectory $plotSize $outputCommandPath >> $outputCommandPath
	chmod +x $outputCommandPath
done