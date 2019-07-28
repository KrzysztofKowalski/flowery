#!/usr/bin/env sh
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
cp base.md README.md
S='!'

printf "%s %s\n" "\#\#" "Samples" > samples.md
for a in $(ls -1 samples );
do
  s=`printf "%s\n" $a | cut -d'-' -f2-5 | cut -d'.' -f1 | tr '-' ' '`
  printf "\nValues X: %s Y: %s Z: %s, P: %s \n%s[%s Y: %s Z: %s, P: %s](./samples/%s)\n" $s "$S" $s "$a" >> samples.md
done

cat footer.md >> README.md