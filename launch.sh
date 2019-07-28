#!/usr/bin/env dash
#  Copyright (C) <year>  <name of author>
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
printf "Generating directories\n"
find out -type f -iname '*.sh.mkdir' | sort -n > makedirectories.sh
parallel --progress --bar -eta -j 24 < makedirectories.sh
printf "Going with execution...\n"
find out -type f -iname '*.sh' | sort -n > commands.sh
for a in `cat commands.sh`
do
  printf "Executing %s\b" %a
  parallel --progress --bar -eta -j 512 < $a
done