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
baseDirectory=${1}; Start=${2}; bStart=${3}; cStart=${4}; aEnd=${5}; bEnd=${6}; cEnd=${7};
multiplier=${8}; computeExecutionDir=${9}; plotSize=${10}; makeCommand=${11}
makeDirectoryFile=`printf "%s.mkdir" $makeCommand`
touch $makeDirectoryFile
printf "%s%s/usr/bin/env dash\n" '#' '!' > $makeDirectoryFile
chmod +x $makeDirectoryFile
for a in `seq $Start $aEnd`
do
	printf "%s %s %s/images/%04d/%04d\n" "mkdir" "-p" "${baseDirectory}" $plotSize $a >> $makeDirectoryFile
	for b in `seq $bStart $bEnd`
	do
		printf "%s %s %s/%s/%04d/%04d/%04d\n" "mkdir" "-p" "${baseDirectory}" "images" ${plotSize} ${a} ${b} >> $makeDirectoryFile
		for c in `seq $cStart $cEnd`
		do
			x=$(( $a * $multiplier ))
			y=$(( $b * $multiplier ))
			z=$(( $c * $multiplier ))
			printf "%s/%s %d %d %d %s/%s/%04d/%04d/%04d %d\n" "${computeExecutionDir}" "cx.sh" "${x}" "${y}" "${z}" "${baseDirectory}" "images"  "${plotSize}" "${a}" "${b}" "${plotSize}"
		done
	done
done