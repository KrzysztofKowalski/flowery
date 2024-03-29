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
baseDirectory=${1}; imageDirectory=${2}; plotSize=${3}; aStart=${4}; bStart=${5}; aEnd=${6}; bEnd=${7}; cStart=${8}; cEnd=${9}
mkdir -p $baseDirectory
mkdir -p $imageDirectory
parallelCommandsFile="parallelcommands.sh"
cat /dev/null > $parallelCommandsFile
loopMultiplier=1
for a in `seq $aStart $aEnd`
do
	printf "%s %s %s %s %s %s %s %s %s %s\n" "./subprepare.sh" "${a}" "${plotSize}" "${bStart}" "${bEnd}" "${baseDirectory}" "${imageDirectory}" "${loopMultiplier}" "${cStart}" "${cEnd}" >> "${parallelCommandsFile}"
done
parallel -j 36 --bar --progress --eta < $parallelCommandsFile