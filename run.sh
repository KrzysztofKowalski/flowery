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
printf "%s\n" "Cleaning..."
rm -fr out/commands
printf "%s\n" "Cleaned!"
printf "%s\n" "Generation"
cSize=720
if [ -n "${1}" ]; then cSize=$1; fi
plotSize=$cSize
aSize=1
bSize=$cSize
./preparecommands.sh "$(pwd)" "out/images" "${plotSize}" "1" "1" "${aSize}" "${bSize}" "1" "${cSize}"
printf "%s\n" "Commands ready"
printf "%s\n" "Launching"
./launch.sh